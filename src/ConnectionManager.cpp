/*
 * $Id$
 *
 * File:   ConnectionManager.cpp
 * Author: Werner Jaeger
 *
 * Created on July 7, 2010, 5:44 PM
 */

#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QMessageBox>
#include <qt4/QtCore/qprocess.h>
//#include <QDebug>

#include "util/NetworkInterfaceMonitor.h"
#include "util/SmartCardState.h"
#include "dialogs/ConnectionEditorDialog.h"
#include "dialogs/ConnectionInformationDialog.h"
#include "settings/ConnectionSettings.h"
#include "L2tpIPsecVpnApplication.h"
#include "ConnectionState.h"
#include "VPNControlTask.h"
#include "ConnectionManager.h"

/*!
    \class ConnectionManager

    \brief The ConnectionManager class manages L2TP over IPsec VPN connections.
*/

/*!
    Creates a new ConnectionManager object.
*/

ConnectionManager::ConnectionManager(L2tpIPsecVpnApplication& application, QObject* pParent) : QObject(pParent), m_pConnectionInformation(new ConnectionInformationDialog()),
   m_pTimeout(new QTimer), m_pActions(new ActionList()), m_Application(application), m_pState(new NotConnected), m_fIsExecuting(false), m_fRoutePriorityIsChanging(false)
{
   connect(&m_Application, SIGNAL(connectionAdded(const QString&)), SLOT(onConnectionAdded(const QString&)));
   connect(&m_Application, SIGNAL(connectionRemoved(const QString&)), SLOT(onConnectionRemoved(const QString&)));

   m_pTimeout->setInterval(40000);
   m_pTimeout->setSingleShot(true);
   connect(m_pTimeout, SIGNAL(timeout()), SLOT(onTimeout()));

   createActions();

   m_pVPNControlTask = new VPNControlTask(this);
   connect(m_pVPNControlTask, SIGNAL(commandOutputReceived(const QString&)), SLOT(onCommandOutput(const QString&)));
   connect(m_pVPNControlTask, SIGNAL(readyReadLog()), SLOT(readyReadLog()));
   connect(m_pVPNControlTask, SIGNAL(errorMessageEmited(int)), SLOT(onErrorMsg(int)));
   connect(m_pVPNControlTask, SIGNAL(finished()), SLOT(onVpnTaskFinished()));
}

/*!
    Frees the resources associated with this ConnectionManager object.
*/
ConnectionManager::~ConnectionManager()
{
   NetworkInterfaceMonitor::instance()->stop();

   delete m_pConnectionInformation;

   while (!m_pActions->isEmpty())
      delete m_pActions->takeFirst();

   delete m_pTimeout;
   delete m_pActions;
   delete m_pState;
   delete m_pTrayIcon;
   delete m_pTrayIconMenu;
   delete m_pVPNControlTask;
}

int ConnectionManager::exec()
{
   int iRet(1);

   // at startup it might last few seconds until a try is available, lets try a view times before giving up
   for (int i = 0; iRet != 0 && i < 10; i++)
   {
      if (QSystemTrayIcon::isSystemTrayAvailable())
         iRet = 0;
      else
         ::sleep(1);
   }

   if (iRet == 0)
   {
      createTrayIcon();
      onStatusChanged();

      const ConnectionSettings settings;
      const int iSize = settings.connections();
      for (int i = 0; i < iSize; i++)
         onConnectionAdded(settings.connection(i));

      m_pTrayIcon->show();

      QTimer::singleShot(100, this, SLOT(detectConnectionState()));

      iRet = m_Application.exec();
   }
   else
      QMessageBox::critical(NULL, m_Application.applicationName(), QObject::tr("I couldn't detect any system tray on this system."));

   return(iRet);
}

void ConnectionManager::createActions()
{
   m_pActions->insert(DISC, new QAction(tr("&Disconnect"), this));
   m_pActions->insert(EDIT, new QAction(tr("&Edit Connections ..."), this));
   m_pActions->insert(INFO, new QAction(tr("Connection &Information"), this));
   m_pActions->insert(QUIT, new QAction(tr("&Quit"), this));

   connect(action(DISC), SIGNAL(triggered()), SLOT(vpnDisconnect()));
   connect(action(EDIT), SIGNAL(triggered()), SLOT(editConnections()));
   connect(action(INFO), SIGNAL(triggered()), SLOT(showConnectionInformation()));
   connect(action(QUIT), SIGNAL(triggered()), &m_Application, SLOT(quit()));
}

void ConnectionManager::createTrayIcon()
{
   m_pTrayIconMenu = new QMenu();
   m_pTrayIconMenu->addAction(action(DISC));
   m_pTrayIconMenu->addSeparator();
   m_pTrayIconMenu->addAction(action(EDIT));
   m_pTrayIconMenu->addAction(action(INFO));
   m_pTrayIconMenu->addSeparator();
   m_pTrayIconMenu->addAction(action(QUIT));

   m_pTrayIcon = new QSystemTrayIcon(this);
   m_pTrayIcon->setContextMenu(m_pTrayIconMenu);
   connect(m_pTrayIconMenu, SIGNAL(triggered(QAction*)), SLOT(vpnConnect(QAction*)));
   connect(m_pTrayIcon, SIGNAL(messageClicked()), SLOT(messageClicked()));
   connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void ConnectionManager::onStatusChanged()
{
   if (m_pTimeout->isActive())
      m_pTimeout->stop();

   if (m_pState->isState(ConnectionState::Connected))
   {
      enableAllConnections(false);
      action(DISC)->setEnabled(true);
   }
   else if (m_pState->isState(ConnectionState::Connecting))
   {
      enableAllConnections(false);
      action(DISC)->setEnabled(true);
   }
   else if (m_pState->isState(ConnectionState::Disconnecting))
   {
      enableAllConnections(false);
      action(DISC)->setEnabled(false);
   }
   else if (m_pState->isState(ConnectionState::Error))
   {
      enableAllConnections(true);
      action(DISC)->setEnabled(true);
   }
   else
   {
      enableAllConnections(true);
      action(DISC)->setEnabled(false);
   }

   const QIcon& icon = m_pState->icon();
   m_pTrayIcon->setIcon(icon);
   m_pTrayIcon->setToolTip(m_pState->msgTitle());
   m_pConnectionInformation->setWindowIcon(icon);
   showMessage();
}

void ConnectionManager::vpnConnect(const QString& strConnectionName)
{
//   qDebug() << "ConnectionManager::vpnConnect(const QString&" << strConnectionName << ")";

   if (!m_fIsExecuting)
   {
      const bool fUseSmartCard = ConnectionSettings().pppSettings(strConnectionName).eapSettings().useSmartCard();

      m_pConnectionInformation->clearLog();

      SmartCardState sc;

      if (fUseSmartCard && sc.readersAvailable() <= 0)
         m_pVPNControlTask->restartPcscDaemon();

      if (!fUseSmartCard || sc.readersAvailable() > 0)
      {
         if (!fUseSmartCard || sc.waitForCardPresent() > 0)
         {
            const QString strGateway(ConnectionSettings().gateway(strConnectionName));

            delete m_pState;
            m_pState = new Connecting(strGateway);
            onStatusChanged();

            m_pVPNControlTask->setConnectionName(strConnectionName);
            m_pVPNControlTask->setAction(VPNControlTask::Connect);
            m_pVPNControlTask->start();
            m_fIsExecuting = true;
            m_pTimeout->start();
         }
      }
      else
         QMessageBox::critical(NULL, qApp->applicationName(), QObject::tr("No smart card reader found."));
   }

//   qDebug() << "ConnectionManager::vpnConnect(const QString&" << strConnectionName << ") -> finished";
}


void ConnectionManager::vpnConnect(QAction* pAction)
{
   if (m_pTrayIconMenu && pAction)
   {
      if (!pAction->data().isNull())
         vpnConnect(pAction->data().toString());
   }
}

void ConnectionManager::vpnDisconnect(bool fDontChangeStatus)
{
//   qDebug() << "ConnectionManager::vpnDisconnect(bool" << fDontChangeStatus << ")";

   if (!m_fIsExecuting)
   {
      if (!fDontChangeStatus)
      {
         const ConnectionSettings settings;
         const QString strGateway(settings.gateway(m_pVPNControlTask->connectionName()));

         delete m_pState;
         m_pState = new Disconnecting(strGateway);

         onStatusChanged();
      }

      m_pVPNControlTask->setAction(VPNControlTask::Disconnect);
      m_pVPNControlTask->start();
      m_fIsExecuting = true;
      m_pTimeout->start();
   }

//   qDebug() << "ConnectionManager::vpnDisconnect(bool" << fDontChangeStatus << ") -> finished";
}

void ConnectionManager::editConnections() const
{
   m_Application.startConnectionEditorDialog();
}

void ConnectionManager::showConnectionInformation() const
{
   m_pConnectionInformation->exec();
}

void ConnectionManager::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
   switch (reason)
   {
      case QSystemTrayIcon::MiddleClick:
         showMessage();
         break;

      default:
         ;
   }
}

void ConnectionManager::showMessage()
{
   m_pTrayIcon->showMessage(m_pState->msgTitle(), m_pState->msgBody(), m_pState->msgIcon(), 5 * 1000);
}

void ConnectionManager::messageClicked()
{
   showConnectionInformation();
}

void ConnectionManager::onCommandOutput(const QString& strOutputLine)
{
//   qDebug() << "ConnectionManager::onCommandOutput(const QString&" << strOutputLine << ")";

   if (strOutputLine.trimmed().length() > 0)
      m_pConnectionInformation->appendLogPlainText((strOutputLine + '\n').toAscii().constData());

//   qDebug() << "ConnectionManager::onCommandOutput(const QString&" << strOutputLine << ") -> finished";
}

void ConnectionManager::readyReadLog()
{
//   qDebug() << "ConnectionManager::readyReadLog()";

   char acBuf[1024];

   qint64 iRet;
   do
   {
      iRet = m_pVPNControlTask->readLogLine(acBuf, sizeof(acBuf));
      if (iRet > 0)
         m_pConnectionInformation->appendLogPlainText(acBuf);

   } while (iRet != -1);

//   qDebug() << "ConnectionManager::readyReadLog() -> finished";
}

void ConnectionManager::onErrorMsg(int iErrorCode)
{
//   qDebug() << "ConnectionManager::onErrorMsg(int" << iErrorCode << ")";

   char acBuf[1024];

   const qint64 iLineLength = m_pVPNControlTask->readErrorLine(acBuf, sizeof(acBuf));
   if (iLineLength > 0)
   {
      m_pConnectionInformation->appendLogColorText(QColor(255, 0, 0), acBuf);
      error(iErrorCode);
   }

//   qDebug() << "ConnectionManager::onErrorMsg(int" << iErrorCode << ") -> finished";
}

void ConnectionManager::onTimeout()
{
//   qDebug() << "ConnectionManager::onConnectionTimeout()";
   if ((m_pState->isState(ConnectionState::Connecting) || m_pState->isState(ConnectionState::Disconnecting)))
      error(500);

//   qDebug() << "ConnectionManager::onConnectionTimeout() -> finised";
}

void ConnectionManager::onConnectionAdded(const QString& strName)
{
//   qDebug() << "ConnectionManager::onConnectionAdded(const QString&" << strName << ")";

   if (m_pTrayIconMenu && m_pActions)
   {
      m_pActions->append(new QAction(strName, this));
      m_pActions->last()->setData(strName);
      m_pActions->last()->setToolTip(tr("Click to establish a vpn connection to '%1'").arg(strName));

      if (m_pState->isState(ConnectionState::NotConnected) || m_pState->isState(ConnectionState::Error))
         m_pActions->last()->setEnabled(true);
      else
         m_pActions->last()->setEnabled(false);

      m_pTrayIconMenu->insertAction(action(DISC), m_pActions->last());
   }

//   qDebug() << "ConnectionManager::onConnectionAdded(const QString&" << strName << ") -> finished";
}

void ConnectionManager::onConnectionRemoved(const QString& strName)
{
//   qDebug() << "ConnectionManager::onConnectionRemoved(const QString&" << strName << ")";

   if (m_pTrayIconMenu && m_pActions)
   {
      QAction* pAction = NULL;
      for (int i = QUIT + 1; !pAction && i < m_pActions->size(); i++)
      {
         if (m_pActions->at(i)->text() == strName)
            pAction = m_pActions->at(i);
      }

      if (pAction)
      {
         m_pTrayIconMenu->removeAction(pAction);
         m_pActions->removeAll(pAction);
         delete pAction;
      }
   }

//   qDebug() << "ConnectionManager::onConnectionRemoved(const QString&" << strName << ") -> finished";
}

void ConnectionManager::onRouteAdded(NetworkInterface interface, unsigned int iPriority)
{
//   qDebug() << "ConnectionManager::onRouteAdded(" << interface.name().c_str() << ", " << iPriority << ")";

   if (!interface.isPtP())
   {
      if (iPriority != 100)
      {
         if (!m_fRoutePriorityIsChanging)
         {
            bool fHasDefaultGateway = false;
            if (!interface.isNull())
            {
               if (interface.routeEntries().size() == 1)
               {
//                  qDebug() << "ConnectionManager::onRouteAdded(" << interface.name().c_str() << "): dst =" << (*interface.routeEntries().begin()).ip() << "gw =" << (*interface.routeEntries().begin()).broadcast() << "gw is null =" << (*interface.routeEntries().begin()).broadcast().isNull();
                  fHasDefaultGateway = !(*interface.routeEntries().begin()).broadcast().isNull() && (*interface.routeEntries().begin()).ip().isNull();
               }
            }
            else
               fHasDefaultGateway = NetworkInterface::defaultGateway().size() == 1;

            if (m_pState && m_pState->isState(ConnectionState::NotConnected) && fHasDefaultGateway)
            {
//               qDebug() << "ConnectionManager::onRouteAdded: found default gateway";

               ConnectionSettings settings;
               const int iConnections = settings.connections();

               bool fDone = false;
               for (int i = 0; !fDone && i < iConnections; i++)
               {
                  const QString strName = settings.connection(i);
                  if (settings.commonSettings(strName).autoConnect())
                  {
                     fDone = true;
                     vpnConnect(strName);
                  }
               }
            }
         }
         else
            m_fRoutePriorityIsChanging = false;
      }
      else
         m_fRoutePriorityIsChanging = true;
   }

//   qDebug() << "ConnectionManager::onRouteAdded(" << interface.name().c_str() << ", " << iPriority << ") -> finished";
}

void ConnectionManager::onRouteDeleted(NetworkInterface interface, unsigned int iPriority)
{
//   qDebug() << "ConnectionManager::onRouteDeleted(" << interface.name().c_str() << ", " << iPriority << ")";

   if (!interface.isPtP())
   {
      if (iPriority != 100)
      {
         if (!m_fRoutePriorityIsChanging)
         {
            if (m_pState && m_pState->isState(ConnectionState::Connected) && NetworkInterface::defaultGateway().size() == 0)
               vpnDisconnect();
         }
         else
            m_fRoutePriorityIsChanging = false;
      }
      else
         m_fRoutePriorityIsChanging = true;
   }

//   qDebug() << "ConnectionManager::onRouteDeleted(" << interface.name().c_str() << ") -> finished";
}

void ConnectionManager::onPtpInterfaceIsUpAnRunning(NetworkInterface interface)
{
//   qDebug() << "ConnectionManager::onPtpInterfaceIsUpAnRunning(" << interface.name().c_str() << ")";

   if (m_pState->isState(ConnectionState::Connecting) || m_pState->isState(ConnectionState::NotConnected))
   {
      const QString strConnectionName(ConnectionManager::connectionName(interface));
      if (!strConnectionName.isNull())
         connected(strConnectionName);
   }

//   qDebug() << "ConnectionManager::onPtpInterfaceIsUpAnRunning(" << interface.name().c_str() << ", " << iPriority << ") -> finished";
}

void ConnectionManager::onPtpInterfaceIsGoingDown(NetworkInterface interface)
{
//   qDebug() << "ConnectionManager::onPtpInterfaceIsGoingDown(" << interface.name().c_str() << ")";

   if ((m_pState->isState(ConnectionState::Connected) || m_pState->isState(ConnectionState::Disconnecting)))
   {
      const QString strConnectionName(ConnectionManager::connectionName(interface));
      if (!strConnectionName.isNull())
         disConnected();
   }

//   qDebug() << "ConnectionManager::onPtpInterfaceIsGoingDown(" << interface.name().c_str() << ") -> finished";
}

void ConnectionManager::detectConnectionState()
{
   NetworkInterface::InterfaceMap interfaces = NetworkInterface::pointToPointInterfaces();

   NetworkInterface::InterfaceMap::const_iterator itInterfaces = interfaces.begin();
   bool fFound;
   for (fFound = false; !fFound && itInterfaces != interfaces.end(); itInterfaces++)
   {
      const QString strConnectionName(ConnectionManager::connectionName((*itInterfaces).second));
      if (!strConnectionName.isNull())
      {
         if (NetworkInterface::defaultGateway().size() == 1)
            connected(strConnectionName);
         else
            vpnDisconnect(true);

         fFound = true;
      }
   }

   if (!fFound)
      onRouteAdded(NetworkInterface::null, 0);

   NetworkInterfaceMonitor::instance()->subscribe(this);
   NetworkInterfaceMonitor::instance()->start();
}

QAction* ConnectionManager::action(ActionType type) const
{
   return(m_pActions != NULL ? m_pActions->at(type) : NULL);
}

void ConnectionManager::enableAllConnections(bool fEnable) const
{
   for (int i = QUIT + 1; i < m_pActions->size(); i++)
      m_pActions->at(i)->setEnabled(fEnable);
}

void ConnectionManager::connected(const QString& strConnectionName)
{
   const QString strGateway(ConnectionSettings().gateway(strConnectionName));

   m_pVPNControlTask->setConnectionName(strConnectionName);

   delete m_pState;

   m_pState = new Connected(strGateway);

   onStatusChanged();
}

void ConnectionManager::disConnected()
{
   delete m_pState;

   m_pState = new NotConnected();

   onStatusChanged();
}

void ConnectionManager::error(int iErrorCode)
{
   if (!m_pState->isState(ConnectionState::NotConnected))
   {
      const ConnectionSettings settings;
      const QString strGateway(settings.ipsecSettings(m_pVPNControlTask->connectionName()).gateway());
      const bool fDisconnecting = !m_pState->isState(ConnectionState::Connecting);

      delete m_pState;
      m_pState = new Error(strGateway, iErrorCode, fDisconnecting);
      onStatusChanged();

      vpnDisconnect(true);
   }
}

QString ConnectionManager::connectionName(const NetworkInterface& interface)
{
   QString strConnectionName;

   const NetworkInterface::AddressEntries addressEntries(interface.addressEntries());
   NetworkInterface::AddressEntries::const_iterator addressIterator(addressEntries.begin());
   for (bool fFound = false; !fFound && addressIterator != addressEntries.end(); addressIterator++)
   {
      strConnectionName = ConnectionSettings().name((*addressIterator).broadcast());
      if (!strConnectionName.isNull())
         fFound = true;
   }

   return(strConnectionName);
}
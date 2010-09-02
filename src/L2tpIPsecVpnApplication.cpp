/*
 * $Id$
 *
 * File:   L2tpIPsecVpnApplication.cpp
 * Author: Werner Jaeger
 *
 * Created on August 9, 2010, 2:29 PM
 */

#include <QMetaType>
#include <QLocalSocket>
#include <QProcess>

#include "localpeer/LocalPeer.h"
#include "ConnectionManager.h"
#include "L2tpIPsecVpnApplication.h"
#include "ConnectionEditor.h"

static const char* const CONNECTIONEDITOR_CMD_SWITCH = "connectionEditor";
static QString const APPLICATIONNAME = "L2TP IPsec VPN Manager";
static QString const GKSUDO_CMD = "gksudo -D \"" + APPLICATIONNAME + "\" ";
static QString const CONNECTION_ADDED_MSG_PREFIX = "connectionAdded:";
static QString const CONNECTION_REMOVED_MSG_PREFIX = "connectionRemoved:";

L2tpIPsecVpnApplication::L2tpIPsecVpnApplication(int& iArgc, char** ppArgv) : QApplication(iArgc, ppArgv), m_pProcess(new QProcess), m_pLocalPeer(new LocalPeer())
{
   setOrganizationName("WernerJaeger");
   setOrganizationDomain("wejaeger.com");
   setApplicationName(APPLICATIONNAME);
   setApplicationVersion("1.0.0");
   setObjectName("L2tpIPsecVpn");
   qRegisterMetaType<NetworkInterface>("NetworkInterface");
   qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");

   connect(m_pLocalPeer, SIGNAL(messageReceived(const QString&)), SLOT(onMessageReceived(const QString&)));
   connect(m_pProcess, SIGNAL(finished(int)), this, SLOT(onConnectionEditorDialogClosed(int)));

   if (mode() == CONNECTION_MANAGER)
      setQuitOnLastWindowClosed(false);
}

L2tpIPsecVpnApplication::~L2tpIPsecVpnApplication()
{
   delete m_pLocalPeer;
   delete m_pProcess;
}

bool L2tpIPsecVpnApplication::isRunning()
{
   return(m_pLocalPeer->isClient());
}

L2tpIPsecVpnApplication::APPLICATIONMODE L2tpIPsecVpnApplication::mode() const
{
   return(isConnectionEditor() ? CONNECTION_EDITOR : (isPasswordCallback() ? PASSWORD_CALLBACK : CONNECTION_MANAGER));
}

bool L2tpIPsecVpnApplication::sendConnectionAddedMessage(const QString& strConnectionName)
{
   return(m_pLocalPeer->sendMessage(CONNECTION_ADDED_MSG_PREFIX + strConnectionName, 5000));
}

bool L2tpIPsecVpnApplication::sendConnectionRemovedMessage(const QString& strConnectionName)
{
   return(m_pLocalPeer->sendMessage(CONNECTION_REMOVED_MSG_PREFIX + strConnectionName, 5000));
}

int L2tpIPsecVpnApplication::startConnectionEditorDialog() const
{
   m_pProcess->start(GKSUDO_CMD + arguments()[0] + " " + CONNECTIONEDITOR_CMD_SWITCH);

   return(0);
}

void L2tpIPsecVpnApplication::onMessageReceived(const QString& strMessage)
{
   if (strMessage.startsWith(CONNECTION_ADDED_MSG_PREFIX))
      emit connectionAdded(strMessage.mid(strMessage.indexOf(':') + 1));
   else if (strMessage.startsWith(CONNECTION_REMOVED_MSG_PREFIX))
      emit connectionRemoved(strMessage.mid(strMessage.indexOf(':') + 1));
}

void L2tpIPsecVpnApplication::onConnectionEditorDialogClosed(int iExitCode)
{
   emit connectionEditorDialogClosed(iExitCode);
}

bool L2tpIPsecVpnApplication::isConnectionEditor() const
{
   return(arguments().count() == 2 && arguments()[1] == CONNECTIONEDITOR_CMD_SWITCH);
}

bool L2tpIPsecVpnApplication::isPasswordCallback() const
{
   return(arguments().count() == 4);
}

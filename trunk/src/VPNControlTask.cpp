/*
 * $Id$
 *
 * File:   VPNControlTask.cpp
 * Author: Werner Jaeger
 *
 * Created on July 8, 2010, 5:09 PM
 */

#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QSocketNotifier>
#include <QMessageBox>
#include <QDebug>

#include <unistd.h>
#include <fcntl.h>

#include "conf/ConfWriter.h"
#include "util/VpnControlDaemonClient.h"
#include "VPNControlTask.h"
#include "ConnectionManager.h"

static const QFile plutoPid("/var/run/pluto/pluto.pid");
static const QFile xl2tpdPid("/var/run/xl2tpd.pid");

static const char* const strVpnLogPipeName("/var/log/l2tpipsecvpn.pipe");

static const QRegExp RE_LOG_SPLITLINE("\\s(?=\\w+\\[\\d+\\]\\:\\s)");
static const QRegExp RE_LOG_CAP_CERTIFICATEID("\\'(\\d\\:\\d{1,3})\\'");

static const char* const STR_LOG_MATCH_IPSECSAESTABLISHED = "IPsec SA established";

static const char* const STR_LOG_MATCH_CERTIFICATELOADERROR = "Error loading certificate";
static const char* const STR_LOG_MATCH_AUTHFAILED = "LCP terminated by peer (Authentication failed)";
static const char* const STR_LOG_MATCH_NO_DATA = "No data from BIO_read";
static const char* const STR_LOG_MATCH_PEERAUTHFAILED = "but I couldn't find any suitable secret (password) for it to use to do so.";

QFile VPNControlTask::m_vpnLogPipe(strVpnLogPipeName);

VPNControlTask::VPNControlTask(QObject* pParent) : QThread(pParent), m_pControlClient(new VpnControlDaemonClient), m_Action(Connect), m_iReturnCode(0),
   m_fIPSecConnectionIsUp(false), m_pByteArray(new QByteArray()), m_pErrorStream(new QTextStream(m_pByteArray))
{
   clearVpnLogPipe();

   if (!m_vpnLogPipe.open(QIODevice::ReadWrite | QIODevice::Text))
      qWarning("Warning: VPNControlTask: Failed to open pipe %s", strVpnLogPipeName);

   ::fcntl(m_vpnLogPipe.handle(), F_SETFL, O_NONBLOCK);

   m_pVpnLogPipeNotifier = new QSocketNotifier(m_vpnLogPipe.handle(), QSocketNotifier::Read, pParent);

   connect(m_pVpnLogPipeNotifier, SIGNAL(activated(int)), this, SLOT(readyReadVpnLogPipe()));
}

VPNControlTask::~VPNControlTask()
{
   deleteControlClient();
   delete m_pErrorStream;
   delete m_pByteArray;
   delete m_pVpnLogPipeNotifier;
   m_vpnLogPipe.close();

}

void VPNControlTask::setConnectionName(const QString& strConnectionName)
{
   m_strConnectionName = strConnectionName;
}

const QString& VPNControlTask::connectionName() const
{
   return(m_strConnectionName);
}

VPNControlTask::Action VPNControlTask::action() const
{
   return(m_Action);
}

void VPNControlTask::setAction(Action action)
{
   m_Action = action;
}

int VPNControlTask::restartPcscDaemon()
{
//   qDebug() << "VPNControlTask::restartPcscDaemon()";

   if (createControlClient())
   {
      m_iReturnCode = 0;

      runAndWait(VpnClientConnection::CMD_START_PCSCD);
      runAndWait(VpnClientConnection::CMD_STOP_PCSCD);
      sleep(2);
   }


   deleteControlClient();

   return(m_iReturnCode);
}

void VPNControlTask::run()
{
//   qDebug() << "VPNControlTask::run()";

   if (createControlClient())
   {
      m_pByteArray->clear();
      m_pErrorStream->reset();
      m_pErrorStream->resetStatus();
      m_iReturnCode = 0;
      m_fIPSecConnectionIsUp = false;

      switch (m_Action)
      {
         case Connect:
            runConnect();
            break;

         case Disconnect:
            runDisconnect();
            break;
      }
   }

   deleteControlClient();

//   qDebug() << "VPNControlTask::run() -> finished";
}

bool VPNControlTask::createControlClient()
{
   deleteControlClient();
   m_pControlClient = new VpnControlDaemonClient;
   connect(m_pControlClient, SIGNAL(notifyResult(int, const QString&)), SLOT(onResult(int, const QString&)));
   connect(m_pControlClient, SIGNAL(notifyCommandOutput(const QString&)), SLOT(onCommandOutput(const QString&)));

   m_pControlClient->connectToServer();

   const bool fConnected(m_pControlClient->waitForConnected());

   if (!fConnected)
   {
      m_iReturnCode = 99;
      emitErrorMsg("");
   }

   return(fConnected);
}

void VPNControlTask::deleteControlClient()
{
   if (m_pControlClient != NULL)
   {
      m_pControlClient->leave();
      m_pControlClient->deleteLater();
      m_pControlClient = NULL;
   }
}

void VPNControlTask::runConnect()
{
//   qDebug() << "VPNControlTask::runConnect()";

   if (!plutoPid.exists())
      runAndWait(VpnClientConnection::CMD_START_IPSECD);

   if (m_iReturnCode == 0)
   {
      if (!xl2tpdPid.exists())
         runAndWait(VpnClientConnection::CMD_START_L2TPD);

      if (m_iReturnCode == 0)
      {
         // avoid need --listen before --initiate error
         ::sleep(2);

         runAndWait(VpnClientConnection::CMD_IPSEC_UP, m_strConnectionName);

         if (!m_fIPSecConnectionIsUp)
         {
            m_iReturnCode = 300;
            emitErrorMsg("IPsec");
         }

         if (m_iReturnCode == 0)
            runAndWait(VpnClientConnection::CMD_L2TP_CONNECT, m_strConnectionName);
      }
   }
//   qDebug() << "VPNControlTask::runConnect() -> finished";
}

void VPNControlTask::runDisconnect()
{
//   qDebug() << "VPNControlTask::runDisconnect()";

   if (xl2tpdPid.exists())
      runAndWait(VpnClientConnection::CMD_STOP_L2TPD);

   if (m_iReturnCode == 0)
      runAndWait(VpnClientConnection::CMD_STOP_IPSECD);

//   qDebug() << "VPNControlTask::runDisconnect() -> finished";
}

void VPNControlTask::runAndWait(VpnClientConnection::Command iCommand, const QString strArguments)
{
//   qDebug() << "VPNControlTask::runAndWait(Command" << iCommand << ", const QString&" <<  strArguments << ")";

   if (!m_pControlClient->start(iCommand, strArguments))
   {
      m_iReturnCode = 99;
      emitErrorMsg("");
   }
   else
      exec();

//   qDebug() << "VPNControlTask::runAndWait(Command" << iCommand << ", const QString&" <<  strArguments << ") -> finished";
}

qint64 VPNControlTask::readLogLine(char* data, qint64 iMaxSize)
{
//   qDebug() << "VPNControlTask::readLogLine()";

   qint64 iRet = m_vpnLogPipe.readLine(data, iMaxSize);

   if (iRet > 0)
   {
      const QString strLine(data);
      const QStringList astrParts = strLine.split(RE_LOG_SPLITLINE);
      ::strcpy(data, astrParts.last().toAscii().data());
      iRet = ::strlen(data);

      if (::strstr(data, STR_LOG_MATCH_CERTIFICATELOADERROR) != NULL)
      {
         m_iReturnCode = 400;
         if (RE_LOG_CAP_CERTIFICATEID.indexIn(data) > 0)
            emitErrorMsg(RE_LOG_CAP_CERTIFICATEID.cap(1));
         else
            emitErrorMsg("unknown");
      }
      if (::strstr(data, STR_LOG_MATCH_AUTHFAILED) != NULL)
      {
         m_iReturnCode = 404;
         emitErrorMsg(connectionName());
      }
      else if (::strstr(data, STR_LOG_MATCH_NO_DATA))
      {
         m_iReturnCode = 405;
         emitErrorMsg(connectionName());
      }
      else if (::strstr(data, STR_LOG_MATCH_PEERAUTHFAILED))
      {
         m_iReturnCode = 406;
         emitErrorMsg(connectionName());
      }
   }

//   qDebug() << "VPNControlTask::readLogLine() -> finished with" << iRet;

   return(iRet);
}

qint64 VPNControlTask::readErrorLine(char* data, qint64 iMaxSize)
{
//   qDebug() << "VPNControlTask::readErrorLine()";

   qint64 iRet = -1;

   QString strLine = m_pErrorStream->readLine(iMaxSize);
   if (!strLine.isNull())
   {
      strLine.append("\n");
      ::strcpy(data, strLine.toAscii().data());
      iRet = ::strlen(data);
   }

//   qDebug() << "VPNControlTask::readErrorLine() -> finished with" << iRet;

   return(iRet);
}

void VPNControlTask::readyReadVpnLogPipe()
{
   emit readyReadLog();
}

void VPNControlTask::onResult(int iReturnCode, const QString& strCommand)
{
//   qDebug() << "VPNControlTask::onResult(int" << iReturnCode << ", const String&" << strCommand << ")";

   m_iReturnCode = iReturnCode;

   exit(iReturnCode);

   if (iReturnCode != 0)
      emitErrorMsg(strCommand);

//   qDebug() << "VPNControlTask::onResult(int" << iReturnCode << ", const String&" << strCommand << ") -> finished";
}

void VPNControlTask::onCommandOutput(const QString& strOutputLine)
{
//   qDebug() << "VPNControlTask::onCommandOutput(const String&" << strOutputLine << ")";

   if (!m_fIPSecConnectionIsUp)
      m_fIPSecConnectionIsUp = strOutputLine.contains(STR_LOG_MATCH_IPSECSAESTABLISHED);

   emit commandOutputReceived(strOutputLine);

//   qDebug() << "VPNControlTask::onCommandOutput(const String&" << strOutputLine << ") --> finished";
}

void VPNControlTask::emitErrorMsg(const QString& strErrorContext)
{
//   qDebug() << "VPNControlTask::emitErrorMsg(const QString&" << strErrorContext << ")";

   const qint64 iPos = m_pErrorStream->pos();

   *m_pErrorStream << "[ERROR" << qSetFieldWidth(5) << right << m_iReturnCode << reset << "]   ";

   switch (m_iReturnCode)
   {
      case 90:
         *m_pErrorStream << "L2tpIPsecVpnControlDaemon did not recognize the command sent";
         break;

      case 91:
         *m_pErrorStream << "L2tpIPsecVpnControlDaemon is complained about the number of arguments in command '" << strErrorContext << "'";
         break;

      case 99:
         *m_pErrorStream << "L2tpIPsecVpnControlDaemon is either not started or connection to it failed";
         break;

      case 100:
         *m_pErrorStream << "The process '" << strErrorContext << "' failed to start";
         break;

      case 200:
         *m_pErrorStream << "Failed to write command '" << strErrorContext << "' to l2tp-control";
         break;

      case 210:
         *m_pErrorStream << "Failed to open l2tp control file '" << strErrorContext << "'";
         break;

      case 220:
         *m_pErrorStream << "Failed to open l2tp ipsec vpn log file '" << strErrorContext << "'";
         break;

      case 300:
         *m_pErrorStream << "'" << strErrorContext << "' failed to negotiate or establish security associations";
         break;

      case 310:
         *m_pErrorStream << "'" << strErrorContext << "' did not start connecting to peer within given time";
         break;

      case 400:
         *m_pErrorStream << "Error loading certificate with id '" << strErrorContext << "'";
         break;

      case 404:
         *m_pErrorStream << "Authentication failed: closing connection to '" << strErrorContext << "'";
         break;

      case 405:
         *m_pErrorStream << "Peer did not accept certificate sent from smart card: closing connection to '" << strErrorContext << "'";
         break;

      case 406:
         *m_pErrorStream << "No secret found to authenticate  '" << strErrorContext << "'";
         break;

      case 500:
         *m_pErrorStream << "Connection to '" << strErrorContext << "' timed out";
         break;

      default:
         *m_pErrorStream << "Command '" << strErrorContext << "' failed and exited with given error code";
         break;
   }
   m_pErrorStream->flush();
   m_pErrorStream->seek(iPos);
   emit errorMessageEmited(m_iReturnCode);
//   qDebug() << "VPNControlTask::emitErrorMsg(const QString&" << strErrorContext << ") -> finished";
}

void VPNControlTask::clearVpnLogPipe()
{
   const int iVpnPipeFileDescriptor = ::open(m_vpnLogPipe.fileName().toAscii().data(), O_RDONLY | O_NONBLOCK);

   if (iVpnPipeFileDescriptor != -1)
   {
      char* buf[1024];
      int iNoRead;

      do
         iNoRead = ::read(iVpnPipeFileDescriptor, buf, sizeof(buf));
      while (iNoRead > 0);

      ::close(iVpnPipeFileDescriptor);
   }
   else
   {
      const int iResult(VpnControlDaemonClient::execute(VpnClientConnection::CMD_CREATE_VPN_LOGPIPE, m_vpnLogPipe.fileName()));
      if (iResult != VpnClientConnection::OK)
         QMessageBox::critical(NULL, tr("A critical error occurred"), tr("Create vpn syslog pipe command failed with exit code: %1").arg(iResult));
   }
}

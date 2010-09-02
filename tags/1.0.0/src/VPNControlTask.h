/*
 * $Id$
 *
 * File:   VPNControlTask.h
 * Author: Werner Jaeger
 *
 * Created on July 8, 2010, 5:09 PM
 */

#ifndef VPNCONTROLTASK_H
#define	VPNCONTROLTASK_H

#include <QThread>

#include "VpnClientConnection.h"

class QFile;
class QByteArray;
class QTextStream;
class QSocketNotifier;
class VpnControlDaemonClient;

class VPNControlTask : public QThread
{
   Q_OBJECT

public:
   enum Action {Connect, Disconnect};

   explicit VPNControlTask(QObject* pParent);
   virtual ~VPNControlTask();

   void run();

   void setConnectionName(const QString& strConnectionName);
   const QString& connectionName() const;
   void setAction(Action action);
   Action action() const;
   int restartPcscDaemon();

   qint64 readLogLine(char* data, qint64 iMaxSize);
   qint64 readErrorLine(char* data, qint64 iMaxSize);

signals:
   void readyReadLog();
   void commandOutputReceived(const QString& strOutputLine);
   void errorMessageEmited(int iErrorCode);

private slots:
   void readyReadVpnLogPipe();
   void onResult(int iReturnCode, const QString& stCommand);
   void onCommandOutput(const QString& strOutputLine);

private:
   VPNControlTask(const VPNControlTask& orig);
   VPNControlTask& operator=(const VPNControlTask& orig);

   bool createControlClient();
   void deleteControlClient();
   void runConnect();
   void runDisconnect();
   void runAndWait(VpnClientConnection::Command iCommand, const QString strArguments = "");
   void emitErrorMsg(const QString& strErrorContext);
   void clearVpnLogPipe();

   VpnControlDaemonClient* m_pControlClient;
   QString m_strConnectionName;
   Action m_Action;
   volatile int m_iReturnCode;
   volatile bool m_fIPSecConnectionIsUp;
   QByteArray* const m_pByteArray;
   QTextStream* const m_pErrorStream;
   QSocketNotifier* m_pVpnLogPipeNotifier;

   static QFile m_vpnLogPipe;
};

#endif	/* VPNCONTROLTASK_H */

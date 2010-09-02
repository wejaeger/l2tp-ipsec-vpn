/*
 * $Id$
 *
 * File:   VpnControlDaemonClient.h
 * Author: Werner Jaeger
 *
 * Created on August 26, 2010, 12:48 PM
 */

#ifndef VPNCONTROLDAEMONCLIENT_H
#define	VPNCONTROLDAEMONCLIENT_H

#include <QPointer>

#include "VpnClientConnection.h"

class QTextStream;
class QLocalSocket;

class VpnControlDaemonClient : public QObject
{
   Q_OBJECT

public:
   explicit VpnControlDaemonClient(QObject* pParent = NULL);
   virtual ~VpnControlDaemonClient();

   void connectToServer();
   bool waitForConnected(int iMiliSeconds = 30000);
   bool start(VpnClientConnection::Command iCommand, const QString& strArguments = QString::null);
   static int execute(VpnClientConnection::Command iCommand, const QString& strArguments = QString::null);

signals:
   void notifyConnected();
   void notifyDisconnected();
   void notifyResult(int iReturnCode, const QString& strCommand);
   void notifyCommandOutput(const QString& strOutputLine);

public slots:
   void exit();
   void leave();

private slots:
   void onConnected();
   void onDisconnected();
   void onReadyRead();

private:
   explicit VpnControlDaemonClient(bool fSynchronous);
   VpnControlDaemonClient(const VpnControlDaemonClient& orig);
   VpnControlDaemonClient& operator=(const VpnControlDaemonClient& orig);

   int waitForResult();

   struct Result
   {
      int iReturnCode;
      VpnClientConnection::ResponseType type;
   };

   Result readResponse();

   QTextStream* const m_pStream;
   QPointer<QLocalSocket> const m_pSocket;
};

#endif	/* VPNCONTROLDAEMONCLIENT_H */


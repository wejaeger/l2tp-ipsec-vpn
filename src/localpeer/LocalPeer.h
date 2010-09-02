/*
 * $Id$
 *
 * File:   LocalPeer.h
 * Author:  Werner Jaeger
 *
 * Created on September 2, 2010, 5:47 PM
 */

#ifndef LOCALPEER_H
#define	LOCALPEER_H

#include <QObject>

#include "LockedFile.h"

class QLocalServer;

class LocalPeer : public QObject
{
   Q_OBJECT

public:
   LocalPeer(QObject* pParent = NULL, const QString& strAppId = QString());
   virtual ~LocalPeer();

   bool isClient();
   QString applicationId() const;
   bool sendMessage(const QString &strMessage, int iTimeout);

signals:
   void messageReceived(const QString& strMessage);

protected slots:
   void receiveConnection();

protected:
private:
   LocalPeer(const LocalPeer& orig);
   LocalPeer& operator=(const LocalPeer& orig);

   QString m_strApplicationId;
   QString m_strSocketName;
   QLocalServer* const m_pServer;
   LockedFile m_LockedFile;

   static const char* m_pcAck;
};

#endif	/* LOCALPEER_H */


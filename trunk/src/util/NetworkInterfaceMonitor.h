/*
 * $Id$
 *
 * File:   NetworkInterfaceMonitor.h
 * Author: Werner Jaeger
 *
 * Created on August 13, 2010, 8:01 AM
 */

#ifndef NETWORKINTERFACEMONITOR_H
#define	NETWORKINTERFACEMONITOR_H

#include <QThread>
#include <QList>

#include "NetworkInterface.h"

class NetworkInterfaceMonitor : public QThread
{
   Q_OBJECT

public:
   virtual ~NetworkInterfaceMonitor();

   void run();
   void stop();
   void subscribe(const QObject* pSubscriber);
   void unSubscribe(const QObject* pSubscriber);

   static NetworkInterfaceMonitor* instance();

protected:
   NetworkInterfaceMonitor();

signals:
   void routeAdded(NetworkInterface interface, unsigned int iPriority) const;
   void routeDeleted(NetworkInterface interface, unsigned int iPriority) const;
   void ptpInterfaceIsUpAnRunning(NetworkInterface interface) const;
   void ptpInterfaceIsGoingDown(NetworkInterface interface) const;

private:
   NetworkInterfaceMonitor(const NetworkInterfaceMonitor& orig);
   NetworkInterfaceMonitor& operator=(const NetworkInterfaceMonitor& orig);

   void handleRoutingMessage(struct nlmsghdr* pNetLinkMessageHeader);
   void handleInterfaceInfoMessage(struct nlmsghdr* pNetLinkMessageHeader);
   void handleAddressMessage(struct nlmsghdr* pNetLinkMessageHeader);

#ifndef QT_NO_DEBUG
   static void debugFlags(unsigned iFlags);
#endif

   QList<const QObject*> m_Subscribers;
   int m_iSocket;

   NetworkInterface::InterfaceMap m_Interfaces;

   static NetworkInterfaceMonitor* m_pInstance;
};

#endif	/* NETWORKINTERFACEMONITOR_H */


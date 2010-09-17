/*
 * $Id$
 *
 * File:   NetworkInterface.h
 * Author: Werner Jaeger
 *
 * Created on July 12, 2010, 9:10 PM
 *
 * Copyright 2010 Werner Jaeger.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NETWORKINTERFACE_H
#define	NETWORKINTERFACE_H

#include <QObject>
#include <QNetworkAddressEntry>

#include <map>
#include <vector>
#include <string>

class NetworkInterface
{
public:
   typedef std::map<std::string, NetworkInterface> InterfaceMap;
   typedef std::pair<std::string, NetworkInterface> InterfaceMapEntry;
   typedef std::vector<QNetworkAddressEntry> AddressEntries;
   typedef std::vector<QNetworkAddressEntry> RouteEntries;
   typedef unsigned long uint32;

   enum InterfaceFlag
   {
     IsUp = 0x1,
     IsRunning = 0x2,
     CanBroadcast = 0x4,
     IsLoopBack = 0x8,
     IsPointToPoint = 0x10,
     CanMulticast = 0x20
   };
   Q_DECLARE_FLAGS(InterfaceFlags, InterfaceFlag)

   NetworkInterface() : m_strName(""), m_iIndex(0), m_Flags(0) {}
   NetworkInterface(const NetworkInterface& orig);

   virtual ~NetworkInterface() {}

   const std::string& name() const { return(m_strName); }
   unsigned int index() const { return(m_iIndex); }
   InterfaceFlags flags() const { return(m_Flags); }
   bool isNull() const { return(m_strName.empty()); }
   bool isPtP() const { return(m_Flags & IsPointToPoint); }

   const AddressEntries& addressEntries() const { return(m_AddressEntries); }
   void clearAddressEntries(void) { m_AddressEntries.clear(); }
   const AddressEntries& routeEntries() const { return(m_RouteEntries); }
   void clearRouteEntries(void) { m_RouteEntries.clear(); }

   static InterfaceMap pointToPointInterfaces(void);
   static InterfaceMap defaultGateway(void);
   static InterfaceMap ipsecPhysicalGateway(void);

   static const NetworkInterface null;

private:
   NetworkInterface(const char* const cstrName, int iIndex, uint iRawFlags) : m_strName(cstrName), m_iIndex(iIndex), m_Flags(convertFlags(iRawFlags)), m_AddressEntries(std::vector<QNetworkAddressEntry>()), m_RouteEntries(std::vector<QNetworkAddressEntry>()) {}
   NetworkInterface& operator=(const NetworkInterface& orig);

   void addAddressEntry(const QNetworkAddressEntry& addressEntry) { m_AddressEntries.push_back(addressEntry); }
   bool removeAddressEntry(const QNetworkAddressEntry& addressEntry);
   void addRouteEntry(const QNetworkAddressEntry& routeEntry) { m_RouteEntries.push_back(routeEntry); }
   bool removeRouteEntry(const QNetworkAddressEntry& routeEntry);

   void setFlags(uint iRawFlags) { m_Flags = convertFlags(iRawFlags); }

   static InterfaceFlags convertFlags(uint iRawFlags);

   const std::string m_strName;
   const unsigned int m_iIndex;
   InterfaceFlags m_Flags;

   std::vector<QNetworkAddressEntry> m_AddressEntries;
   std::vector<QNetworkAddressEntry> m_RouteEntries;

   friend class NetworkInterfaceMonitor;
};

#endif	/* NETWORKINTERFACE_H */


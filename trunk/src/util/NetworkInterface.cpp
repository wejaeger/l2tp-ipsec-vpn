/*
 * $Id$
 *
 * File:   NetworkInterface.cpp
 * Author: Werner Jaeger
 *
 * Created on July 12, 2010, 6:07 PM
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

//#include <QDebug>

#include <netinet/in.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sstream>
#include <fstream>

#include "NetworkInterface.h"

const NetworkInterface NetworkInterface::null(NetworkInterface("", 0, 0));

NetworkInterface::NetworkInterface(const NetworkInterface& orig) : m_strName(orig.m_strName), m_iIndex(orig.m_iIndex), m_Flags(orig.m_Flags), m_AddressEntries(orig.m_AddressEntries), m_RouteEntries(orig.m_RouteEntries)
{
}

bool NetworkInterface::operator==(const NetworkInterface& other) const
{
   return(m_strName.compare(other.m_strName) == 0);
}

bool NetworkInterface::removeAddressEntry(const QNetworkAddressEntry& addressEntry)
{
   bool fFound = false;
   AddressEntries::iterator it = m_AddressEntries.begin();
   while (!fFound && it != m_AddressEntries.end())
   {
      if ((*it++) == addressEntry)
      {
         fFound = true;
         it = m_AddressEntries.erase(it);
      }
   }
   return(fFound);
}

bool NetworkInterface::removeRouteEntry(const QNetworkAddressEntry& routeEntry)
{
   bool fFound = false;
   AddressEntries::iterator it = m_RouteEntries.begin();
   while (!fFound && it != m_RouteEntries.end())
   {
      if ((*it++) == routeEntry)
      {
         fFound = true;
         it = m_RouteEntries.erase(it);
      }
   }
   return(fFound);
}

NetworkInterface::InterfaceMap NetworkInterface::pointToPointInterfaces(void)
{
   InterfaceMap interfaces;

   struct ifaddrs* pInterfaceAddresses;

   if (::getifaddrs(&pInterfaceAddresses) == 0)
   {
      for (const struct ifaddrs* pIter = pInterfaceAddresses; pIter != NULL; pIter = pIter->ifa_next)
      {
         if (pIter->ifa_addr && (pIter->ifa_addr->sa_family == AF_INET || pIter->ifa_addr->sa_family == AF_INET6) && pIter->ifa_flags & IFF_POINTOPOINT && pIter->ifa_flags & IFF_RUNNING)
         {
            const InterfaceMapEntry entry(std::make_pair(pIter->ifa_name, NetworkInterface(pIter->ifa_name, ::if_nametoindex(pIter->ifa_name), pIter->ifa_flags)));
            std::pair<InterfaceMap::iterator, bool> ret = interfaces.insert(entry);

            QNetworkAddressEntry addressEntry;
            addressEntry.setIp(QHostAddress(pIter->ifa_addr));
            addressEntry.setNetmask(QHostAddress(pIter->ifa_netmask));
            addressEntry.setBroadcast(QHostAddress(pIter->ifa_dstaddr));

            (*ret.first).second.addAddressEntry(addressEntry);
         }
      }
      ::freeifaddrs(pInterfaceAddresses);
   }
   return(interfaces);
}

NetworkInterface::InterfaceMap NetworkInterface::defaultGateway(void)
{
   InterfaceMap interfaces;

   using namespace std;

   string strInterfaceName;
   uint32 iDestinationAddress = -1;
   uint32 iGatewayAddress = -1;

   ifstream route("/proc/net/route", ios::in);
   while (iDestinationAddress && route)
   {
      string strLine;
      getline(route, strLine);

//      qDebug() << "NetworkInterface::defaultGateway: route entry:" << strLine.c_str();

      istringstream strFormat(strLine);
      strFormat >> strInterfaceName >> hex >> iDestinationAddress >> hex >> iGatewayAddress ;
      if (!strFormat.good())
         iDestinationAddress = -1;
   }

   if (iDestinationAddress == 0 && !strInterfaceName.empty())
   {
//      qDebug() << "NetworkInterface::defaultGateway: found default gateway on interface" << strInterfaceName.c_str() << "dest Addr =" << iDestinationAddress << "gateway =" << iGatewayAddress;

      const InterfaceMapEntry entry(std::make_pair(strInterfaceName, NetworkInterface(strInterfaceName.c_str(), ::if_nametoindex(strInterfaceName.c_str()), IFF_UP | IFF_RUNNING)));
      std::pair<InterfaceMap::iterator, bool> ret = interfaces.insert(entry);

      QNetworkAddressEntry routeEntry;
      routeEntry.setIp(QHostAddress(iDestinationAddress));
      routeEntry.setNetmask(QHostAddress());
      routeEntry.setBroadcast(QHostAddress(iGatewayAddress));

      (*ret.first).second.addRouteEntry(routeEntry);
   }

   return(interfaces);
}

NetworkInterface::InterfaceMap NetworkInterface::ipsecPhysicalGateway(void)
{
   InterfaceMap interfaces;

   using namespace std;

   string strInterfaceName;
   string strDestination;
   string strGateway;

   ifstream ipsecInfo("/var/run/pluto/ipsec.info", ios::in);
   while (ipsecInfo)
   {
      string strLine;
      getline(ipsecInfo, strLine);

      if (strLine.length() > 0)
      {
         istringstream strFormat(strLine.replace(strLine.find('='), 1, " "));

         string strKey;
         string strValue;
         strFormat >> strKey >> strValue;

         if (strKey == "defaultroutephys")
            strInterfaceName = strValue;
         else if (strKey == "defaultrouteaddr")
            strDestination = strValue;
         else if (strKey == "defaultroutenexthop")
            strGateway = strValue;
      }
   }

   if (!strInterfaceName.empty())
   {
      const InterfaceMapEntry entry(std::make_pair(strInterfaceName, NetworkInterface(strInterfaceName.c_str(), ::if_nametoindex(strInterfaceName.c_str()), IFF_UP | IFF_RUNNING)));
      std::pair<InterfaceMap::iterator, bool> ret = interfaces.insert(entry);

      QNetworkAddressEntry addressEntry;
      addressEntry.setIp(QHostAddress(strDestination.c_str()));
      addressEntry.setNetmask(QHostAddress());
      addressEntry.setBroadcast(QHostAddress(strGateway.c_str()));

      (*ret.first).second.addAddressEntry(addressEntry);
   }

   return(interfaces);
}

NetworkInterface::InterfaceFlags NetworkInterface::convertFlags(uint iRawFlags)
{
   InterfaceFlags flags = InterfaceFlag(0);
   flags |= (iRawFlags & IFF_UP) ? IsUp : InterfaceFlag(0);
   flags |= (iRawFlags & IFF_RUNNING) ? IsRunning : InterfaceFlag(0);
   flags |= (iRawFlags & IFF_BROADCAST) ? CanBroadcast : InterfaceFlag(0);
   flags |= (iRawFlags & IFF_LOOPBACK) ? IsLoopBack : InterfaceFlag(0);
   flags |= (iRawFlags & IFF_POINTOPOINT) ? IsPointToPoint : InterfaceFlag(0);
   flags |= (iRawFlags & IFF_MULTICAST) ? CanMulticast : InterfaceFlag(0);
   return(flags);
}




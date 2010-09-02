/*
 * $Id$
 *
 * File:   PppDnsConfWriter.cpp
 * Author: Werner Jaeger
 *
 * Created on August 21, 2010, 6:43 AM
 */

#include <QCoreApplication>

#include "settings/ConnectionSettings.h"
#include "PppDnsConfWriter.h"

static const char* const DNS_SECTION = "DNS_SECTION";
static const char* const SEARCH_SECTION = "SEARCH_SECTION";
static const char* const IPADDRESS = "IPADDRESS";
static const char* const SEARCHLIST = "SEARCHLIST";

PppDnsConfWriter::PppDnsConfWriter(const QString& strTemplateKey, const QString& strWriteTo) : AbstractConfWriter(strTemplateKey, strWriteTo)
{
}

void PppDnsConfWriter::fill()
{
   QString strInstance(instance());
   const QString strConnectionName(strInstance.replace(QCoreApplication::instance()->objectName() + "-", ""));

   const PppSettings pppSettings = ConnectionSettings().pppSettings(strConnectionName);
   const PppIpSettings ipSettings(pppSettings.ipSettings());
   const bool fUsePeerDns = ipSettings.usePeerDns();

   if (!fUsePeerDns && !ipSettings.preferredDnsServerAddress().isEmpty())
      dictionary()->AddSectionDictionary(DNS_SECTION)->SetValue(IPADDRESS, ipSettings.preferredDnsServerAddress().toAscii().constData());

   if (!fUsePeerDns && !ipSettings.alternateDnsServerAddress().isEmpty())
      dictionary()->AddSectionDictionary(DNS_SECTION)->SetValue(IPADDRESS, ipSettings.alternateDnsServerAddress().toAscii().constData());

   if (!fUsePeerDns && !ipSettings.searchDomains().isEmpty())
      dictionary()->AddSectionDictionary(SEARCH_SECTION)->SetValue(SEARCHLIST, ipSettings.searchDomains().toAscii().constData());
}

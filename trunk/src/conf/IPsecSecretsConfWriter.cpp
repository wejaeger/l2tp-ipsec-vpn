/*
 * $Id$
 *
 * File:   IPsecSecretsConfWriter.cpp
 * Author: Werner Jaeger
 *
 * Created on August 8, 2010, 6:15 PM
 */

#include <QObject>
#include <ctemplate/template.h>
#include "settings/ConnectionSettings.h"
#include "IPsecSecretsConfWriter.h"

static const char* const AUTHBYRSASIG = "rsasig";
static const char* const AUTHBYPSK = "secret";

static const char* const CONN_SECTION = "CONN_SECTION";
static const char* const INDICES = "INDICES";
static const char* const KEYTYPE = "KEYTYPE";
static const char* const KEY = "KEY";
static const char* const PASSPHRASE = "PASSPHRASE";

static const char* const PSKKEYTYPE = "PSK";
static const char* const RSAKEYTYPE = "RSA";

IPsecSecretsConfWriter::IPsecSecretsConfWriter(const QString& strTemplateKey, const QString& strWriteTo) : AbstractConfWriter(strTemplateKey, strWriteTo, AbstractConfWriter::SECRET)
{
}

IPsecSecretsConfWriter::~IPsecSecretsConfWriter()
{
}

void IPsecSecretsConfWriter::fill()
{
   const ConnectionSettings settings;
   const int iConnections = settings.connections();

   for (int i = 0; i < iConnections; i++)
   {
      ctemplate::TemplateDictionary* const pConnection = dictionary()->AddSectionDictionary(CONN_SECTION);
      const QString strName(settings.connection(i));

      if (!strName.isEmpty())
      {
         const IPSecSettings ipsecSetting(settings.ipsecSettings(strName));
         const QString strGateway(ipsecSetting.gateway());

         if (!strGateway.isEmpty())
         {
            if (strGateway.at(0).isNumber())
               pConnection->SetValue(INDICES, strGateway.toAscii().constData());
            else
               pConnection->SetValue(INDICES, QString("@" + strGateway).toAscii().constData());
         }

         if (ipsecSetting.authBy() == AUTHBYRSASIG)
         {
            pConnection->SetValue(KEYTYPE, RSAKEYTYPE);
            pConnection->SetValue(KEY, QString('"' + ipsecSetting.privateKeyFilePath() + '"').toAscii().constData());
            pConnection->SetValue(PASSPHRASE, QString('"' + ipsecSetting.privateKeyPassphrase() + '"').toAscii().constData());
         }
         else
         {
            pConnection->SetValue(KEYTYPE, PSKKEYTYPE);
            pConnection->SetValue(KEY, QString('"' + ipsecSetting.preSharedKey() + '"').toAscii().constData());
         }
      }
      else
         addErrorMsg(QObject::tr("No such connection: '%1'.").arg(strName));
   }
}

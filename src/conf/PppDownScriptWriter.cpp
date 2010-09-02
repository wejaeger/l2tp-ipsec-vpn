/*
 * $Id$
 *
 * File:   PppDownScriptWriter.cpp
 * Author: Werner Jaeger
 *
 * Created on August 6, 2010, 1:28 PM
 */

#include <QCoreApplication>

#include <ctemplate/template.h>
#include "settings/ConnectionSettings.h"
#include "ConfWriter.h"
#include "PppDownScriptWriter.h"

static const char* const CONN_SECTION = "CONN_SECTION";
static const char* const DEFAULT_GATEWAY_SECTION = "DEFAULT_GATEWAY_SECTION";

static const char* const OBJECTNAME = "OBJECTNAME";
static const char* const GETIPSECINFOLIB = "GETIPSECINFOLIB";
static const char* const IPPARAM = "IPPARAM";

PppDownScriptWriter::PppDownScriptWriter(const QString& strTemplateKey, const QString& strWriteTo) : AbstractConfWriter(strTemplateKey, strWriteTo)
{
}

void PppDownScriptWriter::fill()
{
   dictionary()->SetValue(OBJECTNAME, QCoreApplication::instance()->objectName().toAscii().constData());
   dictionary()->SetValue(GETIPSECINFOLIB, ConfWriter::fileName(ConfWriter::GETIPSECINFO).toAscii().constData());

   const ConnectionSettings settings;
   const int iConnections = settings.connections();

   for (int i = 0; i < iConnections; i++)
   {
      const QString strName(settings.connection(i));

      if (!strName.isEmpty())
      {
         const PppIpSettings ipSetting(settings.pppSettings(strName).ipSettings());
         ctemplate::TemplateDictionary* const pConnection = dictionary()->AddSectionDictionary(CONN_SECTION);

         pConnection->SetValue(IPPARAM, (QCoreApplication::instance()->objectName() + "-" + strName).toAscii().constData());

         if (ipSetting.useDefaultGateway())
            pConnection->AddSectionDictionary(DEFAULT_GATEWAY_SECTION);
      }
      else
         addErrorMsg(QObject::tr("No such connection: '%1'.").arg(strName));
   }
}

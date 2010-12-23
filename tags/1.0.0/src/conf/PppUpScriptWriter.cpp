/*
 * $Id$
 *
 * File:   PppUpScriptWriter.cpp
 * Author: Werner Jaeger
 *
 * Created on August 6, 2010, 1:18 PM
 */

#include <QCoreApplication>

#include <ctemplate/template.h>
#include "conf/ConfWriter.h"
#include "settings/ConnectionSettings.h"
#include "PppUpScriptWriter.h"

static const char* const CONN_SECTION = "CONN_SECTION";
static const char* const ROUTE_SECTION = "ROUTE_SECTION";
static const char* const DEFAULT_GATEWAY_SECTION = "DEFAULT_GATEWAY_SECTION";

static const char* const OBJECTNAME = "OBJECTNAME";
static const char* const GETIPSECINFOLIB = "GETIPSECINFOLIB";
static const char* const IPPARAM = "IPPARAM";
static const char* const IPADDRESS = "IPADDRESS";
static const char* const IPNETMASK = "IPNETMASK";

PppUpScriptWriter::PppUpScriptWriter(const QString& strTemplateKey, const QString& strWriteTo) : AbstractConfWriter(strTemplateKey, strWriteTo, AbstractConfWriter::EXECUTABLE)
{
}

void PppUpScriptWriter::fill()
{
   dictionary()->SetValue(OBJECTNAME, QCoreApplication::instance()->objectName().toAscii().constData());
   dictionary()->SetValue(GETIPSECINFOLIB, ConfWriter::fileName(ConfWriter::GETIPSECINFO).toAscii().constData());

   const ConnectionSettings settings;
   const int iConnections = settings.connections();

   for (int i = 0; i < iConnections; i++)
   {
      ctemplate::TemplateDictionary* const pConnection = dictionary()->AddSectionDictionary(CONN_SECTION);
      const QString strName(settings.connection(i));

      if (!strName.isEmpty())
      {
         pConnection->SetValue(IPPARAM, (QCoreApplication::instance()->objectName() + "-" + strName).toAscii().constData());

         const PppIpSettings ipSetting(settings.pppSettings(strName).ipSettings());

         if (!ipSetting.useDefaultGateway())
         {
            ctemplate::TemplateDictionary* const pDefaultRoute = pConnection->AddSectionDictionary(ROUTE_SECTION);
            pDefaultRoute->SetValue(IPADDRESS, "`echo \"${PPP_LOCAL}\" | cut -d'.' -f1`.0.0.0");
            pDefaultRoute->SetValue(IPNETMASK, "255.0.0.0");

            const int iRoutes = ipSetting.routes();
            for (int j = 0; j < iRoutes; j++)
            {
               ctemplate::TemplateDictionary* const pRoute = pConnection->AddSectionDictionary(ROUTE_SECTION);
               pRoute->SetValue(IPADDRESS, ipSetting.routeAddress(j).toAscii().constData());
               pRoute->SetValue(IPNETMASK, ipSetting.routeNetmask(j).toAscii().constData());
            }
         }
         else
            pConnection->AddSectionDictionary(DEFAULT_GATEWAY_SECTION);
      }
      else
         addErrorMsg(QObject::tr("No such connection: '%1'.").arg(strName));
   }
}
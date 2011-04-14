/*
 * $Id$
 *
 * File:   main.cpp
 * Author: Werner Jaeger
 *
 * Created on July 7, 2010, 5:40 PM
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

#include <QMessageBox>

#include <syslog.h>

#include "pkcs11/Pkcs11.h"
#include "settings/Preferences.h"
#include "ConnectionManager.h"
#include "ConnectionEditor.h"
#include "PasswordCallback.h"
#include "L2tpIPsecVpnApplication.h"

void messageOutput(QtMsgType type, const char* pcMsg);

int main(int iArgc, char* pcArgv[])
{
   int iRet(0);

   L2tpIPsecVpnApplication app(iArgc, pcArgv);

   if (app.mode() == L2tpIPsecVpnApplication::CONNECTION_EDITOR || app.mode() == L2tpIPsecVpnApplication::CONNECTION_EDITORSTARTER || app.mode() == L2tpIPsecVpnApplication::PASSWORD_CALLBACK || !app.isRunning())
   {
      qInstallMsgHandler(messageOutput);
      Q_INIT_RESOURCE(L2tpIPsecVpn);

      if (!Pkcs11::loaded())
      {
         const QString strPkcs11Lib(Preferences().openSSLSettings().pkcs11Path());
         if (!strPkcs11Lib.isEmpty())
            iRet = Pkcs11::loadLibrary(Preferences().openSSLSettings().pkcs11Path(), true) ? 0 : 2;
      }

      if (iRet == 0)
      {
         switch (app.mode())
         {
            case L2tpIPsecVpnApplication::CONNECTION_MANAGER:
            {
               ConnectionManager manager(app);
               iRet = manager.exec();
            }
            break;

            case L2tpIPsecVpnApplication::CONNECTION_EDITOR:
            {
               ConnectionEditor connectionEditor(app);
               iRet = connectionEditor.exec();
            }
            break;

            case L2tpIPsecVpnApplication::PASSWORD_CALLBACK:
            {
               PasswordCallback callback(app);
               iRet = callback.exec();
            }
            break;

            case L2tpIPsecVpnApplication::CONNECTION_EDITORSTARTER:
               iRet = app.startConnectionEditorDialog(true);
               break;

            default:
               Q_ASSERT(false);
               break;
         }
      }
      else
         QMessageBox::critical(NULL, app.applicationName(), QObject::tr("I couldn't load PKCS11 library %1.").arg(Preferences().openSSLSettings().pkcs11Path()));
   }

   return(iRet);
}

void messageOutput(QtMsgType type, const char* pcMsg)
{
   switch (type)
   {
     case QtDebugMsg:
#ifndef QT_NO_DEBUG
        ::syslog(LOG_DEBUG, "%s", pcMsg);
#endif
         break;

     case QtWarningMsg:
        ::syslog(LOG_WARNING, "%s", pcMsg);
         break;

     case QtCriticalMsg:
        ::syslog(LOG_CRIT, "%s", pcMsg);
         break;

     case QtFatalMsg:
        ::syslog(LOG_EMERG, "%s", pcMsg);
         abort();
     }
 }

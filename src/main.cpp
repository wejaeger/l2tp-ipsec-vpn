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
#include <QDir>
#include <QFileInfoList>
#include <QFile>

#include <syslog.h>

#include "pkcs11/Pkcs11.h"
#include "settings/Preferences.h"
#include "ConnectionManager.h"
#include "ConnectionEditor.h"
#include "PasswordCallback.h"
#include "L2tpIPsecVpnApplication.h"

static const char* const DESKTOP_SESSION("DESKTOP_SESSION");
static const char* const DESKTOP_SESSION_CMD_SWITCH("desktopSession");
static const char* const PROCDIR("/proc/");

static void checkDesktop(int iArgc, char* pcArgv[]);
static uint effectiveUid();
void messageOutput(QtMsgType type, const char* pcMsg);

int main(int iArgc, char* pcArgv[])
{
   int iRet(0);

   qInstallMsgHandler(messageOutput);

   checkDesktop(iArgc, pcArgv);

   L2tpIPsecVpnApplication app(iArgc, pcArgv);

   if (app.mode() == L2tpIPsecVpnApplication::CONNECTION_EDITOR || app.mode() == L2tpIPsecVpnApplication::CONNECTION_EDITORSTARTER || app.mode() == L2tpIPsecVpnApplication::PASSWORD_CALLBACK || !app.isRunning())
   {
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

/**
 * If no windows manager is set,
 * probe the environment to automatically guess one.
 **/
static void checkDesktop(int iArgc, char* pcArgv[])
{
   // if no desktop is given, look if one is passed via command line
   const char* const pcDesktopSession(::getenv(DESKTOP_SESSION));
   if (!pcDesktopSession)
   {
      bool fDone(false);
      for (int i(1); !fDone && i < iArgc; i++)
      {
         if (i + 1 != iArgc) // Check that we haven't finished parsing already
         {
            if (::strcmp(pcArgv[i], DESKTOP_SESSION_CMD_SWITCH) == 0)
            {
               ::setenv(DESKTOP_SESSION, pcArgv[i + 1], 0);
               fDone = true;
            }
         }
      }

      // no desktop is set and no desktop is given on the command line.
      // most likely we are called via sudo
      // try to test for at least very well known running desktop managers
      if (!fDone)
      {
         const uint uiUid(effectiveUid());

         QFileInfoList procList(QDir(PROCDIR).entryInfoList(QDir::AllDirs|QDir::NoDotAndDotDot));

         for (QFileInfoList::const_iterator procIt(procList.constBegin()); procIt != procList.constEnd(); procIt++)
         {
            bool fOk(false);
            QString strPid((*procIt).fileName());
            strPid.toUInt(&fOk);

            if (fOk) // pid must be numeric, ignore every thing else
            {
               // is this process owned by the user
               if (uiUid == (*procIt).ownerId())
               {
                  // we have a valid pid
                  // open the cmdline file to determine what's the name of the process running
                  QFile cmdLineFile(PROCDIR + strPid + "/cmdline");
                  if (cmdLineFile.open(QFile::ReadOnly))
                  {
                     const QString strCli(cmdLineFile.readAll());
                     if (strCli.endsWith("gnome-session"))
                       ::setenv(DESKTOP_SESSION, "gnome", 0);
                     else if (strCli.endsWith("kcmserver"))
                        ::setenv(DESKTOP_SESSION, "kde", 0);
                     else if (strCli.endsWith("xfce-mcs-manage"))
                        ::setenv(DESKTOP_SESSION, "xfce", 0);
                  }
                  else
                     qWarning() << "Failed to open proc command line file" << cmdLineFile.fileName();
               }
            }
         }
      }
   }
}

static uint effectiveUid()
{
   uint uiUid(::getuid());

   const char* const pcSudoUid(::getenv("SUDO_UID"));
   if (pcSudoUid)
   {
      const uid_t uiSudoUid(::strtol(pcSudoUid, NULL, 0));
      if (uiSudoUid)
         uiUid = uiSudoUid;
   }

   return(uiUid);
}
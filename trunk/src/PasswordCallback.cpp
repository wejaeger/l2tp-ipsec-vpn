/*
 * $Id$
 *
 * File:   PasswordCallback.cpp
 * Author: Werner Jaeger
 *
 * Created on September 1, 2010, 4:20 PM
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

#include <QObject>
#include <QInputDialog>

#include <stdlib.h>

#include "settings/Preferences.h"
#include "settings/ConnectionSettings.h"
#include "L2tpIPsecVpnApplication.h"
#include "PasswordCallback.h"

PasswordCallback::PasswordCallback(L2tpIPsecVpnApplication& application) : m_Application(application)
{
}

PasswordCallback::~PasswordCallback()
{
}

int PasswordCallback::exec() const
{
   int iRet = 1;

   const ConnectionSettings settings;
   const int iConnections = settings.connections();

   QString strPassword;

   if (iConnections > 0)
   {
      for (int i = 0; strPassword.isNull() && i < iConnections; i++)
      {
         const QString strConnectionName(settings.connection(i));
         const PppSettings pppSettings(settings.pppSettings(strConnectionName));
         const PppEapSettings eapSettings(pppSettings.eapSettings());
         if (eapSettings.privateKeyPath() == m_Application.argv()[1])
         {
            strPassword = eapSettings.privateKeyPassword();
            if (strPassword.isEmpty())
            {
               if (eapSettings.privateKeyPath().startsWith(OpenSSLSettings().engineId()))
                  strPassword = promptPwd(m_Application.applicationName(), QObject::tr("Please enter your PIN:"));
               else
                  strPassword = promptPwd(m_Application.applicationName(), QObject::tr("Please enter your passphrase:"));
            }
         }
         else if (pppSettings.userName() ==m_Application.argv()[1])
         {
            strPassword = pppSettings.password();
            if (strPassword.isEmpty())
               strPassword = promptPwd(m_Application.applicationName(), QObject::tr("Please enter your password:"));
         }
      }
   }
   else
      strPassword = promptPwd(m_Application.applicationName(), QObject::tr("Please enter your passphrase:"));

   if (!strPassword.isNull())
   {
      const int iPwdLength = strPassword.length();
      const int iPwdFileDescriptor = ::atoi(m_Application.argv()[3]);
      if (iPwdFileDescriptor >= 0)
      {
         const int iWritten = ::write(iPwdFileDescriptor, strPassword.toAscii().constData(), iPwdLength);
         if (iWritten == iPwdLength)
            iRet = 0;
      }
   }

   return(iRet);
}

QString PasswordCallback::promptPwd(const QString& strTitle, const QString& strLabel)
{
   bool fOk;

   const QString strPassword = QInputDialog::getText(NULL, strTitle, strLabel, QLineEdit::Password, "", &fOk);

   return(fOk ? strPassword : QString());
}


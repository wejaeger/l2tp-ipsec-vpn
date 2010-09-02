/*
 * $Id$
 *
 * File:   PasswordCallback.cpp
 * Author: Werner Jaeger
 *
 * Created on September 1, 2010, 4:20 PM
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


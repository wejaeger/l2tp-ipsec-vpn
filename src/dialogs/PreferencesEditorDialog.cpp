/*
 * File:   PreferencesEditorDialog.cpp
 * Author: wejaeger
 *
 * Created on February 4, 2011, 11:21 AM
  *
 * Copyright 2011 Werner Jaeger.
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

#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>

#include "pkcs11/Pkcs11.h"
#include "settings/Preferences.h"
#include "util/ErrorEx.h"
#include "PreferencesEditorDialog.h"

PreferencesEditorDialog::PreferencesEditorDialog(QWidget* pParent) : QDialog(pParent)
{
   m_Widget.setupUi(this);

   connect(m_Widget.m_pEnginePathPushButton, SIGNAL(clicked()), this, SLOT(onEnginePath()));
   connect(m_Widget.m_pPkcs11PathPushButton, SIGNAL(clicked()), this, SLOT(onPkcs11Path()));

   readSettings();
}

PreferencesEditorDialog::~PreferencesEditorDialog()
{
}

void PreferencesEditorDialog::onEnginePath()
{
   const OpenSSLSettings settings(Preferences().openSSLSettings());

   const QString strEnginePath(QFileDialog::getOpenFileName(this, tr("Choose path of engine library ..."), settings.enginePath(), tr("library files (*.so)")));

   if (!strEnginePath.isNull())
      m_Widget.m_pEnginePathLineEdit->setText(strEnginePath);
}

void PreferencesEditorDialog::onPkcs11Path()
{
   const OpenSSLSettings settings(Preferences().openSSLSettings());

   const QString strPkcs11Path(QFileDialog::getOpenFileName(this, tr("Choose path of PKCS11 library ..."), settings.pkcs11Path(), tr("library files (*.so)")));

   if (!strPkcs11Path.isNull())
      m_Widget.m_pPkcs11PathLineEdit->setText(strPkcs11Path);
}

void PreferencesEditorDialog::accept()
{
   const QString strPkcs11Lib(m_Widget.m_pPkcs11PathLineEdit->text());
   const QString strCurrentPkcs11Lib(Preferences().openSSLSettings().pkcs11Path());

   try
   {
      if (strPkcs11Lib != strCurrentPkcs11Lib)
         Pkcs11::loadLibrary(strPkcs11Lib, false);

      writeSettings();
      QDialog::accept();
   }
   catch (ErrorEx error)
   {
      QMessageBox::critical(NULL, QCoreApplication::applicationName(), error.getString());

      if (!Pkcs11::loaded())
      {
         if (!Pkcs11::loadLibrary(strCurrentPkcs11Lib, true))
            QMessageBox::critical(NULL, QCoreApplication::applicationName(), QObject::tr("I couldn't load PKCS11 library %1.").arg(strCurrentPkcs11Lib));
      }
  }
}

void PreferencesEditorDialog::readSettings() const
{
   const OpenSSLSettings settings(Preferences().openSSLSettings());

   m_Widget.m_pEngineIDLineEdit->setText(settings.engineId());
   m_Widget.m_pEnginePathLineEdit->setText(settings.enginePath());
   m_Widget.m_pPkcs11PathLineEdit->setText(settings.pkcs11Path());
}

bool PreferencesEditorDialog::writeSettings() const
{
   const OpenSSLSettings settings(Preferences().openSSLSettings());

   bool fRet(settings.setEngineId(m_Widget.m_pEngineIDLineEdit->text()));
   if (fRet) fRet = settings.setEnginePath(m_Widget.m_pEnginePathLineEdit->text());
   if (fRet) fRet = settings.setPkcs11Path(m_Widget.m_pPkcs11PathLineEdit->text());

   return(fRet);
}

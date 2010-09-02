/*
 * $Id$
 *
 * File:   PeerAuthenticationDialog.cpp
 * Author: Werner Jaeger
 *
 * Created on August 5, 2010, 11:19 AM
 */

#include "settings/ConnectionSettings.h"
#include "PeerAuthenticationDialog.h"

PeerAuthenticationDialog::PeerAuthenticationDialog(const QString& strConnectionName, QWidget* pParent) : QDialog(pParent), m_strConnectionName(strConnectionName)
{
   m_Widget.setupUi(this);

   setWindowTitle(strConnectionName + tr(" - Peer Authentication Settings"));

   readSettings();
}

PeerAuthenticationDialog::~PeerAuthenticationDialog()
{
}

void PeerAuthenticationDialog::readSettings() const
{
   const ConnectionSettings settings;
   const PppSettings pppSettings = settings.pppSettings(m_strConnectionName);

   m_Widget.m_pRemoteNameEdit->setText(pppSettings.remoteName());
}

bool PeerAuthenticationDialog::writeSettings() const
{
   const ConnectionSettings settings;
   const PppSettings pppSettings = settings.pppSettings(m_strConnectionName);

   bool fRet = pppSettings.setRemoteName(m_Widget.m_pRemoteNameEdit->text());

   return(fRet);
}

void PeerAuthenticationDialog::accept()
{
   writeSettings();
   QDialog::accept();
}


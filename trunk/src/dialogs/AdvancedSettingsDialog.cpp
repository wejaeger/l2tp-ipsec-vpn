/*
 * $Id$
 *
 * File:   AdvancedSettingsDialog.cpp
 * Author: wejaeger
 *
 * Created on September 18, 2010, 4:21 PM
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

#include "AdvancedSettingsDialog.h"

AdvancedSettingsDialog::AdvancedSettingsDialog(const QString& strConnectionName, QWidget* pParent) : QDialog(pParent), m_strConnectionName(strConnectionName)
{
   m_Widget.setupUi(this);

   setWindowTitle(strConnectionName + tr(" - Advanced Settings"));

   readSettings();
}

AdvancedSettingsDialog::~AdvancedSettingsDialog()
{
}

void AdvancedSettingsDialog::accept()
{
   writeSettings();
   QDialog::accept();
}

void AdvancedSettingsDialog::readSettings() const
{

}

bool AdvancedSettingsDialog::writeSettings() const
{
   bool fRet = false;

   return(fRet);
}

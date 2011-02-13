/*
 * $Id$
 *
 * File:   ConnectionInformationDialog.cpp
 * Author: Werner Jaeger
 *
 * Created on July 18, 2010, 3:37 PM
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

#include "ConnectionInformationDialog.h"

ConnectionInformationDialog::ConnectionInformationDialog(QWidget* pParent) : QDialog(pParent)
{
   m_Widget.setupUi(this);
}

ConnectionInformationDialog::~ConnectionInformationDialog()
{
}

void ConnectionInformationDialog::appendLogColorText(const QColor&, const char* pcText)
{
   const QColor currentColor = m_Widget.m_pTextEdit->textColor();

   if (pcText)
   {
      m_Widget.m_pTextEdit->setTextColor(QColor(255, 0, 0));
      m_Widget.m_pTextEdit->insertPlainText(QString(pcText));
      m_Widget.m_pTextEdit->setTextColor(currentColor);
   }
}


/*
 * $Id$
 *
 * File:   ConnectionEditor.cpp
 * Author: Werner Jaeger
 *
 * Created on August 30, 2010, 12:59 PM
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

#include "dialogs/ConnectionEditorDialog.h"
#include "L2tpIPsecVpnApplication.h"
#include "ConnectionEditor.h"

ConnectionEditor::ConnectionEditor(L2tpIPsecVpnApplication& application, QObject* pParent) : QObject(pParent), m_Application(application), m_pConnectionEditorDialog(new ConnectionEditorDialog)
{
   connect(m_pConnectionEditorDialog, SIGNAL(connectionAdded(const QString&)), this, SLOT(onConnectionAdded(const QString&)));
   connect(m_pConnectionEditorDialog, SIGNAL(connectionRemoved(const QString&)), this, SLOT(onConnectionRemoved(const QString&)));
}

ConnectionEditor::~ConnectionEditor()
{
   delete m_pConnectionEditorDialog;
}

int ConnectionEditor::exec()
{
   return(m_pConnectionEditorDialog->exec() == QDialog::Accepted ? 0 : 1);
}

void ConnectionEditor::onConnectionAdded(const QString& strName)
{
   m_Application.sendConnectionAddedMessage(strName);
}

void ConnectionEditor::onConnectionRemoved(const QString& strName)
{
   m_Application.sendConnectionRemovedMessage(strName);
}

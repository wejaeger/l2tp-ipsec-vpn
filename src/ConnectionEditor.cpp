/*
 * $Id$
 *
 * File:   ConnectionEditor.cpp
 * Author: Werner Jaeger
 *
 * Created on August 30, 2010, 12:59 PM
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

/*
 * $Id$
 *
 * File:   ConnectionEditorDialog.cpp
 * Author: Werner Jaeger
 *
 * Created on July 18, 2010, 1:09 PM
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

#include <QInputDialog>
#include <QMessageBox>

#include "models/ConnectionsModel.h"
#include "settings/ConnectionSettings.h"
#include "util/SmartCardState.h"
#include "conf/ConfWriter.h"
#include "ConnectionSettingsDialog.h"
#include "ConnectionEditorDialog.h"

ConnectionEditorDialog::ConnectionEditorDialog(QWidget* pParent) : QDialog(pParent), m_pConnectionsModel(new ConnectionsModel())
{
   m_Widget.setupUi(this);

   m_Widget.m_pConnections->setModel(m_pConnectionsModel);
   m_Widget.m_pConnections->horizontalHeader()->setStretchLastSection(true);
   m_Widget.m_pConnections->setSelectionBehavior(QAbstractItemView::SelectRows);

   connect(m_Widget.m_pAdd, SIGNAL(clicked()), this, SLOT(addConnection()));
   connect(m_Widget.m_pEdit, SIGNAL(clicked()), this, SLOT(editConnection()));
   connect(m_Widget.m_pDelete, SIGNAL(clicked()), this, SLOT(removeConnection()));
   connect(m_Widget.m_pApplyButton, SIGNAL(clicked()), this, SLOT(applySettings()));

   if (m_pConnectionsModel->rowCount() > 0)
      m_Widget.m_pConnections->setCurrentIndex(m_pConnectionsModel->index(0, 0));
   else
      enableDeleteAndEdit(false);

   m_Widget.m_pConnections->setFocus();
}

ConnectionEditorDialog::~ConnectionEditorDialog()
{
   delete m_pConnectionsModel;
}

bool ConnectionEditorDialog::applySettings(bool fInteractive) const
{
   const ConnectionSettings settings;
   const int iConnections = settings.connections();

    bool fRet(true);

   if (iConnections > 0)
   {
      if (m_pConnectionsModel->isWriteable())
      {
         if (!fInteractive || QMessageBox::question(NULL, tr("Confirm applying changes"), tr("You need to reconnect for your changes to take effect!\n\nAre you sure you want to apply your changes?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
         {
            fRet = ConfWriter::write(ConfWriter::IPsec);
            if (fRet) fRet = ConfWriter::write(ConfWriter::IPsecSECRET);
            if (fRet) fRet = ConfWriter::write(ConfWriter::L2TP);

            for (int i = 0; fRet && i < iConnections; i++)
            {
               const QString strConnectionName(settings.connection(i));

               fRet = ConfWriter::write(ConfWriter::PPP, strConnectionName);
               if (fRet) fRet = ConfWriter::write(ConfWriter::PPPDNSCONF, QCoreApplication::instance()->objectName() + "-" +strConnectionName);
            }

            if (fRet) fRet = ConfWriter::write(ConfWriter::PPPUPSCRIPT);
            if (fRet) fRet = ConfWriter::write(ConfWriter::PPPDOWNSCRIPT);
            if (fRet) fRet = ConfWriter::write(ConfWriter::GETIPSECINFO);
            if (fRet) fRet = ConfWriter::write(ConfWriter::OPENSSL);
            if (fRet) fRet = ConfWriter::write(ConfWriter::RSYSLOG);
         }
      }
      else if (fInteractive)
         QMessageBox::critical(NULL, tr("Apply settings"), tr("You do not have the permission to apply settings"));
   }

   if (fInteractive)
      m_Widget.m_pConnections->setFocus();

    return(fRet);
}

void ConnectionEditorDialog::addConnection()
{
   if (m_pConnectionsModel->isWriteable())
   {
      const QString strTitle(tr("New Connection"));

      QString strName("");
      bool fOk = true;
      ConnectionsModel::Result result(ConnectionsModel::InvalidName);
      while (fOk && result != ConnectionsModel::Ok)
      {
         strName = QInputDialog::getText(this, strTitle, tr("Connection name:"), QLineEdit::Normal, strName, &fOk);

         if (fOk)
         {
            const int iRow = m_pConnectionsModel->rowCount();
            result = m_pConnectionsModel->addRow(strName);
            switch (result)
            {
               case ConnectionsModel::Ok:
                  m_Widget.m_pConnections->setCurrentIndex(m_pConnectionsModel->index(iRow, 0));
                  enableDeleteAndEdit(true);
                  emit connectionAdded(strName);
                  break;

               case ConnectionsModel::DuplicateName:
                  QMessageBox::critical(this, strTitle, tr("A connection with name '%1' already exists").arg(strName));
                  break;

               case ConnectionsModel::InvalidName:
                  QMessageBox::critical(this, strTitle, tr("The provided name '%1' contains invalid characters.\n\nName must start with a letter and may contain only letters, digits, periods, underscores, and hyphens.\nMaximum length is 30.").arg(strName));
                  break;

               default:
                  break;
            }
         }
      }
   }
   else
      QMessageBox::critical(this, tr("New Connection"), tr("You do not have the permission to add a connection"));

   m_Widget.m_pConnections->setFocus();
}

void ConnectionEditorDialog::editConnection()
{
   if (m_pConnectionsModel->isWriteable())
   {
      const QModelIndex index = m_Widget.m_pConnections->currentIndex();

      if (index.row() >= 0)
      {
         const QString strName = m_pConnectionsModel->data(index, Qt::DisplayRole).toString();

         ConnectionSettingsDialog settings(strName);
         settings.exec();
      }
   }
   else
      QMessageBox::critical(this, tr("Edit Connection"), tr("You do not have the permission to edit a connection"));

   m_Widget.m_pConnections->setFocus();
}

void ConnectionEditorDialog::removeConnection()
{
   if (m_pConnectionsModel->isWriteable())
   {
      const QModelIndex index = m_Widget.m_pConnections->currentIndex();

      if (index.row() >= 0)
      {
         const QString strName = m_pConnectionsModel->data(index, Qt::DisplayRole).toString();

         if (QMessageBox::question(this, tr("Delete Connection"), tr("Are you sure you wish to delete the connection '%1'?").arg(strName), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
         {
            if (m_pConnectionsModel->removeRow(index.row()))
            {
               const int iRows =  m_pConnectionsModel->rowCount();
               if (index.row() < iRows)
                  m_Widget.m_pConnections->setCurrentIndex(m_pConnectionsModel->index(index.row(), 0));
               else if (iRows > 0)
                  m_Widget.m_pConnections->setCurrentIndex(m_pConnectionsModel->index(index.row() - 1, 0));

               enableDeleteAndEdit(iRows > 0);
               emit connectionRemoved(strName);
            }
         }
      }
   }
   else
      QMessageBox::critical(this, tr("Delete Connection"), tr("You do not have the permission to remmove a connection"));

   m_Widget.m_pConnections->setFocus();
}

void ConnectionEditorDialog::enableDeleteAndEdit(bool fEnable)
{
   m_Widget.m_pDelete->setEnabled(fEnable);
   m_Widget.m_pEdit->setEnabled(fEnable);
}

/*
 * $Id$
 *
 * File:   ConnectionEditorDialog.h
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

#ifndef _CONNECTIONEDITORDIALOG_H
#define	_CONNECTIONEDITORDIALOG_H

#include "ui_ConnectionEditorDialog.h"

class ConnectionsModel;
class ConnectionSettings;

class ConnectionEditorDialog : public QDialog
{
   Q_OBJECT

public:
   explicit ConnectionEditorDialog(QWidget* pParent = 0);
   virtual ~ConnectionEditorDialog();

   bool applySettings(bool fInteractive = true) const;

signals:
   void connectionAdded(const QString& strName);
   void connectionRemoved(const QString& strName);

private slots:
   void accept();
   void reject();
   void editPreferences();
   void addConnection();
   void editConnection();
   void removeConnection();

private:
   ConnectionEditorDialog(const ConnectionEditorDialog& orig);
   ConnectionEditorDialog& operator=(const ConnectionEditorDialog& orig);

   void enableDeleteAndEdit(bool fEnable);

   Ui::ConnectionEditorDialog m_Widget;

   ConnectionsModel* const m_pConnectionsModel;
   ConnectionSettings* const m_pConnectionSettings;
};

#endif	/* _CONNECTIONEDITORDIALOG_H */

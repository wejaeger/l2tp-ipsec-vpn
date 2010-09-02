/*
 * $Id$
 *
 * File:   ConnectionEditorDialog.h
 * Author: Werner Jaeger
 *
 * Created on July 18, 2010, 1:09 PM
 */

#ifndef _CONNECTIONEDITORDIALOG_H
#define	_CONNECTIONEDITORDIALOG_H

#include "ui_ConnectionEditorDialog.h"

class ConnectionsModel;

class ConnectionEditorDialog : public QDialog
{
   Q_OBJECT

public:
   explicit ConnectionEditorDialog(QWidget* pParent = 0);
   virtual ~ConnectionEditorDialog();

signals:
   void connectionAdded(const QString& strName);
   void connectionRemoved(const QString& strName);

private slots:
   void addConnection();
   void editConnection();
   void removeConnection();
   void applySettings() const;

private:
   ConnectionEditorDialog(const ConnectionEditorDialog& orig);
   ConnectionEditorDialog& operator=(const ConnectionEditorDialog& orig);

   void enableDeleteAndEdit(bool fEnable);

   Ui::ConnectionEditorDialog m_Widget;

   ConnectionsModel* const m_pConnectionsModel;
};

#endif	/* _CONNECTIONEDITORDIALOG_H */

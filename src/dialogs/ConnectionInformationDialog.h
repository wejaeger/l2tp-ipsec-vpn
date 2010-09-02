/*
 * $Id$
 *
 * File:   ConnectionInformationDialog.h
 * Author: Werner Jaeger
 *
 * Created on July 18, 2010, 3:09 PM
 */

#ifndef CONNECTIONINFORMATIONDIALOG_H
#define	CONNECTIONINFORMATIONDIALOG_H

#include "ui_ConnectionInformationDialog.h"

class ConnectionInformationDialog : public QDialog
{
   Q_OBJECT

public:
   explicit ConnectionInformationDialog(QWidget* pParent = 0);
   virtual ~ConnectionInformationDialog();

   void clearLog() { m_Widget.m_pTextEdit->clear(); }
   void appendLogPlainText(const char* pcText) { m_Widget.m_pTextEdit->insertPlainText(QString(pcText)); }
   void appendLogColorText(const QColor&, const char* pcText);

private:
   ConnectionInformationDialog(const ConnectionInformationDialog& orig);
   ConnectionInformationDialog& operator=(const ConnectionInformationDialog& orig);

   Ui::ConnectionInformationDialog m_Widget;
};

#endif	/* CONNECTIONINFORMATIONDIALOG_H */


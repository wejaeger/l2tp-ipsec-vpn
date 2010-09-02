/*
 * $Id$
 *
 * File:   PeerAuthenticationDialog.h
 * Author: Werner Jaeger
 *
 * Created on August 5, 2010, 11:19 AM
 */

#ifndef _PEERAUTHENTICATIONDIALOG_H
#define	_PEERAUTHENTICATIONDIALOG_H

#include "ui_PeerAuthenticationDialog.h"

class PeerAuthenticationDialog : public QDialog
{
   Q_OBJECT

public:
   explicit PeerAuthenticationDialog(const QString& strConnectionName, QWidget* pParent = 0);
   virtual ~PeerAuthenticationDialog();

private slots:
   void readSettings() const;
   bool writeSettings() const;
   void accept();

private:
   PeerAuthenticationDialog(const PeerAuthenticationDialog& orig);
   PeerAuthenticationDialog& operator=(const PeerAuthenticationDialog& orig);

   Ui::PeerAuthenticationDialog m_Widget;

   const QString m_strConnectionName;
};

#endif	/* _PEERAUTHENTICATIONDIALOG_H */

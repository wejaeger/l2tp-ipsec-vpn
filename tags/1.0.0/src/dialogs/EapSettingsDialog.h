/*
 * $Id$
 *
 * File:   EapSettingsDialog.h
 * Author: Werner Jaeger
 *
 * Created on July 29, 2010, 2:48 PM
 */

#ifndef _EAPSETTINGSDIALOG_H
#define	_EAPSETTINGSDIALOG_H

#include "ui_EapSettingsDialog.h"

class EapSettingsDialog : public QDialog
{
   Q_OBJECT

public:
   explicit EapSettingsDialog(const QString& strConnectionName, QWidget* pParent = 0);
   virtual ~EapSettingsDialog();

private slots:
   void onUseCertificateRadioButtonToggled(bool fChecked);
   void onCertificate();
   void onPrivateKey();
   void onCaCertificate();
   void readSettings();
   bool writeSettings() const;
   void accept();

private:
   EapSettingsDialog(const EapSettingsDialog& orig);
   EapSettingsDialog& operator=(const EapSettingsDialog& orig);

   Ui::EapSettingsDialog m_Widget;

   const QString m_strConnectionName;
   QString m_strUserName;
};

#endif	/* _EAPSETTINGSDIALOG_H */

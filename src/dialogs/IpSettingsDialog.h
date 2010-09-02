/*
 * $Id$
 *
 * File:   IpSettingsDialog.h
 * Author: Werner Jaeger
 *
 * Created on August 20, 2010, 9:59 AM
 */

#ifndef _IPSETTINGSDIALOG_H
#define	_IPSETTINGSDIALOG_H

#include "ui_IpSettingsDialog.h"

class IpSettingsDialog : public QDialog
{
   Q_OBJECT

public:
   IpSettingsDialog(const QString& strConnectionName, QWidget* pParent = 0);
   virtual ~IpSettingsDialog();

private slots:
   void onRouteSettings() const;
   void accept();

private:
   IpSettingsDialog(const IpSettingsDialog& orig);
   IpSettingsDialog& operator=(const IpSettingsDialog& orig);

   void readSettings() const;
   bool writeSettings() const;

   Ui::IpSettingsDialog m_Widget;

   const QString m_strConnectionName;
};

#endif	/* _IPSECSETTINGSDIALOG_H */

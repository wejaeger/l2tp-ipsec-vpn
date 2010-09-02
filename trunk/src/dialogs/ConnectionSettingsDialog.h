/*
 * $Id$
 *
 * File:   ConnectionSettingsDialog.h
 * Author: Werner Jaeger
 *
 * Created on July 21, 2010, 5:27 PM
 */

#ifndef _CONNECTIONSETTINGSDIALOG_H
#define	_CONNECTIONSETTINGSDIALOG_H

#include "ui_ConnectionSettingsDialog.h"

class CommonSettings;
class IPSecSettings;
class L2tpSettings;
class PppSettings;
class IPsecCertificateListModel;

class ConnectionSettingsDialog : public QDialog
{
   Q_OBJECT

public:
   ConnectionSettingsDialog(const QString& strConnectionName, QWidget* pParent = 0);
   virtual ~ConnectionSettingsDialog();

private slots:
   void accept();
   void readSettings() const;
   bool writeSettings() const;
   void onEapProperties() const;
   void onPeerAuthentication() const;
   void onIpSettings() const;
   void onUseIPsecCertificateRadioButtonToggled(bool fEnable);
   void onUseEapRadioButtonToggled(bool fEnabled);
   void onPrivateKey();

private:
   ConnectionSettingsDialog(const ConnectionSettingsDialog& orig);
   ConnectionSettingsDialog& operator=(const ConnectionSettingsDialog& orig);

   void readCommonSettings(const CommonSettings& commonSettings) const;
   void readIPSecSettings(const IPSecSettings& ipsecSettings) const;
   void readL2tpSettings(const L2tpSettings& l2tpSettings) const;
   void readPppSettings(const PppSettings& pppSettings) const;
   bool writeCommonSetting(const CommonSettings& commonSettings) const;
   bool writeIPsecSetting(const IPSecSettings& ipsecSettings) const;
   bool writeL2tpSettings(const L2tpSettings& l2tpSettings) const;
   bool writePppSettings(const PppSettings& pppSettings) const;

   Ui::ConnectionSettingsDialog m_Widget;

   const QString m_strConnectionName;

   IPsecCertificateListModel* const m_pCertificateListModel;
};

#endif	/* _CONNECTIONSETTINGSDIALOG_H */

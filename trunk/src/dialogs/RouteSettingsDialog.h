/*
 * $Id$
 *
 * File:   RouteSettingsDialog.h
 * Author: Werner Jaeger
 *
 * Created on August 5, 2010, 4:17 PM
 */

#ifndef _ROUTESETTINGSDIALOG_H
#define	_ROUTESETTINGSDIALOG_H

#include "ui_RouteSettingsDialog.h"

class PppRoutesModel;

class RouteSettingsDialog : public QDialog
{
   Q_OBJECT

public:
   explicit RouteSettingsDialog(const QString& strConnectionName, QWidget* pParent = 0);
   virtual ~RouteSettingsDialog();

private slots:
   void addRoute() const;
   void removeRoute() const;
   void accept();

private:
   RouteSettingsDialog(const RouteSettingsDialog& orig);
   RouteSettingsDialog& operator=(const RouteSettingsDialog& orig);

   void readSettings() const;
   bool writeSettings() const;

   Ui::RouteSettingsDialog m_Widget;

   const QString m_strConnectionName;
   PppRoutesModel* m_pRoutesModel;
};

#endif	/* _ROUTESETTINGSDIALOG_H */

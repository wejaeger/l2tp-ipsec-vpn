/*
 * $Id$
 *
 * File:   Settings.h
 * Author: Werner Jaeger
 *
 * Created on August 9, 2010, 11:36 AM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#include <QString>

class QSettings;

class Settings
{
protected:
   Settings();
   virtual ~Settings();

   static bool removeArrayItem(QSettings* pSettings, const QString& strArrayName, int iIndex);

private:
   Settings(const Settings& orig);
   Settings& operator=(const Settings& orig);
};

#endif	/* SETTINGS_H */


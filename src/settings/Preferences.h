/*
 * $Id$
 *
 * File:   Preferences.h
 * Author: Werner Jaeger
 *
 * Created on August 9, 2010, 12:12 PM
 */

#ifndef PREFERENCES_H
#define	PREFERENCES_H

#include "Settings.h"

class OpenSSLSettings
{
public:
    OpenSSLSettings() {}
   virtual ~OpenSSLSettings() {}

   bool setEnginePath(QString& strEnginePath) const;
   QString enginePath() const;
   bool setModulePath(const QString& strModulePath) const;
   QString modulePath() const;
   bool setEngineId(QString& strEngineId) const;
   QString engineId() const;
};

class Preferences : public Settings
{
public:
   Preferences();
   virtual ~Preferences();

   OpenSSLSettings openSSLSettings() const;

private:
   Preferences(const Preferences& orig);
   Preferences& operator=(const Preferences& orig);
};

#endif	/* PREFERENCES_H */


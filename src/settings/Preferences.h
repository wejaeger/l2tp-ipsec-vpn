/*
 * $Id$
 *
 * File:   Preferences.h
 * Author: Werner Jaeger
 *
 * Created on August 9, 2010, 12:12 PM
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


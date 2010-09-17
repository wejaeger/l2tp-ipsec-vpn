/*
 * $Id$
 *
 * File:   Preferences.cpp
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

#include <QSettings>

#include "Preferences.h"

#if defined(_WIN32) || defined(USE_CYGWIN)
static const char* const EngineDefaultLibrary = "engine_pkcs11.dll";
static const char* const PKCS11DefaultLibrary = "opensc-pkcs11.dll";
#else
static const char* const EngineDefaultLibrary = "/usr/lib/engines/engine_pkcs11.so";
static const char* const PKCS11DefaultLibrary = "/usr/lib/opensc-pkcs11.so";
#endif

static const char* const DefaultEngineId = "pkcs11";

static const QString OPENSSL = "OpenSSL";

static const QString ENGINEPATH = OPENSSL + '/' + "enginePath";
static const QString MODULEPATH = OPENSSL + '/' + "modulePath";
static const QString ENGINEID = OPENSSL + '/' + "engineId";

Preferences::Preferences()
{
}

Preferences::~Preferences()
{
}

OpenSSLSettings Preferences::openSSLSettings() const
{
   return(OpenSSLSettings());
}

bool OpenSSLSettings::setEnginePath(QString& strEnginePath) const
{
   QSettings().setValue(ENGINEPATH, strEnginePath);

   return(true);
}

QString OpenSSLSettings::enginePath() const
{
   return(QSettings().value(ENGINEPATH, EngineDefaultLibrary).toString());
}

bool OpenSSLSettings::setModulePath(const QString& strModulePath) const
{
   QSettings().setValue(MODULEPATH, strModulePath);

   return(true);
}

QString OpenSSLSettings::modulePath() const
{
   return(QSettings().value(MODULEPATH, PKCS11DefaultLibrary).toString());
}

bool OpenSSLSettings::setEngineId(QString& strEngineId) const
{
   QSettings().setValue(ENGINEID, strEngineId);

   return(true);
}

QString OpenSSLSettings::engineId() const
{
   return(QSettings().value(ENGINEID, DefaultEngineId).toString());
}

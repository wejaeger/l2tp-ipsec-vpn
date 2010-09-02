/*
 * $Id$
 *
 * File:   OpensslConfWriter.cpp
 * Author: Werner Jaeger
 *
 * Created on August 7, 2010, 4:52 PM
 */

#include <ctemplate/template.h>
#include "settings/Preferences.h"
#include "OpensslConfWriter.h"

static const char* const ENGINEID = "ENGINEID";
static const char* const DYNAMICPATH = "DYNAMICPATH";
static const char* const MODULEPATH = "MODULEPATH";

OpensslConfWriter::OpensslConfWriter(const QString& strTemplateKey, const QString& strWriteTo) : AbstractConfWriter(strTemplateKey, strWriteTo)
{
}

void OpensslConfWriter::fill()
{
   const OpenSSLSettings opensslSettings;

   dictionary()->SetValue(ENGINEID, opensslSettings.engineId().toAscii().constData());
   dictionary()->SetValue(DYNAMICPATH, opensslSettings.enginePath().toAscii().constData());
   dictionary()->SetValue(MODULEPATH, opensslSettings.modulePath().toAscii().constData());
}
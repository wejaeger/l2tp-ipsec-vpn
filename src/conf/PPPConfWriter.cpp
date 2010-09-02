/*
 * $Id$
 *
 * File:   PPPConfWriter.cpp
 * Author: Werner Jaeger
 *
 * Created on July 27, 2010, 8:50 AM
 */

#include <QCoreApplication>

#include "settings/ConnectionSettings.h"
#include "PPPConfWriter.h"

static const char* const REFUSE_SECTION = "PEFUSE_SECTION";

static const char* const REFUSEEAPLINE = "refuse-eap";
static const char* const REFUSEPAPLINE = "refuse-pap";
static const char* const REFUSECHAPLINE = "refuse-chap";
static const char* const REFUSEMSCHAPLINE = "refuse-mschap";
static const char* const REFUSEMSCHAPV2LINE = "refuse-mschap-v2";
static const char* const USEPEERDNSLINE = "usepeerdns";


static const char* const IPPARAM = "IPPARAM";
static const char* const REMOTENAME = "REMOTENAME";
static const char* const NAME = "NAME";
static const char* const USEPEERDNS = "USEPEERDNS";
static const char* const REFUSEPROTOCOL = "REFUSEPROTOCOL";
static const char* const CERT = "CERT";
static const char* const CA = "CA";
static const char* const KEY = "KEY";

PPPConfWriter::PPPConfWriter(const QString& strTemplateKey, const QString& strWriteTo) : AbstractConfWriter(strTemplateKey, strWriteTo)
{
}

PPPConfWriter::~PPPConfWriter()
{
}

void PPPConfWriter::fill()
{
   dictionary()->SetValue(IPPARAM, (QCoreApplication::instance()->objectName() + "-" + instance()).toAscii().constData());

   const PppSettings pppSettings = ConnectionSettings().pppSettings(instance());

   dictionary()->SetValue(REMOTENAME, pppSettings.remoteName().toAscii().constData());
   dictionary()->SetValue(NAME, pppSettings.userName().toAscii().constData());

   if (pppSettings.ipSettings().usePeerDns())
      dictionary()->SetValue(USEPEERDNS, USEPEERDNSLINE);

   const bool fRefuseEap = pppSettings.refuseEap();
   if (fRefuseEap) addRefuseEntry(REFUSEEAPLINE);
   if (!fRefuseEap || pppSettings.refusePap()) addRefuseEntry(REFUSEPAPLINE);
   if (!fRefuseEap || pppSettings.refuseChap()) addRefuseEntry(REFUSECHAPLINE);
   if (!fRefuseEap || pppSettings.refuseMsChap()) addRefuseEntry(REFUSEMSCHAPLINE);
   if (!fRefuseEap || pppSettings.refuseMsChapV2()) addRefuseEntry(REFUSEMSCHAPV2LINE);

   const PppEapSettings eapSettings = pppSettings.eapSettings();
   dictionary()->SetValue(CERT, eapSettings.certificatePath().toAscii().constData());
   dictionary()->SetValue(CA, eapSettings.caCertificatePath().toAscii().constData());
   dictionary()->SetValue(KEY, eapSettings.privateKeyPath().toAscii().constData());
}

void PPPConfWriter::addRefuseEntry(const QString& strRefuse) const
{
   dictionary()->AddSectionDictionary(REFUSE_SECTION)->SetValue(REFUSEPROTOCOL, strRefuse.toAscii().constData());
}

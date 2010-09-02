/*
 * $Id$
 *
 * File:   PppDownScriptWriter.h
 * Author: Werner Jaeger
 *
 * Created on August 6, 2010, 1:28 PM
 */

#ifndef PPPDOWNSCRIPTWRITER_H
#define	PPPDOWNSCRIPTWRITER_H

#include "AbstractConfWriter.h"

class PppDownScriptWriter : public AbstractConfWriter
{
public:
   PppDownScriptWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~PppDownScriptWriter() {}

protected:
   void fill();

private:
   PppDownScriptWriter(const PppDownScriptWriter& orig);
   PppDownScriptWriter& operator=(const PppDownScriptWriter& orig);
};

#endif	/* PPPDOWNSCRIPTWRITER_H */


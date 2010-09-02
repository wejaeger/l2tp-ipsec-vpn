/*
 * $Id$
 *
 * File:   PppUpScriptWriter.h
 * Author: Werner Jaeger
 *
 * Created on August 6, 2010, 1:18 PM
 */

#ifndef PPPUPSCRIPTWRITER_H
#define	PPPUPSCRIPTWRITER_H

#include "AbstractConfWriter.h"

class PppUpScriptWriter : public AbstractConfWriter
{
public:
   PppUpScriptWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~PppUpScriptWriter() {}

protected:
   void fill();

private:
   PppUpScriptWriter(const PppUpScriptWriter& orig);
   PppUpScriptWriter& operator=(const PppUpScriptWriter& orig);
};

#endif	/* PPPUPSCRIPTWRITER_H */


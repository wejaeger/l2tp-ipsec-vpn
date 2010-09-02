/*
 * $Id$
 *
 * File:   PppDnsConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on August 21, 2010, 6:43 AM
 */

#ifndef PPPDNSCONFWRITER_H
#define	PPPDNSCONFWRITER_H

#include "AbstractConfWriter.h"

class PppDnsConfWriter : public AbstractConfWriter
{
public:
   PppDnsConfWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~PppDnsConfWriter() {}

protected:
   void fill();

private:
   PppDnsConfWriter(const PppDnsConfWriter& orig);
   PppDnsConfWriter& operator=(const PppDnsConfWriter& orig);
};

#endif	/* PPPDNSCONFWRITER_H */


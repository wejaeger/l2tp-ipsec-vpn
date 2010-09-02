/*
 * $Id$
 *
 * File:   RSysLogConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on July 24, 2010, 9:41 AM
 */

#ifndef RSYSLOGCONFWRITER_H
#define	RSYSLOGCONFWRITER_H

#include "AbstractConfWriter.h"

class RSysLogConfWriter : public AbstractConfWriter
{
public:
   RSysLogConfWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~RSysLogConfWriter() {}

protected:
   void fill() {}

private:
   RSysLogConfWriter(const RSysLogConfWriter& orig);
   RSysLogConfWriter& operator=(const RSysLogConfWriter& orig);
};

#endif	/* RSYSLOGCONFWRITER_H */


/*
 * $Id$
 *
 * File:   OpensslConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on August 7, 2010, 4:52 PM
 */

#ifndef OPENSSLCONFWRITER_H
#define	OPENSSLCONFWRITER_H

#include "AbstractConfWriter.h"

class OpensslConfWriter : public AbstractConfWriter
{
public:
   OpensslConfWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~OpensslConfWriter() {}

protected:
   void fill();

private:
   OpensslConfWriter(const OpensslConfWriter& orig);
   OpensslConfWriter& operator=(const OpensslConfWriter& orig);
};

#endif	/* OPENSSLCONFWRITER_H */


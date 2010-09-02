/*
 * $Id$
 *
 * File:   IPsecConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on July 23, 2010, 6:52 AM
 */

#ifndef IPSECCONFWRITER_H
#define	IPSECCONFWRITER_H

#include "AbstractConfWriter.h"

class IPsecConfWriter : public AbstractConfWriter
{
public:
   IPsecConfWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~IPsecConfWriter();

protected:
   void fill();

private:
   IPsecConfWriter(const IPsecConfWriter& orig);
   IPsecConfWriter& operator=(const IPsecConfWriter& orig);
};

#endif	/* IPSECCONFWRITER_H */


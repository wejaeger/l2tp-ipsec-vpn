/*
 * $Id$
 *
 * File:   IPsecSecretsConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on August 8, 2010, 6:15 PM
 */

#ifndef IPSECSECRETSCONFWRITER_H
#define	IPSECSECRETSCONFWRITER_H

#include "AbstractConfWriter.h"

class IPsecSecretsConfWriter : public AbstractConfWriter
{
public:
   IPsecSecretsConfWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~IPsecSecretsConfWriter();

protected:
   void fill();

private:
   IPsecSecretsConfWriter(const IPsecSecretsConfWriter& orig);
   IPsecSecretsConfWriter& operator=(const IPsecSecretsConfWriter& orig);
};

#endif	/* IPSECSECRETSCONFWRITER_H */


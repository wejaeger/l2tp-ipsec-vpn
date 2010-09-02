/*
 * $Id$
 *
 * File:   ConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on July 23, 2010, 9:14 AM
 */

#ifndef CONFWRITER_H
#define	CONFWRITER_H

class ConfWriter
{
public:
   typedef enum { IPsec, L2TP, PPP, RSYSLOG, GETIPSECINFO, PPPUPSCRIPT, PPPDNSCONF, PPPDOWNSCRIPT, OPENSSL, IPsecSECRET} Conf;

   static bool write(Conf conf, const QString& strInstance = "");
   static const QString& fileName(Conf conf, const QString& strInstance = "");

private:
   ConfWriter();
   virtual ~ConfWriter();
   ConfWriter(const ConfWriter& orig);
   ConfWriter& operator=(const ConfWriter& orig);
};

#endif	/* CONFWRITER_H */


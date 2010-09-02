/*
 * $Id$
 *
 * File:   GetIPsecInfoScriptWriter.h
 * Author: Werner Jaeger
 *
 * Created on August 6, 2010, 11:58 AM
 */

#ifndef GETIPSECINFOSCRIPTWRITER_H
#define	GETIPSECINFOSCRIPTWRITER_H

#include "AbstractConfWriter.h"

class GetIPsecInfoScriptWriter : public AbstractConfWriter
{
public:
   GetIPsecInfoScriptWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~GetIPsecInfoScriptWriter() {}

protected:
   void fill() {}

private:
   GetIPsecInfoScriptWriter(const GetIPsecInfoScriptWriter& orig);
   GetIPsecInfoScriptWriter& operator=(const GetIPsecInfoScriptWriter& orig);
};

#endif	/* GETIPSECINFOSCRIPTWRITER_H */


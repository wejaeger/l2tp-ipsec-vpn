/*
 * $Id$
 *
 * File:   PPPConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on July 27, 2010, 8:50 AM
 */

#ifndef PPPCONFWRITER_H
#define	PPPCONFWRITER_H

#include "AbstractConfWriter.h"

class PPPConfWriter : public AbstractConfWriter
{
public:
   PPPConfWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~PPPConfWriter();

protected:
   void fill();

private:
   PPPConfWriter(const PPPConfWriter& orig);
   PPPConfWriter& operator=(const PPPConfWriter& orig);

   void addRefuseEntry(const QString& strRefuse) const;
};

#endif	/* PPPCONFWRITER_H */


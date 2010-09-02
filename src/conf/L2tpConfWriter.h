/*
 * $Id$
 *
 * File:   L2tpConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on July 25, 2010, 5:37 PM
 */

#ifndef L2TPCONFWRITER_H
#define	L2TPCONFWRITER_H

#include "AbstractConfWriter.h"

class L2tpConfWriter : public AbstractConfWriter
{
public:
   L2tpConfWriter(const QString& strTemplateKey, const QString& strWriteTo);
   virtual ~L2tpConfWriter();

protected:
   void fill();

private:
   L2tpConfWriter(const L2tpConfWriter& orig);
   L2tpConfWriter& operator=(const L2tpConfWriter& orig);
};

#endif	/* L2TPCONFWRITER_H */


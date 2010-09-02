/*
 * $Id$
 *
 * File:   EncSecrets.h
 * Author: Werner Jaeger
 *
 * Created on July 28, 2010, 10:48 AM
 */

#ifndef ENCSECRETS_H
#define	ENCSECRETS_H

#include <QString>

class EncSecrets
{
public:
   EncSecrets(const unsigned char acKey[16], const unsigned char acIv[8], const char* pcAscbuf);
   EncSecrets(const char* pcB64buf);
   virtual ~EncSecrets();

   const char* getbuf(void) const;
   QString retrieve(const unsigned char acKey[16], const unsigned char acIv[8]);

private:
   EncSecrets(const EncSecrets& orig);
   EncSecrets& operator=(const EncSecrets& orig);

   int encode(const unsigned char* pcBfbuf, int iOutlen);
   int decode(unsigned char* pcBfbuf);
   static int encrypt(const char* pcAscbuf, const unsigned char acIv[8], const unsigned char acKey[16], unsigned char* pcBfbuf);
   static int decrypt(const unsigned char* pcBfbuf, int iTotal, char* pcAscbuf, const unsigned char acKey[16], const unsigned char acIv[8]);

   unsigned char* m_pcB64buf;
   int m_iBufferSize;
};

#endif	/* ENCSECRETS_H */


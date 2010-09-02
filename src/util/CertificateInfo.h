/*
 * $Id$
 *
 * File:   CertificateInfo.h
 * Author: Werner Jaeger
 *
 * Created on July 27, 2010, 6:09 PM
 */

#ifndef CERTIFICATEINFO_H
#define	CERTIFICATEINFO_H

struct x509_st;

class CertificateInfo
{
public:
   explicit CertificateInfo(const QString& strFilePath);
   explicit CertificateInfo(x509_st* pX509);
   virtual ~CertificateInfo();

   QString path() const;
   QString fileName() const;
   QString filePath() const;
   bool isReadable() const;
   QString name() const;
   QString cn() const;
   QString email() const;
   QString subjectAlternativeName() const;
   QString text() const;

private:
   CertificateInfo(const CertificateInfo& orig);
   CertificateInfo& operator=(const CertificateInfo& orig);

   QString subjectFieldByNid(int iNid) const;
   static x509_st* readCert(const QString& strFilePath);

   QString m_strFilePath;
   x509_st* const m_pX509;
};

#endif	/* CERTIFICATEINFO_H */


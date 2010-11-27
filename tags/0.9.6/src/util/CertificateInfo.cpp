/*
 * $Id$
 *
 * File:   CertificateInfo.cpp
 * Author: Werner Jaeger
 *
 * Created on July 27, 2010, 6:09 PM
 *
 * Copyright 2010 Werner Jaeger.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QString>
#include <QFileInfo>
#include <openssl/pem.h>
#include <openssl/x509v3.h>
#include "CertificateInfo.h"

CertificateInfo::CertificateInfo(const QString& strFilePath) : m_strFilePath(strFilePath), m_pX509(readCert(strFilePath))
{
}

CertificateInfo::CertificateInfo(X509* pX509) : m_strFilePath(""), m_pX509(pX509)
{
}

CertificateInfo::~CertificateInfo()
{
   if (m_pX509)
      ::X509_free(m_pX509);
}

QString CertificateInfo::path() const
{
   return(QFileInfo(m_strFilePath).path());
}

QString CertificateInfo::fileName() const
{
   return(QFileInfo(m_strFilePath).fileName());
}

QString CertificateInfo::filePath() const
{
   return(m_strFilePath);
}

bool CertificateInfo::isReadable() const
{
   return(m_pX509);
}

QString CertificateInfo::name() const
{
   QString strRet;

   if (isReadable())
      strRet = m_pX509->name;

   return(strRet);
}

QString CertificateInfo::cn() const
{
   return(subjectFieldByNid(NID_commonName));
}

QString CertificateInfo::email() const
{
   return(subjectFieldByNid(NID_pkcs9_emailAddress));
}

QString CertificateInfo::subjectAlternativeName() const
{
   QString strRet;

   if (isReadable())
   {
      GENERAL_NAMES* const psANs = static_cast<GENERAL_NAMES*>(::X509_get_ext_d2i(m_pX509, NID_subject_alt_name, NULL, NULL));
      if (psANs)
      {
         const int iNumAN = ::sk_GENERAL_NAME_num(psANs);
         for(int i = 0; i < iNumAN; ++i)
         {
            GENERAL_NAME* const psAN = sk_GENERAL_NAME_value(psANs, i);

            // we only care about DNS entries
            if (GEN_DNS == psAN->type)
            {
               unsigned char* pUtf;
               const int iLen = ::ASN1_STRING_to_UTF8(&pUtf, psAN->d.dNSName);
               strRet = QString::fromUtf8(reinterpret_cast<const char*>(pUtf), iLen);
               ::OPENSSL_free(pUtf);
            }
         }
      }
   }

   return(strRet);
}

QString CertificateInfo::text() const
{
   QString strRet;

   if (isReadable())
   {
      BIO* const pMemBio = ::BIO_new(BIO_s_mem());
      if (pMemBio)
      {
         ::X509_print(pMemBio, m_pX509);

         char* pcText;
         const long lSize = BIO_get_mem_data(pMemBio, &pcText);
         strRet = QString::fromAscii(pcText, lSize);
         ::BIO_free(pMemBio);
      }
   }

   return(strRet);
}

 QString CertificateInfo::subjectFieldByNid(int iNid) const
 {
   QString strRet;

   if (isReadable())
   {
      X509_NAME* const pSubject = ::X509_get_subject_name(m_pX509);
      if (pSubject)
      {
         const int iPos = ::X509_NAME_get_index_by_NID(pSubject, iNid, -1);
         if (iPos >= 0)
         {
            //::X509_NAME_get_index_by_NID(pSubject, NID_commonName, iPos) >=0 // Multiple common names;
            X509_NAME_ENTRY* const pEntry = ::X509_NAME_get_entry(pSubject, iPos);
            if (pEntry != NULL)
            {
               ASN1_STRING* const pEntryStr = ::X509_NAME_ENTRY_get_data(pEntry);
               if (pEntryStr != NULL)
               {
                  unsigned char* pUtf;
                  const int iLen = ::ASN1_STRING_to_UTF8(&pUtf, pEntryStr);
                  strRet = QString::fromUtf8(reinterpret_cast<const char*>(pUtf), iLen);
                  ::OPENSSL_free(pUtf);
               }
            }
         }
      }
   }

   return(strRet);
 }

X509* CertificateInfo::readCert(const QString& strFilePath)
{
   X509* pX509 = NULL;

   FILE* const pFile = ::fopen(strFilePath.toAscii().constData(), "r");
   if (pFile != NULL)
   {
      pX509 = ::PEM_read_X509(pFile, NULL, NULL, NULL);
      ::fclose(pFile);
   }

   return(pX509);
}


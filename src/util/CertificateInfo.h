/*
 * $Id$
 *
 * File:   CertificateInfo.h
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


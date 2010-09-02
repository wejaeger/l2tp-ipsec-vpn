/*
 * $Id$
 *
 * File:   SmartCardInfo.h
 * Author: Werner Jaeger
 *
 * Created on August 1, 2010, 2:13 PM
 */

#ifndef SMARTCARDINFO_H
#define	SMARTCARDINFO_H

#include <QString>

class CertificateInfo;

class SmartCardInfo
{
public:
   SmartCardInfo(const Pkcs11& p11, CK_OBJECT_HANDLE ulObjectHandle);
   virtual ~SmartCardInfo();

   const QString& serialNo() const { return(m_strSerialNo); }
   const QString& manufacturer() const { return(m_strManufacturer); }
   const QString& bitLength() const { return(m_strBitLength); }
   const QString& cardLabel() const { return(m_strCardLabel); }
   const QString& slotId() const { return(m_strSlotId); }
   const QString& objectLabel() const { return(m_strObjectLabel); }
   const QString& objectId() const { return(m_strObjectId); }
   const CertificateInfo& certificateInfo() const;

private:
   SmartCardInfo(const SmartCardInfo& orig);
   SmartCardInfo& operator=(const SmartCardInfo& orig);

   void loadToken(const Pkcs11& p11, CK_OBJECT_HANDLE ulObjectHandle);

   static QString BNOneLine(const BIGNUM* pBigNumber);

   QString m_strSerialNo;
   QString m_strManufacturer;
   QString m_strBitLength;
   QString m_strCardLabel;
   QString m_strSlotId;
   QString m_strObjectLabel;
   QString m_strObjectId;

   CertificateInfo* m_pCertificateInfo;
};

#endif	/* SMARTCARDINFO_H */


/*
 * $Id$
 *
 * File:   SmartCardInfo.cpp
 * Author: Werner Jaeger
 *
 * Created on August 1, 2010, 2:13 PM
 */

#include <QStringList>

#include <iostream>
#include <openssl/bn.h>
#include <openssl/x509.h>

#include "util/CertificateInfo.h"
#include "util/ErrorEx.h"
#include "Pkcs11.h"
#include "SmartCardInfo.h"

SmartCardInfo::SmartCardInfo(const Pkcs11& p11, CK_OBJECT_HANDLE ulObjectHandle) : m_pCertificateInfo(NULL)
{
   loadToken(p11, ulObjectHandle);
}

SmartCardInfo::~SmartCardInfo()
{
   if (m_pCertificateInfo)
      delete m_pCertificateInfo;
}

const CertificateInfo& SmartCardInfo::certificateInfo() const
{
   return(*m_pCertificateInfo);
}

void SmartCardInfo::loadToken(const Pkcs11& p11, CK_OBJECT_HANDLE ulObjectHandle)
{
   const QStringList tokenInfoList(p11.tokenInfo());
   m_strCardLabel = tokenInfoList[0];
   m_strManufacturer = tokenInfoList[1];
   m_strSerialNo = tokenInfoList[2];

   try
   {
      Pkcs11AttrUlong bitLengthAttribute(CKA_MODULUS_BITS);
      p11.loadAttribute(bitLengthAttribute, ulObjectHandle);
      m_strBitLength.setNum(bitLengthAttribute.getValue());
   }
   catch (ErrorEx &e)
   {
      // ignore
   }

   Pkcs11AttrData idAttribute(CKA_ID);
   p11.loadAttribute(idAttribute, ulObjectHandle);
   m_strObjectId = BNOneLine(idAttribute.getBignum());

   try
   {
      Pkcs11AttrData objectLabelAttribute(CKA_LABEL);
      p11.loadAttribute(objectLabelAttribute, ulObjectHandle);
      m_strObjectLabel = objectLabelAttribute.getText();
   }
   catch (ErrorEx &e)
   {
      // ignore
   }

   m_strSlotId.setNum(p11.slotId());

   Pkcs11AttrData x509ValueAttribute(CKA_VALUE);
   p11.loadAttribute(x509ValueAttribute, ulObjectHandle);
   const unsigned char* pcValue;
   unsigned long ulLen = x509ValueAttribute.getValue(&pcValue);

   if (m_pCertificateInfo)
      delete m_pCertificateInfo;

   m_pCertificateInfo = new CertificateInfo(::d2i_X509(NULL, &pcValue, ulLen));
}

QString SmartCardInfo::BNOneLine(const BIGNUM* pBigNumber)
{
   QString strRet;
   if (pBigNumber)
   {
      char* pcHex = ::BN_bn2hex(pBigNumber);
      strRet = pcHex;
      ::CRYPTO_free(pcHex);
   }
   return(strRet);
}





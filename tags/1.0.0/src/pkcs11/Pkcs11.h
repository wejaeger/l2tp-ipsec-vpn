/*
 * $Id$
 *
 * File:   Pkcs11.h
 * Author: Werner Jaeger
 *
 * Created on July 31, 2010, 12:10 PM
 */

#ifndef QPKCS11_H
#define	QPKCS11_H

#include <QString>
#include <opensc/pkcs11.h>
#include <ltdl.h>
#include "util/GlobalFunctions.h"
#include "Pkcs11Attribute.h"

class Pkcs11
{
public:
   Pkcs11();
   virtual ~Pkcs11();

   QStringList tokenInfo(CK_SLOT_ID ulSlotId) const;
   QStringList tokenInfo() const;

   bool protectedAuthenticationPath(CK_SLOT_ID ulSlotId) const;
   bool protectedAuthenticationPath() const;

   void startSession(unsigned long ulSlot, bool fRW = false);
   CK_SLOT_ID slotId() const { return(m_ulSlotId); }
   QList<unsigned long> slotList() const;
   QList<CK_OBJECT_HANDLE> objectList(const Pkcs11Attlist& atts) const;
   QList<CK_MECHANISM_TYPE> mechanismList(unsigned long ulSlot) const;

   void loadAttribute(Pkcs11Attribute& attribute, CK_OBJECT_HANDLE ulObjectHandle) const;
   void storeAttribute(Pkcs11Attribute& attribute, CK_OBJECT_HANDLE ulObjectHandle) const;

   void login(const unsigned char* pcPin, unsigned long ulPinlen, bool fAsSecurityOfficer) const;
   void logout() const;
   bool needsLogin(bool fAsSecurityOfficer) const;

   void setPin(const unsigned char* pcOldPin, unsigned long ulOldPinLen, const unsigned char* pcPin, unsigned long ulPinLen) const;
   void initPin(const unsigned char* pcPin, unsigned long ulPinLen) const;

   CK_OBJECT_HANDLE createObject(const Pkcs11Attlist& attrs) const;

   static bool loadLibrary(QString strFilePath, bool fSilent);
   static void pk11error(QString strfunc, int iReturnValue);
   static bool loaded() { return !!m_p11; };

private:
   Pkcs11(const Pkcs11& orig);
   Pkcs11& operator=(const Pkcs11& orig);

   CK_SESSION_HANDLE m_ulSessionHandle;
   CK_SLOT_ID m_ulSlotId;
   CK_OBJECT_HANDLE m_ulObjectHandle;

   static lt_dlhandle m_pLoadedModuleHandle;
   static CK_FUNCTION_LIST* m_p11;

	friend class Pkcs11Attribute;
	friend class Pkcs11AttrUlong;
	friend class Pkcs11AttrData;
};

#endif	/* QPKCS11_H */


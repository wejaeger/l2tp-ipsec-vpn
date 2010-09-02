/*
 * $Id$
 *
 * File:   SmartCardObjectListModel.cpp
 * Author: Werner Jaeger
 *
 * Created on August 2, 2010, 5:11 PM
 */

#include "pkcs11/Pkcs11.h"
#include "util/CertificateInfo.h"

#include "pkcs11/SmartCardInfo.h"
#include "settings/Preferences.h"
#include "SmartCardObjectListModel.h"

SmartCardObjectListModel::SmartCardObjectListModel(ObjectType objectType) : m_ObjectType(objectType), m_pSmartCardObjects(new QList<SmartCardInfo*>())
{
   readTokens();
}

SmartCardObjectListModel::~SmartCardObjectListModel()
{
   if (m_pSmartCardObjects)
      delete m_pSmartCardObjects;
}

int SmartCardObjectListModel::rowCount(const QModelIndex& /* parent */) const
{
   return(m_pSmartCardObjects->count());
}

QVariant SmartCardObjectListModel::data(const QModelIndex& index, int iRole) const
{
   QVariant ret;

   if (index.isValid())
   {
      switch (iRole)
      {
         case Qt::DisplayRole:
            ret = value(index.row()).toAscii().constData();
            break;

         case Qt::ToolTipRole:
            if (m_ObjectType == Certificate)
            {
               const QString strEmail = m_pSmartCardObjects->at(index.row())->certificateInfo().email();
               const QString strCN = m_pSmartCardObjects->at(index.row())->certificateInfo().cn();
               if (!strEmail.isEmpty())
                  ret = "User=" + strEmail;
               else if (!strCN.isEmpty())
                  ret = "CN=" + strCN;
            }
            break;

         case Qt::UserRole:
            ret = idValue(index.row()).toAscii().constData();
            break;

         case Qt::UserRole + 1:
            ret = m_pSmartCardObjects->at(index.row())->certificateInfo().email();
            break;
      }
    }

   return(ret);
}

void SmartCardObjectListModel::readTokens()
{
   if (Pkcs11::loaded())
   {
      Pkcs11 p11;

      const QList<unsigned long> slotList(p11.slotList());
      if (slotList.count() > 0)
      {
         for (int iSlotId = 0; iSlotId < slotList.count(); iSlotId++)
         {
            p11.startSession(iSlotId);

            switch (m_ObjectType)
            {
               case PublicKey:
               {
                  const Pkcs11Attlist publicKeyAttributeList(Pkcs11AttrUlong(CKA_CLASS, CKO_PUBLIC_KEY));
                  const QList<CK_OBJECT_HANDLE> publicKeyObjectHandleList(p11.objectList(publicKeyAttributeList));

                  for (int iObjectHandle = 0; iObjectHandle < publicKeyObjectHandleList.count(); iObjectHandle++)
                     m_pSmartCardObjects->append(new SmartCardInfo(p11, publicKeyObjectHandleList[iObjectHandle]));
               }
               break;

               case Certificate:
               {
                  Pkcs11Attlist certificateAttributeList(Pkcs11AttrUlong(CKA_CLASS, CKO_CERTIFICATE));
                  certificateAttributeList << Pkcs11AttrUlong(CKA_CERTIFICATE_TYPE, CKC_X_509);
                  const QList<CK_OBJECT_HANDLE> certificateObjectHandleList(p11.objectList(certificateAttributeList));

                  for (int iObjectHandle = 0; iObjectHandle < certificateObjectHandleList.count(); iObjectHandle++)
                     m_pSmartCardObjects->append(new SmartCardInfo(p11, certificateObjectHandleList[iObjectHandle]));
               }
               break;
            }
         }
      }
   }
}

QString SmartCardObjectListModel::value(int i) const
{
   QString strRet;

   if (i < m_pSmartCardObjects->count())
   {
      strRet.append(m_pSmartCardObjects->at(i)->cardLabel());
      strRet.append(", " + m_pSmartCardObjects->at(i)->manufacturer());
      strRet.append(", " + m_pSmartCardObjects->at(i)->slotId());
      strRet.append(", " + m_pSmartCardObjects->at(i)->objectId());
      strRet.append(", " + m_pSmartCardObjects->at(i)->objectLabel());
   }

   return(strRet);
}

QString SmartCardObjectListModel::idValue(int i) const
{
   QString strRet;

   if (i < m_pSmartCardObjects->count())
      strRet = OpenSSLSettings().engineId() + ":" + m_pSmartCardObjects->at(i)->slotId() + ":" + m_pSmartCardObjects->at(i)->objectId();

   return(strRet);
}
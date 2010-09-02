/*
 * $Id$
 *
 * File:   IPsecCertificateListModel.cpp
 * Author: Werner Jaeger
 *
 * Created on July 27, 2010, 1:50 PM
 */

#include <QDir>
#include "util/CertificateInfo.h"
#include "IPsecCertificateListModel.h"

static const char* const CERTIFICATEPATH = "/etc/ipsec.d/certs";

IPsecCertificateListModel::IPsecCertificateListModel(QObject* pParent) : QAbstractListModel(pParent), m_pCertificateDirectory(new QDir("/etc/ipsec.d/certs")), m_strSelected("")
{
   m_pCertificateDirectory->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
}

IPsecCertificateListModel::~IPsecCertificateListModel()
{
   delete m_pCertificateDirectory;
}

int IPsecCertificateListModel::rowCount(const QModelIndex& /* parent */) const
{
   return(m_pCertificateDirectory->entryList().size());
}

QVariant IPsecCertificateListModel::data(const QModelIndex& index, int iRole) const
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
            ret = "CN=" + CertificateInfo(m_pCertificateDirectory->path() + "/" + value(index.row())).cn();
            break;

         case Qt::CheckStateRole:
            ret = index.row() == indexOf(m_strSelected);
            break;
      }
    }

   return(ret);
}

bool IPsecCertificateListModel::setData(const QModelIndex& index, const QVariant& qvalue, int iRole)
{
   bool fRet = false;

   if (index.isValid() && iRole == Qt::CheckStateRole)
   {
      if (qvalue == Qt::Checked)
         m_strSelected = value(index.row());
      else
         m_strSelected = "";

      fRet = true;
      emit dataChanged(index, index);
   }

   return(fRet);
}

Qt::ItemFlags IPsecCertificateListModel::flags(const QModelIndex& /* index */) const
{
   return(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
}

const QString& IPsecCertificateListModel::selectedCertificateFileName() const
{
   return(m_strSelected);
}

bool IPsecCertificateListModel::select(const QString& strCertificateFileName)
{
   return(setData(index(indexOf(strCertificateFileName), 0), Qt::Checked, Qt::CheckStateRole));
}

int IPsecCertificateListModel::indexOf(const QString& strCertificateFileName) const
{
   return(QStringList(m_pCertificateDirectory->entryList()).indexOf(strCertificateFileName));
}

QString IPsecCertificateListModel::value(int i) const
{
   QString strRet;

   const QStringList list(m_pCertificateDirectory->entryList());
   if (i < list.size())
      strRet = list.at(i);

   return(strRet);
}
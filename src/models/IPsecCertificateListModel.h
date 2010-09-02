/*
 * $Id$
 *
 * File:   IPsecCertificateListModel.h
 * Author: Werner Jaeger
 *
 * Created on July 27, 2010, 1:50 PM
 */

#ifndef IPSECCERTIFICATELISTMODEL_H
#define	IPSECCERTIFICATELISTMODEL_H

#include <QAbstractListModel>

class QDir;

class IPsecCertificateListModel : public QAbstractListModel
{
   Q_OBJECT

public:
   explicit IPsecCertificateListModel(QObject* pParent = 0);
   virtual ~IPsecCertificateListModel();

   int rowCount(const QModelIndex& parent = QModelIndex()) const;
   QVariant data(const QModelIndex& index, int iRole = Qt::DisplayRole) const;
   bool setData(const QModelIndex& index, const QVariant& qvalue, int iRole = Qt::EditRole);
   Qt::ItemFlags flags(const QModelIndex& index) const;

   const QString& selectedCertificateFileName() const;
   bool select(const QString& strCertificateFileName);

private:
   IPsecCertificateListModel(const IPsecCertificateListModel& orig);
   IPsecCertificateListModel& operator=(const IPsecCertificateListModel& orig);

   int indexOf(const QString& strCertificateFileName) const;
   QString value(int i) const;

   QDir* const m_pCertificateDirectory;
   QString m_strSelected;

};

#endif	/* IPSECCERTIFICATELISTMODEL_H */


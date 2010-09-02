/*
 * $Id$
 *
 * File:   PppRoutesModel.h
 * Author: Werner Jaeger
 *
 * Created on August 5, 2010, 1:17 PM
 */

#ifndef PPPROUTESMODEL_H
#define	PPPROUTESMODEL_H

#include <QAbstractTableModel>
#include "settings/ConnectionSettings.h"

class PppRoutesModel : public QAbstractTableModel
{
   Q_OBJECT

public:
   explicit PppRoutesModel(const QString& strConnectionName, QObject* pParent = 0);
   virtual ~PppRoutesModel();

   int rowCount(const QModelIndex& parent = QModelIndex()) const;
   int columnCount(const QModelIndex& parent = QModelIndex()) const;
   bool setData(const QModelIndex& index, const QVariant& value, int iRole = Qt::EditRole);
   QVariant data(const QModelIndex& index, int iRole = Qt::DisplayRole) const;
   QVariant headerData(int iSection, Qt::Orientation orientation, int iRole = Qt::DisplayRole) const;
   Qt::ItemFlags flags(const QModelIndex& index) const;
   bool addRow();
   bool removeRow(int iRow);

private:
   PppRoutesModel(const PppRoutesModel& orig);
   PppRoutesModel& operator=(const PppRoutesModel& orig);

   const PppIpSettings m_PppIpSettings;
};

#endif	/* PPPROUTESMODEL_H */


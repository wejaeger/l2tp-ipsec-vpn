/*
 * $Id$
 *
 * File:   ConnectionsModel.h
 * Author: Werner Jaeger
 *
 * Created on July 18, 2010, 5:30 PM
 */

#ifndef CONNECTIONSMODEL_H
#define	CONNECTIONSMODEL_H

#include <QAbstractTableModel>

class ConnectionSettings;

class ConnectionsModel : public QAbstractTableModel
{
   Q_OBJECT

public:
   typedef enum { Ok, InvalidName, DuplicateName, ReadOnly } Result;

   explicit ConnectionsModel(QObject* pParent = 0);
   virtual ~ConnectionsModel();

   bool isWriteable() const;
   int rowCount(const QModelIndex& parent = QModelIndex()) const;
   int columnCount(const QModelIndex& parent = QModelIndex()) const;
   QVariant data(const QModelIndex& index, int iRole = Qt::DisplayRole) const;
   QVariant headerData(int iSection, Qt::Orientation orientation, int iRole = Qt::DisplayRole) const;
   Result addRow(const QString& strName, const QModelIndex& parent = QModelIndex());
   bool removeRow(int iRow, const QModelIndex& parent = QModelIndex());

private:
   ConnectionsModel(const ConnectionsModel& orig);
   ConnectionsModel& operator=(const ConnectionsModel& orig);

   ConnectionSettings* const m_pSettings;
};

#endif	/* CONNECTIONSMODEL_H */


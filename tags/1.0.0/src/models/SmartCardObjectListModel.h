/*
 * $Id$
 *
 * File:   SmartCardObjectListModel.h
 * Author: Werner Jaeger
 *
 * Created on August 2, 2010, 5:11 PM
 */

#ifndef SMARTCARDOBJECTLISTMODEL_H
#define	SMARTCARDOBJECTLISTMODEL_H

#include <QList>
#include <QAbstractListModel>

class SmartCardInfo;

class SmartCardObjectListModel : public QAbstractListModel
{
   Q_OBJECT

public:
   typedef enum { Certificate, PublicKey } ObjectType;

   explicit SmartCardObjectListModel(ObjectType objectType);
   virtual ~SmartCardObjectListModel();

   int rowCount(const QModelIndex& parent = QModelIndex()) const;
   QVariant data(const QModelIndex& index, int iRole = Qt::DisplayRole) const;

private:
   SmartCardObjectListModel(const SmartCardObjectListModel& orig);
   SmartCardObjectListModel& operator=(const SmartCardObjectListModel& orig);

   QString value(int i) const;
   QString idValue(int i) const;

   void readTokens();

   ObjectType const m_ObjectType;
   QList<SmartCardInfo*>* const m_pSmartCardObjects;
};

#endif	/* SMARTCARDOBJECTLISTMODEL_H */


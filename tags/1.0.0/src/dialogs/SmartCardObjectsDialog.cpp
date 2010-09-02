/*
 * $Id$
 *
 * File:   SmartCardObjectsDialog.cpp
 * Author: Werner Jaeger
 *
 * Created on August 2, 2010, 6:56 PM
 */

#include "SmartCardObjectsDialog.h"

SmartCardObjectsDialog::SmartCardObjectsDialog(SmartCardObjectListModel::ObjectType objectType, QWidget* pParent) : QDialog(pParent), m_pModel(new SmartCardObjectListModel(objectType))
{
   m_Widget.setupUi(this);
   m_Widget.m_pSmartCardObjectsListView->setModel(m_pModel);
}

SmartCardObjectsDialog::~SmartCardObjectsDialog()
{
   delete m_pModel;
}

const QString SmartCardObjectsDialog::selectedItem() const
{
   QString strItem;

   const QModelIndex currentIndex = m_Widget.m_pSmartCardObjectsListView->currentIndex();
   if (currentIndex.isValid())
      strItem = m_pModel->data(currentIndex, Qt::UserRole).toString();

   return(strItem);
}

const QString SmartCardObjectsDialog::selectedUserName() const
{
   QString strUserName;

   const QModelIndex currentIndex = m_Widget.m_pSmartCardObjectsListView->currentIndex();
   if (currentIndex.isValid())
      strUserName = m_pModel->data(currentIndex, Qt::UserRole + 1).toString();

   return(strUserName);
}



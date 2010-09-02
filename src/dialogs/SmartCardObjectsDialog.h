/*
 * $Id$
 *
 * File:   SmartCardObjectsDialog.h
 * Author: Werner Jaeger
 *
 * Created on August 2, 2010, 6:56 PM
 */

#ifndef _SMARTCARDOBJECTSDIALOG_H
#define	_SMARTCARDOBJECTSDIALOG_H

#include "models/SmartCardObjectListModel.h"
#include "ui_SmartCardObjectsDialog.h"

class SmartCardObjectsDialog : public QDialog
{
   Q_OBJECT

public:
   explicit SmartCardObjectsDialog(SmartCardObjectListModel::ObjectType objectType, QWidget* pParent = 0);
   virtual ~SmartCardObjectsDialog();

   const QString selectedItem() const;
   const QString selectedUserName() const;

private:
   SmartCardObjectsDialog(const SmartCardObjectsDialog& orig);
   SmartCardObjectsDialog& operator=(const SmartCardObjectsDialog& orig);

   Ui::SmartCardObjectsDialog m_Widget;

   SmartCardObjectListModel* const m_pModel;
};

#endif	/* _SMARTCARDOBJECTSDIALOG_H */

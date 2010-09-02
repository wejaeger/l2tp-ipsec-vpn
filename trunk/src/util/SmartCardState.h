/*
 * $Id$
 *
 * File:   SmartCardState.h
 * Author: Werner Jaeger
 *
 * Created on July 16, 2010, 10:31 AM
 */

#ifndef SMARTCARDSTATE_H
#define	SMARTCARDSTATE_H

#include <QObject>

class SmartCardInsertWaitTask;
class QMessageBox;

class SmartCardState : public QObject
{
   Q_OBJECT

public:
   explicit SmartCardState(QObject* pParent = 0);
   virtual ~SmartCardState();

   int readersAvailable();
   int waitForCardPresent();

signals:
   void cardInserted();

private slots:
   void waitTaskFinished();

private:
   SmartCardState(const SmartCardState& orig);
   SmartCardState& operator=(const SmartCardState& orig);

   SmartCardInsertWaitTask* m_pWaitTask;
   QMessageBox* m_pMsgBox;
};

#endif	/* SMARTCARDSTATE_H */


/*
 * $Id$
 *
 * File:   SmartCardInsertWaitTask.h
 * Author: Werner Jaeger
 *
 * Created on July 16, 2010, 7:08 PM
 */

#ifndef SMARTCARDINSERTWAITTASK_H
#define	SMARTCARDINSERTWAITTASK_H

#include <QThread>

struct sc_reader;

class SmartCardInsertWaitTask : public QThread
{
public:
   SmartCardInsertWaitTask(struct sc_reader* pReader, int  iSlot);
   virtual ~SmartCardInsertWaitTask();

   void run();
   void stop() { m_fStop = true;  wait(); }

   int result() const { return(m_iRet); }

private:
   SmartCardInsertWaitTask(const SmartCardInsertWaitTask& orig);
   SmartCardInsertWaitTask& operator=(const SmartCardInsertWaitTask& orig);

   struct sc_reader* const m_pReader;
   int const m_iSlot;

   int m_iRet;
   volatile bool m_fStop;
};

#endif	/* SMARTCARDINSERTWAITTASK_H */


/*
 * $Id$
 *
 * File:   SmartCardInsertWaitTask.h
 * Author: Werner Jaeger
 *
 * Created on July 16, 2010, 7:08 PM
 *
 * Copyright 2010 Werner Jaeger.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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


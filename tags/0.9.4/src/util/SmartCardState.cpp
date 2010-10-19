/*
 * $Id$
 *
 * File:   SmartCardState.cpp
 * Author: Werner Jaeger
 *
 * Created on July 16, 2010, 10:31 AM
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

#include <QCoreApplication>
#include <qt4/QtCore/qglobal.h>
#include <qt4/QtGui/qmessagebox.h>
#include <qt4/QtGui/qpushbutton.h>
#include <opensc/opensc.h>

#include "SmartCardState.h"
#include "SmartCardInsertWaitTask.h"

SmartCardState::SmartCardState(QObject* pParent) : QObject(pParent), m_pWaitTask(NULL),
   m_pMsgBox(new QMessageBox(QMessageBox::Information, QCoreApplication::instance()->applicationName(), QObject::tr("Please insert your smart card ..."), QMessageBox::Cancel))
{
}

SmartCardState::~SmartCardState()
{

   if (m_pWaitTask)
      delete m_pWaitTask;

   if (m_pMsgBox)
      delete m_pMsgBox;
}

int SmartCardState::readersAvailable()
{
   sc_context_t* pCtx = NULL;
   int iRet = ::sc_context_create(&pCtx, NULL);
   if (iRet == 0)
   {
      iRet = ::sc_ctx_get_reader_count(pCtx);
      ::sc_release_context(pCtx);
   }

   return(iRet);
}

int SmartCardState::waitForCardPresent()
{
   sc_context_t* pCtx = NULL;
   int iRet = ::sc_context_create(&pCtx, NULL);
   if (iRet == 0)
   {
      const int iReaderCount = ::sc_ctx_get_reader_count(pCtx);
      sc_reader_t* paReaders[16];
      int iaSlots[16];
      int iNoSlots;
      int i, j;

      for (i = iNoSlots = 0; i < iReaderCount; i++)
      {
         sc_reader_t* const pReader = ::sc_ctx_get_reader(pCtx, i);
         for (j = 0; pReader && j < pReader->slot_count; j++, iNoSlots++)
         {
            paReaders[iNoSlots] = pReader;
            iaSlots[iNoSlots] = j;
         }
      }

      if ((iRet = ::sc_detect_card_presence(paReaders[0], iaSlots[0])) <= 0)
      {
         if (m_pWaitTask != NULL)
            delete m_pWaitTask;

         m_pWaitTask = new SmartCardInsertWaitTask(paReaders[0], iaSlots[0]);
         connect(m_pWaitTask, SIGNAL(finished()), this, SLOT(waitTaskFinished()));
         m_pWaitTask->start();

         iRet = m_pMsgBox->exec();
         if(iRet == QMessageBox::Cancel)
         {
            m_pWaitTask->stop();
            iRet = 0;
         }
      }
      ::sc_release_context(pCtx);
   }

   return(iRet);
}

void SmartCardState::waitTaskFinished()
{
   const int iRet = m_pWaitTask->result();
   m_pMsgBox->done(iRet);
}

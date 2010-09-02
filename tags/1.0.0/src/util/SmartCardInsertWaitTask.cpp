/*
 * $Id$
 *
 * File:   SmartCardInsertWaitTask.cpp
 * Author: Werner Jaeger
 *
 * Created on July 16, 2010, 7:08 PM
 */

#include "SmartCardInsertWaitTask.h"
#include <opensc/opensc.h>

SmartCardInsertWaitTask::SmartCardInsertWaitTask(sc_reader_t* pReader, int iSlot) : m_pReader(pReader),  m_iSlot(iSlot), m_iRet(0), m_fStop(false)
{
}

SmartCardInsertWaitTask::~SmartCardInsertWaitTask()
{
}

void SmartCardInsertWaitTask::run()
{
   m_fStop = false;

   do
   {
      m_iRet = ::sc_detect_card_presence(m_pReader, m_iSlot);
      msleep(100);
   }
   while (m_iRet == 0 && !m_fStop);
}

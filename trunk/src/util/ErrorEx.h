/*
 * $Id$
 *
 * File:   ErrorEx.h
 * Author: Werner Jaeger
 *
 * Created on July 31, 2010, 4:30 PM
 */

#ifndef ERROREX_H
#define	ERROREX_H

#include <QObject>
#include <QString>

class ErrorEx
{
public:
   ErrorEx(QString strText, QString strClassName = "")
   {
      m_strMsg = strText;
      if (!strClassName.isEmpty())
         m_strMsg += " (" + strClassName + ")";
   }

   ErrorEx(const ErrorEx& errorEx)
   {
      m_strMsg = errorEx.m_strMsg;
   }

   virtual ~ErrorEx() {}

   void appendString(QString strText)
   {
      m_strMsg = m_strMsg + " " + strText;
   }

   QString getString() const
   {
      return(m_strMsg);
   }

   const char* getCString() const
   {
      return(m_strMsg.toAscii());
   }

   bool isEmpty() const
   {
      return(m_strMsg.isEmpty());
   }

   static void checkOutOfMemory(const void* pVoid)
   {
      if (!pVoid) throw ErrorEx(QObject::tr("Out of Memory"));
   }

private:
   ErrorEx& operator=(const ErrorEx& orig);

	QString m_strMsg;
};

#endif	/* ERROREX_H */


/*
 * $Id$
 *
 * File:   Settings.cpp
 * Author: Werner Jaeger
 *
 * Created on August 9, 2010, 11:36 AM
 */

#include <QSettings>
#include <QStringList>

#include "Settings.h"

Settings::Settings()
{
}

Settings::~Settings()
{
}

/**
 * Unfortunately QSettings does not provide a method to remove array elements.
 *
 * So, we have to read in all keys and values of the named array section, delete
 * the whole array section and write it out again, omitting the deleted
 * row. We also have to take care to adapt the array indices after the deleted
 * row and to write finally the new size entry.
 */
bool Settings::removeArrayItem(QSettings* pSettings, const QString& strArrayName, int iIndex)
{
   bool fRet = false;

   pSettings->beginReadArray(strArrayName);
   const QStringList keys(pSettings->allKeys());
   QStringList values;

   if (iIndex < keys.size())
   {
      for (int i = 0; i < keys.size(); i++)
         values.insert(i, pSettings->value(keys.at(i)).toString());
      pSettings->endArray();

      pSettings->beginGroup(strArrayName);
      pSettings->remove("");

      const QChar cIndex2Remove(iIndex + 1 + 48);
      QChar cIndex2Write;
      for (int i = 0; i < keys.size(); i++)
      {
         const QString& strKey = keys.at(i);
         const QChar cIndex2Read(strKey.at(0));
         if (cIndex2Read.isDigit())
         {
            if (cIndex2Read != cIndex2Remove)
            {
               cIndex2Write = cIndex2Read > cIndex2Remove ? QChar(cIndex2Read.digitValue() - 1 + 48) : QChar(cIndex2Read.digitValue() + 48);
               pSettings->setValue(cIndex2Write + strKey.mid(1), values.at(i));
            }
         }
      }
      pSettings->setValue("size", QString(cIndex2Write));
      pSettings->endGroup();
      fRet = true;
   }

   return(fRet);
}


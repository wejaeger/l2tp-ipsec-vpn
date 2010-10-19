/*
 * $Id$
 *
 * File:   Settings.cpp
 * Author: Werner Jaeger
 *
 * Created on August 9, 2010, 11:36 AM
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


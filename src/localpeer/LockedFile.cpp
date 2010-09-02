/*
 * $Id$
 *
 * File:   LockedFile.cpp
 * Author:  Werner Jaeger
 *
 * Created on September 2, 2010, 6:42 PM
 */

#include <errno.h>
#include <fcntl.h>

#include "LockedFile.h"

LockedFile::LockedFile() : QFile(), m_LockMode(NoLock)
{
}

LockedFile::LockedFile(const QString& strName) : QFile(strName), m_LockMode(NoLock)
{
}

LockedFile::~LockedFile()
{
   if (isOpen())
      unlock();
}

bool LockedFile::open(OpenMode mode)
{
   bool fRet(false);

   if (mode & QIODevice::Truncate)
      qWarning("LockedFile::open(): Truncate mode not allowed.");
   else
      fRet = QFile::open(mode);

   return(fRet);
}

bool LockedFile::lock(LockMode mode, bool fBlock)
{
   bool fRet(false);

   if (isOpen())
   {
      if (mode != NoLock)
      {
         if (mode != m_LockMode)
         {
            if (m_LockMode != NoLock)
               unlock();

            struct flock fl;
            fl.l_whence = SEEK_SET;
            fl.l_start = 0;
            fl.l_len = 0;
            fl.l_type = (mode == ReadLock) ? F_RDLCK : F_WRLCK;

            if (::fcntl(handle(),  fBlock ? F_SETLKW : F_SETLK, &fl) != -1)
            {
               m_LockMode = mode;
               fRet = true;
            }
            else
            {
               if (errno != EINTR && errno != EAGAIN)
                  qWarning("LockedFile::lock(): fcntl: %s", ::strerror(errno));
            }
         }
         else
            fRet = true;
      }
      else
         fRet = unlock();
   }
   else
      qWarning("LockedFile::lock(): file is not opened");

   return(fRet);
}

bool LockedFile::unlock()
{
   bool fRet(false);

   if (isOpen())
   {
      if (isLocked())
      {
         struct flock fl;
         fl.l_whence = SEEK_SET;
         fl.l_start = 0;
         fl.l_len = 0;
         fl.l_type = F_UNLCK;

         if (::fcntl(handle(), F_SETLKW, &fl) != -1)
         {
            m_LockMode = NoLock;
            fRet = true;
         }
         else
            qWarning("LockedFile::lock(): fcntl: %s", ::strerror(errno));
      }
      else
         fRet = true;
   }
   else
      qWarning("LockedFile::unlock(): file is not opened");

   return(fRet);
}

bool LockedFile::isLocked() const
{
   return(m_LockMode != NoLock);
}

LockedFile::LockMode LockedFile::lockMode() const
{
   return(m_LockMode);
}
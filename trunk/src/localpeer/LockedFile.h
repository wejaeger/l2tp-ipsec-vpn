/*
 * $Id$
 *
 * File:   LockedFile.h
 * Author:  Werner Jaeger
 *
 * Created on September 2, 2010, 6:42 PM
 */

#include <QFile>

#ifndef LOCKEDFILE_H
#define	LOCKEDFILE_H

class LockedFile : public QFile
{
public:
   enum LockMode { NoLock = 0, ReadLock, WriteLock };

   LockedFile();
   LockedFile(const QString &strName);
   virtual ~LockedFile();

   bool open(OpenMode mode);

   bool lock(LockMode mode, bool fBlock = true);
   bool unlock();
   bool isLocked() const;
   LockMode lockMode() const;

private:
   LockedFile(const LockedFile& orig);
   LockedFile& operator=(const LockedFile& orig);

   LockMode m_LockMode;
};

#endif	/* LOCKEDFILE_H */


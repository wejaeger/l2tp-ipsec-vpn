/*
 * $Id$
 *
 * File:   AbstractConfWriter.h
 * Author: Werner Jaeger
 *
 * Created on July 23, 2010, 8:43 AM
 */

#ifndef ABSTRACTCONFWRITER_H
#define	ABSTRACTCONFWRITER_H

#include <QString>
#include <ctemplate/template.h>

class AbstractConfWriter
{
public:
   const QString& write();
   const QString& instance() const;
   void setInstance(const QString& strInstance);
   const QString& fileName() const;

protected:
   typedef enum { NORMAL, EXECUTABLE, SECRET } FileType;

   AbstractConfWriter(const QString& strTemplateKey, const QString& strWriteTo, FileType type = NORMAL);
   virtual ~AbstractConfWriter();

   const QString& templateKey() const { return(m_strTemplateKey); }
   ctemplate::TemplateDictionary* dictionary() const { return(m_pDictionary); }
   void addErrorMsg(const QString& strErrorMsg);

   virtual void fill() = 0;

private:
   AbstractConfWriter(const AbstractConfWriter& orig);
   AbstractConfWriter& operator=(const AbstractConfWriter& orig);

   void save();

   QString readTemplate(const QString& strKey);

   void newDictionary();

   const QString m_strTemplateKey;
   const QString m_strWriteTo;
   const FileType m_Type;
   QString m_strInstance;
   QString m_strFileName;
   ctemplate::TemplateDictionary* m_pDictionary;

   QString m_strLastErrorMsg;
};

#endif	/* ABSTRACTCONFWRITER_H */


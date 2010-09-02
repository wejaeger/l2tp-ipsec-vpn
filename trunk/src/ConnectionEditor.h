/*
 * $Id$
 *
 * File:   ConnectionEditor.h
 * Author: Werner Jaeger
 *
 * Created on August 30, 2010, 12:59 PM
 */

#ifndef CONNECTIONEDITOR_H
#define	CONNECTIONEDITOR_H

#include <QObject>

class L2tpIPsecVpnApplication;
class ConnectionEditorDialog;

class ConnectionEditor : public QObject
{
   Q_OBJECT

public:
   explicit ConnectionEditor(L2tpIPsecVpnApplication& application, QObject* pParent = 0);
   virtual ~ConnectionEditor();

   int exec();

private slots:
   void onConnectionAdded(const QString& strName);
   void onConnectionRemoved(const QString& strName);

private:
   ConnectionEditor(const ConnectionEditor& orig);
   ConnectionEditor& operator=(const ConnectionEditor& orig);

   L2tpIPsecVpnApplication& m_Application;
   ConnectionEditorDialog* const m_pConnectionEditorDialog;
};

#endif	/* CONNECTIONEDITOR_H */


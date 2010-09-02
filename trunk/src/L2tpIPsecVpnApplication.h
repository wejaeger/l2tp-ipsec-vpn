/*
 * $Id$
 *
 * File:   L2tpIPsecVpnApplication.h
 * Author: Werner Jaeger
 *
 * Created on August 9, 2010, 2:29 PM
 */

#ifndef L2TPIPSECVPNAPPLICATION_H
#define	L2TPIPSECVPNAPPLICATION_H

#include <qtsingleapplication.h>

class ConnectionManager;
class QProcess;

class L2tpIPsecVpnApplication : public QtSingleApplication
{
   Q_OBJECT

public:
   enum APPLICATIONMODE
   {
      CONNECTION_MANAGER,
      CONNECTION_EDITOR,
      PASSWORD_CALLBACK
   };

   L2tpIPsecVpnApplication(int& iArgc, char** ppArgv);
   virtual ~L2tpIPsecVpnApplication();

   APPLICATIONMODE mode() const;
   bool sendConnectionAddedMessage(const QString& strConnectionName);
   bool sendConnectionRemovedMessage(const QString& strConnectionName);

   int startConnectionEditorDialog() const;

signals:
   void connectionAdded(const QString& strConnectionName);
   void connectionRemoved(const QString& strConnectionName);
   void connectionEditorDialogClosed(int iExitCode);

private slots:
   void onMessageReceived(const QString& strMessage);
   void onConnectionEditorDialogClosed(int iExitCode);

private:
   L2tpIPsecVpnApplication(const L2tpIPsecVpnApplication& orig);
   L2tpIPsecVpnApplication & operator=(const L2tpIPsecVpnApplication& orig);

   bool isConnectionEditor() const;
   bool isPasswordCallback() const;

   QProcess* const m_pProcess;
};

#endif	/* L2TPIPSECVPNAPPLICATION_H */


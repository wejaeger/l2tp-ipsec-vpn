/*
 * $Id$
 *
 * File:   PasswordCallback.h
 * Author: Werner Jaeger
 *
 * Created on September 1, 2010, 4:20 PM
 */

#ifndef PASSWORDCALLBACK_H
#define	PASSWORDCALLBACK_H

class L2tpIPsecVpnApplication;

class PasswordCallback
{
public:
   PasswordCallback(L2tpIPsecVpnApplication& application);
   virtual ~PasswordCallback();

   int exec() const;

private:
   PasswordCallback(const PasswordCallback& orig);
   PasswordCallback& operator=(const PasswordCallback& orig);

   static QString promptPwd(const QString& strTitle, const QString& strLabel);

   L2tpIPsecVpnApplication& m_Application;
};

#endif	/* PASSWORDCALLBACK_H */


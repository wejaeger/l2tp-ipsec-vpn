/*
 * $Id$
 *
 * File:   ConnectionState.h
 * Author: Werner Jaeger
 *
 * Created on July 8, 2010, 10:35 AM
 */

#ifndef CONNECTIONSTATE_H
#define	CONNECTIONSTATE_H

#include <QSystemTrayIcon>

class QIcon;
class QString;

class ConnectionState
{
public:
   enum State {NotConnected, Connecting, Disconnecting, Connected, Error};

   ConnectionState(const QString& strHostName, const QString& strMsgTitle, const QString& strMsgBody, const QIcon& icon, const QSystemTrayIcon::MessageIcon& msgIcon);
   virtual ~ConnectionState();

   virtual bool isState(State state) const = 0;
   const QString& hostName() const;
   const QString& msgTitle() const;
   const QString& msgBody() const;
   const QIcon& icon() const;
   const QSystemTrayIcon::MessageIcon& msgIcon() const;

private:
   ConnectionState(const ConnectionState& orig);
   ConnectionState& operator=(const ConnectionState& orig);

   const QString m_strHostName;
   const QString m_strMsgTitle;
   const QString m_strMsgBody;
   const QIcon m_Icon;
   const QSystemTrayIcon::MessageIcon m_MsgIcon;
};

class NotConnected : public ConnectionState
{
public:
   NotConnected();
   virtual ~NotConnected();
   virtual bool isState(State state) const { return(state == ConnectionState::NotConnected ? true : false); }

private:
   NotConnected(const NotConnected& orig);
   NotConnected& operator=(const NotConnected& orig);
};

class Connecting : public ConnectionState
{
public:
   Connecting(const QString& strHostName);
   virtual ~Connecting();
   virtual bool isState(State state) const { return(state == ConnectionState::Connecting ? true : false); }

private:
   Connecting(const Connecting& orig);
   Connecting& operator=(const Connecting& orig);
};

class Disconnecting : public ConnectionState
{
public:
   Disconnecting(const QString& strHostName);
   virtual ~Disconnecting();
   virtual bool isState(State state) const { return(state == ConnectionState::Disconnecting ? true : false); }

private:
   Disconnecting(const Disconnecting& orig);
   Disconnecting& operator=(const Disconnecting& orig);
};

class Connected : public ConnectionState
{
public:
   Connected(const QString& strHostName);
   virtual ~Connected();
   virtual bool isState(State state) const { return(state == ConnectionState::Connected ? true : false); }

private:
   Connected(const Connected& orig);
   Connected& operator=(const Connected& orig);
};

class Error : public ConnectionState
{
public:
   Error(const QString& strHostName, int iReturnCocde, bool fDisconnecting);
   virtual ~Error();
   virtual bool isState(State state) const { return(state == ConnectionState::Error ? true : false); }

private:
   Error(const Error& orig);
   Error& operator=(const Error& orig);
};

#endif	/* CONNECTIONSTATE_H */


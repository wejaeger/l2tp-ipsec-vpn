/*
 * $Id$
 *
 * File:   ConnectionState.cpp
 * Author: Werner Jaeger
 *
 * Created on July 8, 2010, 10:35 AM
 */

#include "ConnectionState.h"

ConnectionState::ConnectionState(const QString& strHostName, const QString& strMsgTitle, const QString& strMsgBody, const QIcon& icon, const QSystemTrayIcon::MessageIcon& msgIcon) : m_strHostName(strHostName), m_strMsgTitle(strMsgTitle), m_strMsgBody(strMsgBody), m_Icon(icon), m_MsgIcon(msgIcon)
{
}

ConnectionState::~ConnectionState()
{
}

const QString& ConnectionState::hostName() const
{
   return(m_strHostName);
}

const QString& ConnectionState::msgTitle() const
{
   return(m_strMsgTitle);
}

const QString& ConnectionState::msgBody() const
{
   return(m_strMsgBody);
}

const QIcon& ConnectionState::icon() const
{
   return(m_Icon);
}

const QSystemTrayIcon::MessageIcon& ConnectionState::msgIcon() const
{
   return(m_MsgIcon);
}

NotConnected::NotConnected() : ConnectionState("", QObject::tr("Not Connected"), QObject::tr("Click to show details"), QIcon(":/images/connectNo.png"), QSystemTrayIcon::Warning)
{
}

NotConnected::~NotConnected()
{
}

Connecting::Connecting(const QString& strHostName) : ConnectionState(strHostName, QObject::tr("Connecting to ") + strHostName + " ...", QObject::tr("Click to show details"), QIcon(":/images/busy.gif"), QSystemTrayIcon::Information)
{
}

Connecting::~Connecting()
{
}

Disconnecting::Disconnecting(const QString& strHostName) : ConnectionState(strHostName, QObject::tr("Disconnecting from ") + strHostName + " ...", QObject::tr("Click to show details"), QIcon(":/images/busy.gif"), QSystemTrayIcon::Information)
{
}

Disconnecting::~Disconnecting()
{
}

Connected::Connected(const QString& strHostName) : ConnectionState(strHostName, QObject::tr("Connected to ") + strHostName, QObject::tr("Click to show details"), QIcon(":/images/connectEstablished.png"), QSystemTrayIcon::Information)
{
}

Connected::~Connected()
{
}

Error::Error(const QString& strHostName, int iReturnCode, bool fDisconnecting) : ConnectionState(strHostName, (fDisconnecting ? QObject::tr("Error %1 occurred when disconnecting from ").arg(iReturnCode) : QObject::tr("Error %1 occurred when connecting to ").arg(iReturnCode)) + strHostName, QObject::tr("Click to show details"), QIcon(":/images/connectNo.png"), QSystemTrayIcon::Critical)
{
}

Error::~Error()
{
}

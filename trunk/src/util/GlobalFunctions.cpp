#include <QRegExp>

#include "GlobalFunctions.h"

static QString HOSTNAMEPATTERN("([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+[a-zA-Z]{2,6}");
static QString IPV4PATTERN("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
static QString IPPATTERN("\\s*((([0-9A-Fa-f]{1,4}:){7}(([0-9A-Fa-f]{1,4})|:))|(([0-9A-Fa-f]{1,4}:){6}(:|((25[0-5]|2[0-4]\\d|[01]?\\d{1,2})(\\.(25[0-5]|2[0-4]\\d|[01]?\\d{1,2})){3})|(:[0-9A-Fa-f]{1,4})))|(([0-9A-Fa-f]{1,4}:){5}((:((25[0-5]|2[0-4]\\d|[01]?\\d{1,2})(\\.(25[0-5]|2[0-4]\\d|[01]?\\d{1,2})){3})?)|((:[0-9A-Fa-f]{1,4}){1,2})))|(([0-9A-Fa-f]{1,4}:){4}(:[0-9A-Fa-f]{1,4}){0,1}((:((25[0-5]|2[0-4]\\d|[01]?\\d{1,2})(\\.(25[0-5]|2[0-4]\\d|[01]?\\d{1,2})){3})?)|((:[0-9A-Fa-f]{1,4}){1,2})))|(([0-9A-Fa-f]{1,4}:){3}(:[0-9A-Fa-f]{1,4}){0,2}((:((25[0-5]|2[0-4]\\d|[01]?\\d{1,2})(\\.(25[0-5]|2[0-4]\\d|[01]?\\d{1,2})){3})?)|((:[0-9A-Fa-f]{1,4}){1,2})))|(([0-9A-Fa-f]{1,4}:){2}(:[0-9A-Fa-f]{1,4}){0,3}((:((25[0-5]|2[0-4]\\d|[01]?\\d{1,2})(\\.(25[0-5]|2[0-4]\\d|[01]?\\d{1,2})){3})?)|((:[0-9A-Fa-f]{1,4}){1,2})))|(([0-9A-Fa-f]{1,4}:)(:[0-9A-Fa-f]{1,4}){0,4}((:((25[0-5]|2[0-4]\\d|[01]?\\d{1,2})(\\.(25[0-5]|2[0-4]\\d|[01]?\\d{1,2})){3})?)|((:[0-9A-Fa-f]{1,4}){1,2})))|(:(:[0-9A-Fa-f]{1,4}){0,5}((:((25[0-5]|2[0-4]\\d|[01]?\\d{1,2})(\\.(25[0-5]|2[0-4]\\d|[01]?\\d{1,2})){3})?)|((:[0-9A-Fa-f]{1,4}){1,2})))|(((25[0-5]|2[0-4]\\d|[01]?\\d{1,2})(\\.(25[0-5]|2[0-4]\\d|[01]?\\d{1,2})){3})))(%.+)?\\s*");
//static const QRegExp REVALIDGATEWAY(QString("^") + QString("(") + HOSTNAMEPATTERN + ")|(" + IPPATTERN + ")" + "$");
static const QRegExp REVALIDIPV4("^" + IPV4PATTERN + "$");
static const QRegExp REVALIDIP("^" + IPPATTERN + "$");
static const QRegExp REVALIDHOSTNAME("^" + HOSTNAMEPATTERN + "$");
static const QRegExp REVALIDHOSTNAMELIST("^" + HOSTNAMEPATTERN + "(?:\\s+" + HOSTNAMEPATTERN + ")*$");

const QRegExp& ipv4ValidationRE()
{
   return(REVALIDIPV4);
}

const QRegExp& ipValidationRE()
{
   return(REVALIDIP);
}

const QRegExp& hostNameValidationRE()
{
   return(REVALIDHOSTNAME);
}

const QRegExp& hostNameListValidationRE()
{
   return(REVALIDHOSTNAMELIST);
}

QString utf82QString(const void* vpValue, unsigned long ulLen)
{
   const QString str(QString::fromUtf8(static_cast<const char*>(vpValue), ulLen));
   return(str.trimmed());
}

QByteArray fileName2ByteArray(const QString& strFileName)
{
#ifdef WIN32
	return(strFileName.toLocal8Bit());
#else
	return(strFileName.toUtf8());
#endif
}

const char* string2FileName(const QString& strFileNme)
{
   return(fileName2ByteArray(strFileNme).constData());
}

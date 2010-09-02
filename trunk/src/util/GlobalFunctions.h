/*
 * $Id$
 *
 * File:   GlobalFunctions.h
 * Author: Werner Jaeger
 *
 * Created on July 31, 2010, 7:28 PM
 */

#ifndef GLOBALFUNCTIONS_H
#define	GLOBALFUNCTIONS_H

#include <QString>

class QRegExp;

const QRegExp& ipv4ValidationRE();
const QRegExp& ipValidationRE();
const QRegExp& hostNameValidationRE();
const QRegExp& hostNameListValidationRE();

QString utf82QString(const void* vpValue, unsigned long ulLen);
QByteArray fileName2ByteArray(const QString& strFileName);
const char* string2FileName(const QString& strFileNme);

#endif	/* GLOBALFUNCTIONS_H */


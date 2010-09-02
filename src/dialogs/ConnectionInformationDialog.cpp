/*
 * $Id$
 *
 * File:   ConnectionInformationDialog.cpp
 * Author: Werner Jaeger
 *
 * Created on July 18, 2010, 3:37 PM
 */

#include "ConnectionInformationDialog.h"

ConnectionInformationDialog::ConnectionInformationDialog(QWidget* pParent) : QDialog(pParent)
{
   m_Widget.setupUi(this);
}

ConnectionInformationDialog::~ConnectionInformationDialog()
{
}

void ConnectionInformationDialog::appendLogColorText(const QColor&, const char* pcText)
{
   const QColor currentColor = m_Widget.m_pTextEdit->textColor();

   if (pcText)
   {
      m_Widget.m_pTextEdit->setTextColor(QColor(255, 0, 0));
      m_Widget.m_pTextEdit->insertPlainText(QString(pcText));
      m_Widget.m_pTextEdit->setTextColor(currentColor);
   }
}


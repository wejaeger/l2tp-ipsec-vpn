/********************************************************************************
** Form generated from reading UI file 'AdvancedSettingsDialog.ui'
**
** Created: Sat Sep 18 17:20:10 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADVANCEDSETTINGSDIALOG_H
#define UI_ADVANCEDSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_AdvancedSettingsDialog
{
public:
    QDialogButtonBox *m_pButtonBox;

    void setupUi(QDialog *AdvancedSettingsDialog)
    {
        if (AdvancedSettingsDialog->objectName().isEmpty())
            AdvancedSettingsDialog->setObjectName(QString::fromUtf8("AdvancedSettingsDialog"));
        AdvancedSettingsDialog->resize(400, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/configure.png"), QSize(), QIcon::Normal, QIcon::Off);
        AdvancedSettingsDialog->setWindowIcon(icon);
        m_pButtonBox = new QDialogButtonBox(AdvancedSettingsDialog);
        m_pButtonBox->setObjectName(QString::fromUtf8("m_pButtonBox"));
        m_pButtonBox->setGeometry(QRect(30, 240, 341, 32));
        m_pButtonBox->setOrientation(Qt::Horizontal);
        m_pButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(AdvancedSettingsDialog);
        QObject::connect(m_pButtonBox, SIGNAL(accepted()), AdvancedSettingsDialog, SLOT(accept()));
        QObject::connect(m_pButtonBox, SIGNAL(rejected()), AdvancedSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AdvancedSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *AdvancedSettingsDialog)
    {
        AdvancedSettingsDialog->setWindowTitle(QApplication::translate("AdvancedSettingsDialog", "Advanced Settings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AdvancedSettingsDialog: public Ui_AdvancedSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADVANCEDSETTINGSDIALOG_H

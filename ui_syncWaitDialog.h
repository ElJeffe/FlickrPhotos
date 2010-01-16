/********************************************************************************
** Form generated from reading UI file 'syncwaitdialog.ui'
**
** Created: Sat 16. Jan 15:18:38 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYNCWAITDIALOG_H
#define UI_SYNCWAITDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_SyncWaitDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;

    void setupUi(QDialog *SyncWaitDialog)
    {
        if (SyncWaitDialog->objectName().isEmpty())
            SyncWaitDialog->setObjectName(QString::fromUtf8("SyncWaitDialog"));
        SyncWaitDialog->resize(403, 82);
        horizontalLayout = new QHBoxLayout(SyncWaitDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(SyncWaitDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label->setWordWrap(true);

        horizontalLayout->addWidget(label);


        retranslateUi(SyncWaitDialog);

        QMetaObject::connectSlotsByName(SyncWaitDialog);
    } // setupUi

    void retranslateUi(QDialog *SyncWaitDialog)
    {
        SyncWaitDialog->setWindowTitle(QApplication::translate("SyncWaitDialog", "Synchronizing", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SyncWaitDialog", "Synchronizing data with Flickr. Please wait...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SyncWaitDialog: public Ui_SyncWaitDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNCWAITDIALOG_H

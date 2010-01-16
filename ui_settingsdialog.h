/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created: Sat 16. Jan 15:18:38 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *dlLabel;
    QLineEdit *downloadLoc;
    QToolButton *dirDialog;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *rbGood;
    QRadioButton *rbBest;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(407, 179);
        verticalLayout_3 = new QVBoxLayout(SettingsDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget = new QWidget(SettingsDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        dlLabel = new QLabel(widget);
        dlLabel->setObjectName(QString::fromUtf8("dlLabel"));

        horizontalLayout->addWidget(dlLabel);

        downloadLoc = new QLineEdit(widget);
        downloadLoc->setObjectName(QString::fromUtf8("downloadLoc"));

        horizontalLayout->addWidget(downloadLoc);

        dirDialog = new QToolButton(widget);
        dirDialog->setObjectName(QString::fromUtf8("dirDialog"));

        horizontalLayout->addWidget(dirDialog);


        verticalLayout_3->addWidget(widget);

        groupBox = new QGroupBox(SettingsDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        rbGood = new QRadioButton(groupBox);
        rbGood->setObjectName(QString::fromUtf8("rbGood"));

        verticalLayout_2->addWidget(rbGood);

        rbBest = new QRadioButton(groupBox);
        rbBest->setObjectName(QString::fromUtf8("rbBest"));
        rbBest->setChecked(true);

        verticalLayout_2->addWidget(rbBest);


        verticalLayout_3->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(SettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(SettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingsDialog, SLOT(reject()));
        QObject::connect(dirDialog, SIGNAL(clicked()), SettingsDialog, SLOT(dirDialog()));

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Settings", 0, QApplication::UnicodeUTF8));
        dlLabel->setText(QApplication::translate("SettingsDialog", "Download location", 0, QApplication::UnicodeUTF8));
        dirDialog->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SettingsDialog", "Quality", 0, QApplication::UnicodeUTF8));
        rbGood->setText(QApplication::translate("SettingsDialog", "Good", 0, QApplication::UnicodeUTF8));
        rbBest->setText(QApplication::translate("SettingsDialog", "Best", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H

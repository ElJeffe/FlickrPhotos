/********************************************************************************
** Form generated from reading UI file 'downloaddialog.ui'
**
** Created: Sat 16. Jan 12:19:51 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOADDIALOG_H
#define UI_DOWNLOADDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DownloadDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_3;
    QLabel *thumbImage;
    QSpacerItem *verticalSpacer_4;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout;
    QLabel *statusLabel;
    QProgressBar *photoProgress;
    QProgressBar *totalProgress;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DownloadDialog)
    {
        if (DownloadDialog->objectName().isEmpty())
            DownloadDialog->setObjectName(QString::fromUtf8("DownloadDialog"));
        DownloadDialog->resize(421, 169);
        verticalLayout_2 = new QVBoxLayout(DownloadDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widget = new QWidget(DownloadDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMaximumSize(QSize(100, 16777215));
        verticalLayout_3 = new QVBoxLayout(widget_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_3 = new QSpacerItem(20, 156, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        thumbImage = new QLabel(widget_2);
        thumbImage->setObjectName(QString::fromUtf8("thumbImage"));
        thumbImage->setMaximumSize(QSize(100, 16777215));

        verticalLayout_3->addWidget(thumbImage);

        verticalSpacer_4 = new QSpacerItem(20, 156, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);


        horizontalLayout->addWidget(widget_2);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        verticalLayout = new QVBoxLayout(widget_3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        statusLabel = new QLabel(widget_3);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setWordWrap(true);

        verticalLayout->addWidget(statusLabel);

        photoProgress = new QProgressBar(widget_3);
        photoProgress->setObjectName(QString::fromUtf8("photoProgress"));
        photoProgress->setValue(0);

        verticalLayout->addWidget(photoProgress);

        totalProgress = new QProgressBar(widget_3);
        totalProgress->setObjectName(QString::fromUtf8("totalProgress"));
        totalProgress->setValue(0);

        verticalLayout->addWidget(totalProgress);


        horizontalLayout->addWidget(widget_3);


        verticalLayout_2->addWidget(widget);

        buttonBox = new QDialogButtonBox(DownloadDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(DownloadDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DownloadDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DownloadDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DownloadDialog);
    } // setupUi

    void retranslateUi(QDialog *DownloadDialog)
    {
        DownloadDialog->setWindowTitle(QApplication::translate("DownloadDialog", "Downloading", 0, QApplication::UnicodeUTF8));
        thumbImage->setText(QString());
        statusLabel->setText(QString());
        totalProgress->setFormat(QApplication::translate("DownloadDialog", "%v/%m", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DownloadDialog: public Ui_DownloadDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOADDIALOG_H

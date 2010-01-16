/********************************************************************************
** Form generated from reading UI file 'flickrphotos.ui'
**
** Created: Sat 16. Jan 12:20:51 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLICKRPHOTOS_H
#define UI_FLICKRPHOTOS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FlickrPhotos
{
public:
    QAction *actionSettings;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *downloadWidget;
    QVBoxLayout *verticalLayout;
    QLabel *foundLabel;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *downloadButton;
    QPushButton *openButton;
    QPushButton *quitButton;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QMenu *menuSettings;

    void setupUi(QMainWindow *FlickrPhotos)
    {
        if (FlickrPhotos->objectName().isEmpty())
            FlickrPhotos->setObjectName(QString::fromUtf8("FlickrPhotos"));
        FlickrPhotos->resize(328, 153);
        actionSettings = new QAction(FlickrPhotos);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        centralWidget = new QWidget(FlickrPhotos);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        downloadWidget = new QWidget(centralWidget);
        downloadWidget->setObjectName(QString::fromUtf8("downloadWidget"));
        downloadWidget->setEnabled(true);
        verticalLayout = new QVBoxLayout(downloadWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        foundLabel = new QLabel(downloadWidget);
        foundLabel->setObjectName(QString::fromUtf8("foundLabel"));

        verticalLayout->addWidget(foundLabel);

        widget = new QWidget(downloadWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        downloadButton = new QPushButton(widget);
        downloadButton->setObjectName(QString::fromUtf8("downloadButton"));

        horizontalLayout_2->addWidget(downloadButton);

        openButton = new QPushButton(widget);
        openButton->setObjectName(QString::fromUtf8("openButton"));

        horizontalLayout_2->addWidget(openButton);

        quitButton = new QPushButton(widget);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));

        horizontalLayout_2->addWidget(quitButton);


        verticalLayout->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 274, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(downloadWidget);

        FlickrPhotos->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FlickrPhotos);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 328, 21));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        FlickrPhotos->setMenuBar(menuBar);

        menuBar->addAction(menuSettings->menuAction());
        menuSettings->addAction(actionSettings);

        retranslateUi(FlickrPhotos);
        QObject::connect(quitButton, SIGNAL(clicked()), FlickrPhotos, SLOT(close()));
        QObject::connect(downloadButton, SIGNAL(clicked()), FlickrPhotos, SLOT(downloadNew()));
        QObject::connect(actionSettings, SIGNAL(activated()), FlickrPhotos, SLOT(showSettings()));
        QObject::connect(openButton, SIGNAL(clicked()), FlickrPhotos, SLOT(openPhotos()));

        QMetaObject::connectSlotsByName(FlickrPhotos);
    } // setupUi

    void retranslateUi(QMainWindow *FlickrPhotos)
    {
        FlickrPhotos->setWindowTitle(QApplication::translate("FlickrPhotos", "FlickrPhotos", 0, QApplication::UnicodeUTF8));
        actionSettings->setText(QApplication::translate("FlickrPhotos", "Settings", 0, QApplication::UnicodeUTF8));
        foundLabel->setText(QApplication::translate("FlickrPhotos", "TextLabel", 0, QApplication::UnicodeUTF8));
        downloadButton->setText(QApplication::translate("FlickrPhotos", "Download", 0, QApplication::UnicodeUTF8));
        openButton->setText(QApplication::translate("FlickrPhotos", "Open photos", 0, QApplication::UnicodeUTF8));
        quitButton->setText(QApplication::translate("FlickrPhotos", "Quit", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("FlickrPhotos", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FlickrPhotos: public Ui_FlickrPhotos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLICKRPHOTOS_H

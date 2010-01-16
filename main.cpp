#include <QtGui/QApplication>
#include <QTranslator>
#include <QDebug>
#include "flickrphotos.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("FlickrFotos");
    a.setApplicationVersion("0.1");
    a.setOrganizationDomain("steelant.be");
    a.setOrganizationName("steelant.be");
    QTranslator translator;
    translator.load("flickrphotos");
    a.installTranslator(&translator);
    FlickrPhotos w;
    w.show();
    return a.exec();
}

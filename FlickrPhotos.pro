# -------------------------------------------------
# Project created by QtCreator 2010-01-09T15:02:07
# -------------------------------------------------
TARGET = FlickrPhotos
TEMPLATE = app
QT += network \
    xml
SOURCES += main.cpp \
    flickrphotos.cpp \
    qtflickr.cpp \
    collectioninfo.cpp \
    syncwaitdialog.cpp \
    downloaddialog.cpp \
    settingsdialog.cpp
HEADERS += flickrphotos.h \
    qtflickr.h \
    collectioninfo.h \
    syncwaitdialog.h \
    downloaddialog.h \
    settingsdialog.h \
    quality.h
FORMS += flickrphotos.ui \
    syncwaitdialog.ui \
    downloaddialog.ui \
    settingsdialog.ui
TRANSLATIONS = flickrphotos_nl.ts \
    flickrphotos_de.ts
OTHER_FILES += config.txt

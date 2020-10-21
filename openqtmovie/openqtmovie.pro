#-------------------------------------------------
#
# Project created by QtCreator 2020-09-16T14:33:20
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG -= app_bundle
TARGET = openqtmovie
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        ./view/mainwindow.cpp \
        ./model/normalAPI.cpp \
    view/addurldialog.cpp \
    application.cpp

HEADERS += \
        ./view/mainwindow.h \
        ./model/normalAPI.h \
    view/addurldialog.h \
    application.h

FORMS += \
        ./view/mainwindow.ui \
    view/addurldialog.ui

INCLUDEPATH += view \
               model
# Default rules for deployment.

isEmpty(PREFIX){
    PREFIX = /usr
}

target.path = /opt/$${TARGET}/bin

desktop.path = $$PREFIX/share/applications/
desktop.files = $$PWD/openqtmovie.desktop

icons.path = $$PREFIX/share/$${TARGET}/icon
icons.files = $$PWD/image/play.svg

dbus_service.path =  $$PREFIX/share/dbus-1/services
dbus_service.files += $$PWD/com.openqtmovie.service

#/opt/openqtmovie/setting/config.ini
#setting.path = /tmp/openqtmovie/
#setting.files += $$PWD/setting/config.ini

INSTALLS += target icons desktop dbus_service

DISTFILES += \
com.openQtMovie.service

RESOURCES += \
    view/qrc.qrc

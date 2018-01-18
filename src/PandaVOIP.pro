#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T18:09:42
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlackChat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    messagebox.cpp \
    pandavoip.cpp \
    opusutil.cpp \
    voipaudioiodevice.cpp \
    voiptcpsocket.cpp \
    voipudpsocket.cpp \
    voipcontroller.cpp \
    settings.cpp \
    account.cpp \
    menu.cpp

HEADERS += \
    messagebox.h \
    pandavoip.h \
    opusutil.h \
    voipaudioiodevice.h \
    voiptcpsocket.h \
    voipudpsocket.h \
    voipcontroller.h \
    settings.h \
    account.h \
    menu.h

FORMS += \
    pandavoip.ui \
    settings.ui \
    account.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    images/profileimage.jpg

unix|win32: LIBS += -lopus

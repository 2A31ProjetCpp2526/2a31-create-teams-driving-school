QT       += core gui sql printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -std=gnu++17
QT += multimedia texttospeech
QT += websockets
QT += network
# Encodage
DEFINES += QT_DEPRECATED_WARNINGS
QT += charts sql
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp
SOURCES += mainwindow.cpp
SOURCES += connection.cpp
SOURCES += seance.cpp
SOURCES += vehicule.cpp
SOURCES += circuit.cpp
SOURCES += client.cpp
SOURCES += login.cpp
SOURCES += Employe.cpp
SOURCES += moniteur.cpp
SOURCES += qrcode.cpp
SOURCES += clientmapwidget.cpp
SOURCES += notification.cpp
SOURCES += moniteurstat.cpp
SOURCES += voicechat.cpp
SOURCES += voicechatdialog.cpp

HEADERS += mainwindow.h
HEADERS += connection.h
HEADERS += seance.h
HEADERS += vehicule.h
HEADERS += circuit.h
HEADERS += client.h
HEADERS += login.h
HEADERS += Employe.h
HEADERS += moniteur.h
HEADERS += qrcode.h
HEADERS += clientmapwidget.h
HEADERS += notification.h
HEADERS += moniteurstat.h
HEADERS += voicechat.h
HEADERS += voicechatdialog.h

FORMS += mainwindow.ui
FORMS += login.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc

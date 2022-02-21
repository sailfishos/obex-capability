TEMPLATE = app
TARGET = obex-capability

QT += xml
QT -= gui
CONFIG += link_pkgconfig
PKGCONFIG += ssu
PKGCONFIG += systemsettings

HEADERS = obex-capability.h

SOURCES = obex-capability.cpp

target.path = /usr/bin
INSTALLS += target

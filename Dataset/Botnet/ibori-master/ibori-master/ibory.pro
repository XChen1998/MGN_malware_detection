QT += core
QT -= gui

CONFIG += c++11

TARGET = ibory
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += /root/work/winpcap/Include/
INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/include/cppconn
LIBS += -L/usr/lib/x86_64-linux-gnu -lmysqlcppconn
LIBS += -L/usr/lib/x86_64-linux-gnu -lpcap
LIBS += -L/usr/local/boost_1_62_0/stage/lib -lboost_regex

SOURCES += main.cpp \
    packet.cpp \
    dbconn.cpp \
    match.cpp

HEADERS += \
    packet.h \
    dbconn.h \
    match.h

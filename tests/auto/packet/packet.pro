TEMPLATE = app
TARGET = tst_packet

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

SOURCES +=  tst_packet.cpp

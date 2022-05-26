QT += widgets testlib
LIBS += -lpcp -lpcp_mmv

INCLUDEPATH += ../..

SOURCES = \
          testbattery.cpp \
          ../../Battery.cpp \
          ../../MyMetrics.cpp \
          ../../MySettings.cpp \

HEADERS = \
          ../../Battery.h \
          ../../MyMetrics.h \
          ../../MySettings.h \

CONFIG += debug

QT += widgets testlib
LIBS += -lpcp -lpcp_mmv

INCLUDEPATH += ../..
SOURCES = ../../Battery.cpp ../../MyMetrics.cpp testbattery.cpp
HEADERS = ../../Battery.h ../../MyMetrics.h


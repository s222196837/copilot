QT += positioning location network widgets testlib
LIBS += -lpcp -lpcp_mmv

INCLUDEPATH += ../..
SOURCES = ../../FlyingObjects.cpp ../../Transmitter.cpp ../../MyMetrics.cpp ../../MySettings.cpp testproximity.cpp
HEADERS = ../../FlyingObjects.h ../../Transmitter.h ../../MyMetrics.h ../../MySettings.h


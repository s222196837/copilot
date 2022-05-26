QT += positioning location network widgets testlib
LIBS += -lpcp -lpcp_mmv

INCLUDEPATH += ../..

SOURCES = \
	  testproximity.cpp \
	  ../../FlyingObjects.cpp \
	  ../../Transmitter.cpp \
	  ../../MyMetrics.cpp \
	  ../../MySettings.cpp \

HEADERS = \
	  ../../FlyingObjects.h \
	  ../../Transmitter.h \
	  ../../MyMetrics.h \
	  ../../MySettings.h \

CONFIG += debug

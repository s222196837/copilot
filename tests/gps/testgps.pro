QT += positioning location network widgets testlib
LIBS += -lpcp -lpcp_mmv

INCLUDEPATH += ../..

SOURCES = \
	  testgps.cpp \
	  ../../GPS.cpp \
	  ../../MyMetrics.cpp \
	  ../../MySettings.cpp \

HEADERS = \
	  ../../GPS.h \
	  ../../MyMetrics.h \
	  ../../MySettings.h \

CONFIG += debug

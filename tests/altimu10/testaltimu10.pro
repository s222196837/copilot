QT += widgets testlib
LIBS += -lpcp -lpcp_mmv

INCLUDEPATH += ../..

SOURCES = \
	  testaltimu10.cpp \
	  ../../Altimu10.cpp \
	  ../../MyMetrics.cpp \
	  ../../MySettings.cpp \

HEADERS = \
	  ../../Altimu10.h \
	  ../../MyMetrics.h \
	  ../../MySettings.h \

CONFIG += debug

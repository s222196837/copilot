QT += positioning location network widgets testlib
LIBS += -lpcp -lpcp_mmv

INCLUDEPATH += ../..
SOURCES = ../../GPS.cpp ../../MyMetrics.cpp testgps.cpp
HEADERS = ../../GPS.h ../../MyMetrics.h

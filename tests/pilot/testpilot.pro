TEMPLATE = app
TARGET = testpilot
QT += positioning location network
requires(qtConfig(udpsocket))
LIBS += -lpcp -lpcp_mmv

INCLUDEPATH += ../..

SOURCES += \
	testpilot.cpp \
	../../MyMetrics.cpp \
	../../MySettings.cpp \
	../../Transmitter.cpp \
	../../GPS.cpp \
	# END SOURCES

HEADERS += \
	../../MyMetrics.h \
	../../MySettings.h \
	../../Transmitter.h \
	../../GPS.h \
	# END HEADERS

target.path = $$[QT_INSTALL_EXAMPLES]/location/testpilot
INSTALLS += target

CONFIG += disable-desktop
CONFIG += debug

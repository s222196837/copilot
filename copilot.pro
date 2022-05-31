TEMPLATE = app
TARGET = copilot
QT += qml quick positioning location network svg
QTPLUGIN += qtvirtualkeyboardplugin
requires(qtConfig(udpsocket))
LIBS += -lpcp -lpcp_mmv

SOURCES += \
	main.cpp \
	MyMetrics.cpp \
	MySettings.cpp \
	FlyingObjects.cpp \
	Transmitter.cpp \
	Receiver.cpp \
	Altimu10.cpp \
	Battery.cpp \
	Buzzer.cpp \
	GPS.cpp \
	# END SOURCES

HEADERS += \
	MyMetrics.h \
	MySettings.h \
	FlyingObjects.h \
	Transmitter.h \
	Receiver.h \
	Altimu10.h \
	Battery.h \
	Buzzer.h \
	GPS.h \
	# END HEADERS

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/location/copilot
INSTALLS += target

CONFIG += disable-desktop
#CONFIG+=qml_debug
CONFIG += debug

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
	FlyingObject.cpp \
	Transponder.cpp \
	Altimu10.cpp \
	Battery.cpp \
	Buzzer.cpp \
	GPS.cpp \
	# END SOURCES

HEADERS += \
	MyMetrics.h \
	MySettings.h \
	FlyingObject.h \
	Transponder.h \
	Altimu10.h \
	Battery.h \
	Buzzer.h \
	GPS.h \
	# END HEADERS

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/location/copilot
INSTALLS += target

disable-xcb {
    message("The disable-xcb option has been deprecated. Please use disable-desktop instead.")
    CONFIG += disable-desktop
}

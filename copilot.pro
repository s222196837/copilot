TEMPLATE = app
TARGET = copilot
QT += qml quick positioning location network svg
QTPLUGIN += qtvirtualkeyboardplugin
requires(qtConfig(udpsocket))

SOURCES += \
	main.cpp \
	FlyingObject.cpp \
	Transponder.cpp \
	Altimu10.cpp \
	GPS.cpp \
	# END SOURCES

HEADERS += \
	FlyingObject.h \
	Transponder.h \
	Altimu10.h \
	GPS.h \
	# END HEADERS

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/location/copilot
INSTALLS += target

disable-xcb {
    message("The disable-xcb option has been deprecated. Please use disable-desktop instead.")
    CONFIG += disable-desktop
}

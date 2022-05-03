TEMPLATE = app
TARGET = copilot
QT += qml quick positioning location network svg
QTPLUGIN += qtvirtualkeyboardplugin
requires(qtConfig(udpsocket))

SOURCES += \
	main.cpp \
	transponder.cpp \
	# END SOURCES

HEADERS += \
	transponder.h \
	# END HEADERS

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/location/copilot
INSTALLS += target

disable-xcb {
    message("The disable-xcb option has been deprecated. Please use disable-desktop instead.")
    CONFIG += disable-desktop
}

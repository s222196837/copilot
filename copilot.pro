TEMPLATE = app
TARGET = copilot
QT += qml quick positioning location network
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

TEMPLATE = app
TARGET = copilot
QT += qml quick positioning location

SOURCES += main.cpp

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/location/copilot
INSTALLS += target

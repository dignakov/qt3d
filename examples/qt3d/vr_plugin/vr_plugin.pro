!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

TEMPLATE=lib

QT += 3dcore 3drender 3dinput 3dquick 3dlogic qml quick 3dquickextras

SOURCES += \
    plugin.cpp

HEADERS += \
    plugin.h

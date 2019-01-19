!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

###QUESTION: how do I get these headers as part of QT
INCLUDEPATH += $$PWD/../../../src/render/vr_plugin_interface/include

TEMPLATE = lib
CONFIG += plugin

QT += 3dcore 3drender 3dinput 3dquick 3dlogic qml quick 3dquickextras 3dcore-private 3drender-private

SOURCES += \
    plugin.cpp

HEADERS += \
    plugin.h

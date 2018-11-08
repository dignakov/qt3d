!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

###QUESTION: how do I get these headers as part of QT
INCLUDEPATH += $$PWD/../../../src/render/vr_plugin_interface/include

TEMPLATE=lib

QT += 3dcore 3drender 3dinput 3dquick 3dlogic qml quick 3dquickextras 3dcore-private 3drender-private

SOURCES += \
    openvr_plugin.cpp

HEADERS += \
    openvr_plugin.h


win32: {
  LIBS += -L$$PWD/../../../../../openvr/lib/win64/ -lopenvr_api
  INCLUDEPATH += $$PWD/../../../../../openvr/headers
  DEPENDPATH += $$PWD/../../../../../openvr/headers
}
unix: {
  LIBS += -L$$PWD/../../../../openvr/lib/linux64/ -lopenvr_api
  INCLUDEPATH += $$PWD/../../../../openvr/headers
  DEPENDPATH += $$PWD/../../../../openvr/headers
}

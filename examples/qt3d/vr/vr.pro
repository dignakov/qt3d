!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

QT += 3dcore 3drender 3dinput 3dquick 3dlogic qml quick 3dquickextras 3dcore-private 3drender-private

SOURCES += \
    main.cpp

#HEADERS += \
#    testvrdevice_p.h \
#    qtestvrdevice.h

OTHER_FILES += \
    main.qml

RESOURCES += \
    vr.qrc

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qopenvrdevice.h \
    $$PWD/openvrdevice_p.h \
    $$PWD/qopenvrsubmitcommand.h \
    $$PWD/openvrsubmitcommand_p.h

SOURCES += \
    $$PWD/qopenvrdevice.cpp \
    $$PWD/openvrdevice.cpp \
    $$PWD/qopenvrsubmitcommand.cpp \
    $$PWD/openvrsubmitcommand.cpp


#TO DO: Make nicer and cross platform
#INCLUDEPATH += C:/Users/kdab/work/openvr/headers
##LIBS += -LC:/Users/kdab/work/openvr/lib/win64 -lopenvr_api
#LIBS += -LC:\Users\kdab\work\openvr\bin\win64\Debug -lopenvr_api64


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


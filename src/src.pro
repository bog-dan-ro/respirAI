TARGET = respirAI
DESTDIR = $$PWD/..

QT += quick charts serialport

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS

DUMMY_DATA: DEFINES += DUMMY_DATA

SOURCES += \
        alarmsmanager.cpp \
        main.cpp \
        packet.cpp \
        sensorsdatamanager.cpp \
        serialmanager.cpp \
        startupmanager.cpp

HEADERS += \
        alarmsmanager.h \
        packet.h \
        sensorsdatamanager.h \
        serialmanager.h \
        startupmanager.h

RESOURCES += \
        qml/qml.qrc

TRANSLATIONS += \
        translations/respirAI_ro_RO.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

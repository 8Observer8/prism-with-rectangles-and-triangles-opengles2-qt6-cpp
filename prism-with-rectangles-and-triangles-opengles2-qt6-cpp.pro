QT += core gui opengl widgets

win32: LIBS += -lopengl32

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    opengl_window.cpp

HEADERS += \
    opengl_window.h

RESOURCES += \
    assets.qrc

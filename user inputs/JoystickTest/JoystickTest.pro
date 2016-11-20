#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T18:52:40
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JoystickTest
TEMPLATE = app

# Add SFML include directory, where SFML headers are located
INCLUDEPATH += /home/joe/Documents/SFML-2.4.0/include

# Add SFML library directory to search path
LIBS += -L"/home/joe/Documents/SFML-2.4.0/lib"

# Add SFML window library, where Joystick is located
LIBS += -lsfml-window
#LIBS += -lsfml-system

SOURCES += main.cpp\
        mainwindow.cpp \
    xinputgamepad.cpp

HEADERS  += mainwindow.h \
    xinputGamepad.h

FORMS    += mainwindow.ui

DISTFILES += \
    ../../Downloads/Up Arrow.png \
    ../../Downloads/Up Arrow.png

RESOURCES += \
    resbuttonicons.qrc

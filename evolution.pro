QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animal.cpp \
    animalcreator.cpp \
    brain.cpp \
    camera.cpp \
    canvas.cpp \
    food.cpp \
    genetics.cpp \
    grid.cpp \
    main.cpp \
    mainwindow.cpp \
    object.cpp \
    simulation.cpp

HEADERS += \
    animal.hpp \
    animalcreator.hpp \
    brain.hpp \
    camera.h \
    canvas.h \
    food.hpp \
    genetics.hpp \
    grid.hpp \
    mainwindow.hpp \
    object.hpp \
    simulation.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

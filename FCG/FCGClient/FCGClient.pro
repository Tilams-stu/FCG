QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/gamecontroller.cpp \
    main.cpp \
    mainview.cpp \
    model/gamemodel.cpp \
    model/gamestate.cpp \
    model/plane.cpp \
    view/boardpanel.cpp \
    view/connectdialog.cpp \
    view/controlpanel.cpp

HEADERS += \
    controller/gamecontroller.h \
    mainview.h \
    model/gamemodel.h \
    model/gamestate.h \
    model/plane.h \
    view/boardpanel.h \
    view/connectdialog.h \
    view/controlpanel.h

FORMS += \
    mainview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

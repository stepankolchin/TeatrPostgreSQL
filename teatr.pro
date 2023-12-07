QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addshowdialogue.cpp \
    basket_view.cpp \
    deleteshowdialogue.cpp \
    labelclicker.cpp \
    main.cpp \
    mainwindow.cpp \
    mybutton.cpp \
    profile.cpp \
    schema_zal.cpp \
    vhod_register.cpp

HEADERS += \
    addshowdialogue.h \
    basket_view.h \
    deleteshowdialogue.h \
    labelclicker.h \
    mainwindow.h \
    mybutton.h \
    profile.h \
    schema_zal.h \
    vhod_register.h

FORMS += \
    addshowdialogue.ui \
    basket_view.ui \
    deleteshowdialogue.ui \
    mainwindow.ui \
    profile.ui \
    schema_zal.ui \
    vhod_register.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

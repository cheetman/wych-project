QT       += core gui network
QT       += multimedia
QT       += multimediawidgets
QT       += xml
QT       += concurrent


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    form.cpp \
    itemview7.cpp \
    itemview7player.cpp \
    itemview7vlcplayer.cpp \
    itemview8.cpp \
    main.cpp \
    mainwindow.cpp \
    qftp.cpp \
    qurlinfo.cpp \
    utils.cpp \
    vlclib.cpp

HEADERS += \
    form.h \
    itemview7.h \
    itemview7player.h \
    itemview7vlcplayer.h \
    itemview8.h \
    mainwindow.h \
    qftp.h \
    qurlinfo.h \
    utils.h \
    vlclib.h

FORMS +=


win32:CONFIG(release, debug|release) {
    # WINDOWS RELEASE
    PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/release/libqwebdav.a
    INCLUDEPATH += $$OUT_PWD/../qwebdavlib/
    LIBS += -L$$OUT_PWD/../qwebdavlib/release/ -lqwebdav
} else:win32:CONFIG(debug, debug|release) {
    # WINDOWS DEBUG
    PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/debug/libqwebdav.a
    INCLUDEPATH += $$PWD/../qwebdavlib/
    LIBS += -L$$OUT_PWD/../qwebdavlib/debug/ -lqwebdav
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

VLCPATH=C:/WorkEnv/vlc-3.0.16
INCLUDEPATH += $$VLCPATH/sdk/include
LIBS += -L$$VLCPATH/sdk/lib -llibvlc -llibvlccore

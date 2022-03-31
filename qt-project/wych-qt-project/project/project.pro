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
#    itemview7.cpp \
    itemview7player.cpp \
    itemview7vlcplayer.cpp \
    itemview8.cpp \
    itemview9.cpp \
    main.cpp \
    mainwindow.cpp \
#    qftp.cpp \
    qurlinfo.cpp \
    selectdialog.cpp \
    utils.cpp \
    vlclib.cpp

HEADERS += \
#    itemview7.h \
    itemview7player.h \
    itemview7vlcplayer.h \
    itemview8.h \
    itemview9.h \
    mainwindow.h \
#    qftp.h \
    qurlinfo.h \
    selectdialog.h \
    test2.h \
    utils.h \
    vlclib.h

FORMS +=


win32:CONFIG(release, debug|release) {
    # webdav
    msvc:PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/release/qwebdav.lib
    mingw:PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/release/libqwebdav.a
    INCLUDEPATH += $$PWD/../qwebdavlib/
    LIBS += -L$$OUT_PWD/../qwebdavlib/release/ -lqwebdav

    # libuv
    msvc:PRE_TARGETDEPS += $$OUT_PWD/../libuv/release/libuv.lib
    mingw:PRE_TARGETDEPS += $$OUT_PWD/../libuv/release/liblibuv.a
    INCLUDEPATH += $$PWD/../libuv/
    LIBS += -L$$OUT_PWD/../libuv/release/ -llibuv


    #Winsock
    LIBS += -lws2_32
    LIBS += -lkernel32
    #线程管理
    LIBS += -lntdll
    LIBS += -luser32


    # libuv中用到的也要加
    LIBS += -lUserEnv
    LIBS += -lIPHLPAPI
    LIBS += -lPsapi
    LIBS += -ladvapi32
    LIBS += -lGdi32



    greaterThan(QT_MAJOR_VERSION, 4) {
        TARGET_ARCH=$${QT_ARCH}
    } else {
        TARGET_ARCH=$${QMAKE_HOST.arch}
    }

    contains(TARGET_ARCH, x86_64) {
        message("64-bit")
        VLCPATH=C:/WorkEnv/vlc-3.0.0-win64
        INCLUDEPATH += $$VLCPATH/sdk/include
        LIBS += -L$$VLCPATH/sdk/lib -llibvlc -llibvlccore
    } else {
        message("32-bit")
        VLCPATH=C:/WorkEnv/vlc-3.0.0
        INCLUDEPATH += $$VLCPATH/sdk/include
        LIBS += -L$$VLCPATH/sdk/lib -llibvlc -llibvlccore
    }


} else:win32:CONFIG(debug, debug|release) {
    # WINDOWS DEBUG
    msvc:PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/debug/qwebdav.lib
    mingw:PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/debug/libqwebdav.a
    INCLUDEPATH += $$PWD/../qwebdavlib/
    LIBS += -L$$OUT_PWD/../qwebdavlib/debug/ -lqwebdav

    # libuv
    msvc:PRE_TARGETDEPS += $$OUT_PWD/../libuv/debug/libuv.lib
    mingw:PRE_TARGETDEPS += $$OUT_PWD/../libuv/debug/liblibuv.a
    INCLUDEPATH += $$PWD/../libuv/
    LIBS += -L$$OUT_PWD/../libuv/debug/ -llibuv



    #Winsock
    LIBS += -lws2_32
    LIBS += -lkernel32
    #线程管理
    LIBS += -lntdll
    LIBS += -luser32

    # libuv中用到的也要加
    LIBS += -lUserEnv
    LIBS += -lIPHLPAPI
    LIBS += -lPsapi
    LIBS += -ladvapi32
    LIBS += -lGdi32



    greaterThan(QT_MAJOR_VERSION, 4) {
        TARGET_ARCH=$${QT_ARCH}
    } else {
        TARGET_ARCH=$${QMAKE_HOST.arch}
    }

    contains(TARGET_ARCH, x86_64) {
        message("64-bit")
        VLCPATH=C:/WorkEnv/vlc-3.0.0-win64
        INCLUDEPATH += $$VLCPATH/sdk/include
        LIBS += -L$$VLCPATH/sdk/lib -llibvlc -llibvlccore
    } else {
        message("32-bit")
        VLCPATH=C:/WorkEnv/vlc-3.0.0
        INCLUDEPATH += $$VLCPATH/sdk/include
        LIBS += -L$$VLCPATH/sdk/lib -llibvlc -llibvlccore
    }

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8



RC_ICONS = ./favicon.ico

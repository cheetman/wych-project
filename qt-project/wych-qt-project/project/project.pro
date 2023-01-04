QT       += core gui network
QT       += multimedia
QT       += multimediawidgets
QT       += xml
QT       += concurrent
QT       += winextras


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += HV_WITHOUT_SSL
DEFINES += HV_STATICLIB
DEFINES += WIN32
DEFINES += _WINDOWS
#DEFINES += _WIN32_WINNT=0x0600
DEFINES += _WIN32_WINNT=0x0A00

DEFINES += NDEBUG


MAKE_LFLAGS += "/STACK:65536,4096"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    itemview7.cpp \
    components/capturebtn.cpp \
    components/pixmapwidget.cpp \
    components/scriptchecktypedialog.cpp \
    components/scripttypedialog.cpp \
    components/selectdialog.cpp \
    itemview10.cpp \
    itemview10Inject.cpp \
    itemview10ProcessStatus.cpp \
    itemview10Script.cpp \
    itemview10pe.cpp \
    itemview11.cpp \
    itemview11cmd.cpp \
    itemview6.cpp \
    itemview6tcp.cpp \
    itemview6tcpc.cpp \
    itemview6tcphv.cpp \
    itemview7player.cpp \
    itemview7vlcplayer.cpp \
    itemview8.cpp \
    itemview9.cpp \
    itemview9cs16.cpp \
    itemview9csgo.cpp \
    itemview9css.cpp \
    itemview9l4d2.cpp \
    main.cpp \
    mainwindow.cpp \
#    qftp.cpp \
    structs/pe.cpp \
    qurlinfo.cpp \
    tcpserver.cpp \
    threads/threadscript.cpp \
    threads/threadwindowhide.cpp \
    utils.cpp \
    vlclib.cpp \
    winapi.cpp \
    window.cpp

HEADERS += \
#    itemview7.h \
    components/capturebtn.h \
    components/pixmapwidget.h \
    components/scriptchecktypedialog.h \
    components/scripttypedialog.h \
    components/selectdialog.h \
    events/customevent.h \
    events/eventstatusgrid.h \
    events/eventwinmessage.h \
    itemview10Inject.h \
    itemview10.h \
    itemview10ProcessStatus.h \
    itemview10Script.h \
    itemview10pe.h \
    itemview11.h \
    itemview11cmd.h \
    itemview6.h \
    itemview6tcp.h \
    itemview6tcpc.h \
    itemview6tcphv.h \
    itemview7player.h \
    itemview7vlcplayer.h \
    itemview8.h \
    itemview9.h \
    itemview9cs16.h \
    itemview9csgo.h \
    itemview9css.h \
    itemview9l4d2.h \
    mainwindow.h \
#    qftp.h \
    structs/pe.h \
    qurlinfo.h \
    tcpserver.h \
    test2.h \
    threads/threadscript.h \
    threads/threadwindowhide.h \
    utils.h \
    vlclib.h \
    winapi.h \
    window.h \
    configs/csgo.hpp

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

    # libhv
    msvc:PRE_TARGETDEPS += $$OUT_PWD/../libhv/release/libhv.lib
    mingw:PRE_TARGETDEPS += $$OUT_PWD/../libhv/release/liblibhv.a
    INCLUDEPATH += $$PWD/../libhv/
    LIBS += -L$$OUT_PWD/../libhv/release/ -llibhv


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

    # D3D用到
    LIBS += -ldwmapi

    # OpenCV
    INCLUDEPATH += C:/WorkMe/opencv/build/include/
    INCLUDEPATH += C:/WorkMe/opencv/build/include/opencv/
    INCLUDEPATH += C:/WorkMe/opencv/build/include/opencv2/
    # OpenCV(VS2019)
    LIBS += -LC:/WorkMe/opencv_build/lib/Release \
    -lopencv_aruco342 \
    -lopencv_bgsegm342 \
    -lopencv_bioinspired342 \
    -lopencv_calib3d342 \
    -lopencv_ccalib342 \
    -lopencv_core342 \
    -lopencv_datasets342 \
    -lopencv_dnn342 \
    -lopencv_dnn_objdetect342 \
    -lopencv_dpm342 \
    -lopencv_face342 \
    -lopencv_features2d342 \
    -lopencv_flann342 \
    -lopencv_fuzzy342 \
    -lopencv_hfs342 \
    -lopencv_highgui342 \
    -lopencv_imgcodecs342 \
    -lopencv_imgproc342 \
    -lopencv_img_hash342 \
    -lopencv_line_descriptor342 \
    -lopencv_ml342 \
    -lopencv_objdetect342 \
    -lopencv_optflow342 \
    -lopencv_phase_unwrapping342 \
    -lopencv_photo342 \
    -lopencv_plot342 \
    -lopencv_reg342 \
    -lopencv_rgbd342 \
    -lopencv_saliency342 \
    -lopencv_shape342 \
    -lopencv_stereo342 \
    -lopencv_stitching342 \
    -lopencv_structured_light342 \
    -lopencv_superres342 \
    -lopencv_surface_matching342 \
    -lopencv_text342 \
    -lopencv_tracking342 \
    -lopencv_video342 \
    -lopencv_videoio342 \
    -lopencv_videostab342 \
    -lopencv_xfeatures2d342 \
    -lopencv_ximgproc342 \
    -lopencv_xobjdetect342 \
    -lopencv_xphoto342

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
        # d3d9
        D3DPATH="C:/WorkEnv/Microsoft DirectX SDK (June 2010)"
        msvc:PRE_TARGETDEPS += $$D3DPATH/Lib/x64/d3d9.lib
        msvc:PRE_TARGETDEPS += $$D3DPATH/Lib/x64/d3dx9.lib
        INCLUDEPATH += $$D3DPATH/Include
        LIBS +=  -L$$D3DPATH/Lib/x64/ -ld3d9 -ld3dx9
    } else {
        message("32-bit")
        VLCPATH=C:/WorkEnv/vlc-3.0.0
        INCLUDEPATH += $$VLCPATH/sdk/include
        LIBS += -L$$VLCPATH/sdk/lib -llibvlc -llibvlccore

        # d3d9
        D3DPATH="C:/WorkEnv/Microsoft DirectX SDK (June 2010)"
        msvc:PRE_TARGETDEPS += $$D3DPATH/Lib/x86/d3d9.lib
        msvc:PRE_TARGETDEPS += $$D3DPATH/Lib/x86/d3dx9.lib
        INCLUDEPATH += $$D3DPATH/Include
        LIBS +=  -L$$D3DPATH/Lib/x86/ -ld3d9 -ld3dx9
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

    # libhv
    msvc:PRE_TARGETDEPS += $$OUT_PWD/../libhv/debug/libhv.lib
    mingw:PRE_TARGETDEPS += $$OUT_PWD/../libhv/debug/liblibhv.a
    INCLUDEPATH += $$PWD/../libhv/
    LIBS += -L$$OUT_PWD/../libhv/debug/ -llibhv


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

    # D3D用到
    LIBS += -ldwmapi

    # OpenCV
    INCLUDEPATH += C:/WorkMe/opencv/build/include/
    INCLUDEPATH += C:/WorkMe/opencv/build/include/opencv/
    INCLUDEPATH += C:/WorkMe/opencv/build/include/opencv2/
    # OpenCV(VS2019)
    LIBS += -LC:/WorkMe/opencv_build/lib/Debug \
    -lopencv_aruco342d \
    -lopencv_bgsegm342d \
    -lopencv_bioinspired342d \
    -lopencv_calib3d342d \
    -lopencv_ccalib342d \
    -lopencv_core342d \
    -lopencv_datasets342d \
    -lopencv_dnn342d \
    -lopencv_dnn_objdetect342d \
    -lopencv_dpm342d \
    -lopencv_face342d \
    -lopencv_features2d342d \
    -lopencv_flann342d \
    -lopencv_fuzzy342d \
    -lopencv_hfs342d \
    -lopencv_highgui342d \
    -lopencv_imgcodecs342d \
    -lopencv_imgproc342d \
    -lopencv_img_hash342d \
    -lopencv_line_descriptor342d \
    -lopencv_ml342d \
    -lopencv_objdetect342d \
    -lopencv_optflow342d \
    -lopencv_phase_unwrapping342d \
    -lopencv_photo342d \
    -lopencv_plot342d \
    -lopencv_reg342d \
    -lopencv_rgbd342d \
    -lopencv_saliency342d \
    -lopencv_shape342d \
    -lopencv_stereo342d \
    -lopencv_stitching342d \
    -lopencv_structured_light342d \
    -lopencv_superres342d \
    -lopencv_surface_matching342d \
    -lopencv_text342d \
    -lopencv_tracking342d \
    -lopencv_video342d \
    -lopencv_videoio342d \
    -lopencv_videostab342d \
    -lopencv_xfeatures2d342d \
    -lopencv_ximgproc342d \
    -lopencv_xobjdetect342d \
    -lopencv_xphoto342d


    greaterThan(QT_MAJOR_VERSION, 4) {
        TARGET_ARCH=$${QT_ARCH}
    } else {
        TARGET_ARCH=$${QMAKE_HOST.arch}
    }

    contains(TARGET_ARCH, x86_64) {
        message("64-bit")
        # vlc
        VLCPATH=C:/WorkEnv/vlc-3.0.0-win64
        INCLUDEPATH += $$VLCPATH/sdk/include
        LIBS += -L$$VLCPATH/sdk/lib -llibvlc -llibvlccore

        # d3d9
        D3DPATH="C:/WorkEnv/Microsoft DirectX SDK (June 2010)"
        msvc:PRE_TARGETDEPS += $$D3DPATH/Lib/x64/d3d9.lib
        msvc:PRE_TARGETDEPS += $$D3DPATH/Lib/x64/d3dx9.lib
        INCLUDEPATH += $$D3DPATH/Include
        LIBS +=  -L$$D3DPATH/Lib/x64/ -ld3d9 -ld3dx9
    } else {
        message("32-bit")
        VLCPATH=C:/WorkEnv/vlc-3.0.0
        INCLUDEPATH += $$VLCPATH/sdk/include
        LIBS += -L$$VLCPATH/sdk/lib -llibvlc -llibvlccore

# d3d9
D3DPATH="C:/WorkEnv/Microsoft DirectX SDK (June 2010)"
msvc:PRE_TARGETDEPS += $$D3DPATH/Lib/x86/d3d9.lib
msvc:PRE_TARGETDEPS += $$D3DPATH/Lib/x86/d3dx9.lib
INCLUDEPATH += $$D3DPATH/Include
LIBS +=  -L$$D3DPATH/Lib/x86/ -ld3d9 -ld3dx9
    }

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8



RC_ICONS = ./favicon.ico

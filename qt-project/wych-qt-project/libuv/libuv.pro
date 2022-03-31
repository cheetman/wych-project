CONFIG -= qt

# for a qwebdavlib without support for GUI
#QT       -= gui

TARGET = libuv
TEMPLATE = lib
CONFIG += staticlib

#CONFIG += c++17




SOURCES += \
    timer.c \
    fs-poll.c \
    idna.c \
    inet.c \
    random.c \
    strscpy.c \
    threadpool.c \
    uv-common.c \
    uv-data-getter-setters.c \
    version.c

HEADERS += \
    heap-inl.h \
    idna.h \
    queue.h \
    strscpy.h \
    uv-common.h \
    uv/aix.h \
    uv/bsd.h \
    uv/darwin.h \
    uv/errno.h \
    uv/linux.h \
    uv/os390.h \
    uv/posix.h \
    uv/stdint-msvc2008.h \
    uv/sunos.h \
    uv/threadpool.h \
    uv/tree.h \
    uv/unix.h \
    uv/version.h \
    uv/win.h \
    uv.h



win32{


SOURCES += \
    win/async.c \
    win/core.c \
    win/detect-wakeup.c \
    win/dl.c \
    win/error.c \
    win/fs.c \
    win/fs-event.c \
    win/getaddrinfo.c \
    win/getnameinfo.c \
    win/handle.c \
    win/loop-watcher.c \
    win/pipe.c \
    win/poll.c \
    win/process.c \
    win/process-stdio.c \
    win/signal.c \
    win/snprintf.c \
    win/stream.c \
    win/tcp.c \
    win/thread.c \
    win/tty.c \
    win/udp.c \
    win/util.c \
    win/winapi.c \
    win/winsock.c



HEADERS += \
    win/atomicops-inl.h \
    win/fs-fd-hash-inl.h \
    win/handle-inl.h \
    win/internal.h \
    win/req-inl.h \
    win/stream-inl.h \
    win/winapi.h \
    win/winsock.h


LIBS += -lws2_32
LIBS += -lUserEnv
LIBS += -lIPHLPAPI
LIBS += -lPsapi
LIBS += -ladvapi32
LIBS += -lUser32
LIBS += -lGdi32

}



unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target


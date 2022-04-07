CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += HV_WITHOUT_SSL
DEFINES += HV_STATICLIB
DEFINES += WIN32
DEFINES += _WINDOWS
DEFINES += _WIN32_WINNT=0x0600
DEFINES += NDEBUG

SOURCES += \
    libhv.cpp \
    base/rbtree.c \
    base/hbase.c \
    base/herr.c \
    base/hlog.c \
    base/hmain.c \
    base/hsocket.c \
    base/htime.c \
    base/hversion.c \
    util/sha1.c \
    util/base64.c \
    util/md5.c \
    cpputil/ThreadLocalStorage.cpp \
    cpputil/hasync.cpp \
    cpputil/hdir.cpp \
    cpputil/hpath.cpp \
    cpputil/hstring.cpp \
    cpputil/hurl.cpp \
    cpputil/iniparser.cpp \
    cpputil/RAII.cpp \
    protocol/smtp.c \
    protocol/dns.c \
    protocol/ftp.c \
    protocol/icmp.c \
    event/unpack.c \
    event/epoll.c \
    event/evport.c \
    event/hevent.c \
    event/hloop.c \
    event/iocp.c \
    event/kqueue.c \
    event/nio.c \
    event/nlog.c \
    event/noevent.c \
    event/overlapio.c \
    event/poll.c \
    event/rudp.c \
    event/select.c \
    ssl/openssl.c \
    ssl/wintls.c \
    ssl/appletls.c \
    ssl/gnutls.c \
    ssl/hssl.c \
    ssl/mbedtls.c \
    ssl/nossl.c \





HEADERS += \
    libhv.h \
    base/array.h \
    base/hatomic.h \
    base/hbase.h \
    base/hbuf.h \
    base/hdef.h \
    base/heap.h \
    base/hendian.h \
    base/rbtree.h \
    base/herr.h \
    base/hlog.h \
    base/hmain.h \
    base/hmath.h \
    base/hmutex.h \
    base/hplatform.h \
    base/hproc.h \
    base/hsocket.h \
    base/hsysinfo.h \
    base/hthread.h \
    base/htime.h \
    base/hversion.h \
    base/list.h \
    base/netinet.h \
    base/queue.h \
    util/base64.h \
    util/md5.h \
    util/sha1.h \
    cpputil/ThreadLocalStorage.h \
    cpputil/hasync.h \
    cpputil/hdir.h \
    cpputil/hfile.h \
    cpputil/hmap.h \
    cpputil/hobjectpool.h \
    cpputil/hpath.h \
    cpputil/hscope.h \
    cpputil/hstring.h \
    cpputil/hthreadpool.h \
    cpputil/hurl.h \
    cpputil/ifconfig.h \
    cpputil/iniparser.h \
    cpputil/json.hpp \
    cpputil/singleton.h \
    protocol/dns.h \
    protocol/ftp.h \
    protocol/icmp.h \
    protocol/smtp.h \
    hv.h \
    hconfig.h \
    hexport.h \
    evpp/UdpServer.h \
    evpp/Buffer.h \
    evpp/Channel.h \
    evpp/Event.h \
    evpp/EventLoop.h \
    evpp/EventLoopThread.h \
    evpp/EventLoopThreadPool.h \
    evpp/Status.h \
    evpp/TcpClient.h \
    evpp/TcpServer.h \
    evpp/TimerThread.h \
    evpp/UdpClient.h \
    event/unpack.h \
    event/hevent.h \
    event/hloop.h \
    event/iowatcher.h \
    event/nlog.h \
    event/overlapio.h \
    event/rudp.h \
    ssl/hssl.h \


LIBS += -lws2_32
LIBS += -lkernel32


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

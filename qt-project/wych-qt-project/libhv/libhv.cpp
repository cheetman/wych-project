#include "libhv.h"
#include "evpp/TcpServer.h"

Libhv::Libhv()
{
}


bool Libhv::start(int port, const char* host)
{
    auto server = new hv::TcpServer;
    int listenfd = server->createsocket(port, host);
    if (listenfd < 0) {
        return false;
    }
    server->setThreadNum(1);
    server->onConnection = [](const hv::SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
//            g_mainwnd->postMessage(QString::asprintf("%s connected! connfd=%d", peeraddr.c_str(), channel->fd()));
        } else {
//            g_mainwnd->postMessage(QString::asprintf("%s disconnected! connfd=%d", peeraddr.c_str(), channel->fd()));
        }
    };
    server->onMessage = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
//        g_mainwnd->postMessage(QString::asprintf("< %.*s", (int)buf->size(), (char*)buf->data()));
        // echo
        channel->write(buf);
    };
    server->start();
    return true;
}

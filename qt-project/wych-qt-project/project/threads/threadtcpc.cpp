#include "threadtcpc.h"
#include <QDebug>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>


#include <winsock2.h>
#include <WS2tcpip.h>
#include <QApplication>

#include "itemview6tcpc.h"

#include <events/customevent.h>
#include <events/eventtcpcgrid.h>

ThreadTcpC::ThreadTcpC(QObject *parent)
    : QThread{parent}
{}


void ThreadTcpC::setParent(ItemView6TcpC *parent) {
    this->parent = parent;
}

void ThreadTcpC::run()
{
    // Windows 必须先执行WSAStartup ，否则socket构造时会报错
    WSADATA wsadata;
    int     Ret;

    if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0) {
        // 注意，因为winsock没有加载，不能使用WSAGetLastError来确定导致故障的特定错误，但可以根据返回值确定。
        parent->postAppendConsole(tr("WSAStartup error RetCode: %1!").arg(Ret));
        return;
    }


    int  sockfd, n;
    char recvline[200 + 1];

    // struct sockaddr servaddr; 这个结构没有把ip与端口分开
    // struct sockaddr_in servaddr;
    sockaddr_in servaddr;

    // IPPROTO_TCP = 6
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        parent->postAppendConsole(tr("socket error RetCode: %1!").arg(sockfd));
        return;
    }

    // bzero(&servaddr, sizeof(servaddr)); windows 下没有bzero函数
    ZeroMemory(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);


    wchar_t ip_wchar[20] = { 0 };
    ip.toWCharArray(ip_wchar);

    // inet_pton UNIX下
    if (InetPton(AF_INET, ip_wchar, &servaddr.sin_addr) <= 0) {
        parent->postAppendConsole(tr("InetPton error !"));
        return;
    }


    // 这里强转sockaddr * 好像历史遗留问题
    if (::connect(sockfd, (PSOCKADDR)&servaddr, sizeof(servaddr)) < 0) {
        //        QStringEvent *event = new QStringEvent(tr("connect error code:%1!").arg(WSAGetLastError()), qEventAppendConsole);
        //        QApplication::postEvent(parent, event);

        parent->postAppendConsole(tr("connect error code:%1!").arg(WSAGetLastError()));
        return;
    }


    parent->postAppendConsole(tr("服务器连接成功! [id:%1]").arg(sockfd));

    // 推送
    QApplication::postEvent(parent, new EventTcpCGrid(sockfd, qEventTcpCAdd));

    while ((n = recv(sockfd, recvline, 200, 0)) > 0) {
        //        recvline[n] = 0;

        QString data = tr("[%1] ").arg(sockfd);

        if (parent->cbCharEncoding->currentIndex() == 0) {
            data +=  QString::fromUtf8(recvline, n);
        } else {
            data +=  QString::fromLocal8Bit(recvline, n);
        }

        parent->postAppendRecvMsg(data);


        //        if (fputs(recvline, stdout) == EOF) {
        //            std::cout << "error!\n";
        //        }
    }

    if (n < 0) {
        parent->postAppendConsole(tr("read error [id:%1]"));
    }


    parent->postAppendConsole(tr("服务器连接断开! [id:%1]").arg(sockfd));

    // 推送
    QApplication::postEvent(parent, new EventTcpCGrid(sockfd, qEventTcpCRemove));

    emit threadFinished();
}

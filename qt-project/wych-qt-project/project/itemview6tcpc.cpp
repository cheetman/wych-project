#include "itemview6tcpc.h"

#include "mainwindow.h"
#include "events/customevent.h"

#include <QComboBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QStandardItemModel>
#include <QTableView>
#include <QTextCodec>
#include <QTimer>

#include <winsock2.h>
#include <WS2tcpip.h>

#include "threads/threadtcpc.h"

#include <events/eventtcpcgrid.h>


ItemView6TcpC::ItemView6TcpC(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnect();
}

ItemView6TcpC::~ItemView6TcpC()
{
    //    stop();
}

void ItemView6TcpC::initUI()
{
    auto layout = new QHBoxLayout(this);
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);

    leftQWidgetLayout->setAlignment(Qt::AlignTop);


    auto leftQWidgetGroupBox1 = new QGroupBox("TCP服务设置", this);

    auto leftQWidgetGroup1Layout = new QVBoxLayout(leftQWidgetGroupBox1);
    auto leftQWidgetGroup1Layouta = new QHBoxLayout();
    auto leftQWidgetGroup1Layoutb = new QHBoxLayout();
    auto leftQWidgetGroup1Layoutc = new QHBoxLayout();
    leftQWidgetGroup1Layout->setAlignment(Qt::AlignTop);
    leftQWidgetGroup1Layouta->setAlignment(Qt::AlignLeft);
    leftQWidgetGroup1Layoutb->setAlignment(Qt::AlignLeft);
    leftQWidgetGroup1Layoutc->setAlignment(Qt::AlignRight);
    leftQWidgetGroup1Layout->addLayout(leftQWidgetGroup1Layouta);
    leftQWidgetGroup1Layout->addLayout(leftQWidgetGroup1Layoutb);
    leftQWidgetGroup1Layout->addLayout(leftQWidgetGroup1Layoutc);

    auto leftQWidgetGroupBox2 = new QGroupBox("发送信息", this);
    auto leftQWidgetGroup2Layout = new QVBoxLayout();
    auto leftQWidgetGroup2Layouta = new QHBoxLayout();
    auto leftQWidgetGroup2Layoutb = new QHBoxLayout();
    leftQWidgetGroupBox2->setLayout(leftQWidgetGroup2Layout);
    leftQWidgetGroup2Layout->setAlignment(Qt::AlignTop);
    leftQWidgetGroup2Layouta->setAlignment(Qt::AlignLeft);
    leftQWidgetGroup2Layoutb->setAlignment(Qt::AlignRight);
    leftQWidgetGroup2Layout->addLayout(leftQWidgetGroup2Layouta);
    leftQWidgetGroup2Layout->addLayout(leftQWidgetGroup2Layoutb);


    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1, 0, Qt::AlignTop);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2, 0, Qt::AlignTop);


    cbCharEncodingS =  new QComboBox();

    cbCharEncodingS->addItem("UTF-8", QVariant::fromValue(1));
    cbCharEncodingS->addItem("GBK",   QVariant::fromValue(2));

    btnSend = new QPushButton("发送信息");
    edtSend = new QPlainTextEdit();
    leftQWidgetGroup2Layouta->addWidget(edtSend, 0, Qt::AlignLeft);
    leftQWidgetGroup2Layoutb->addWidget(new QLabel("编码:", this), 0, Qt::AlignLeft);
    leftQWidgetGroup2Layoutb->addWidget(cbCharEncodingS,         0, Qt::AlignLeft);
    leftQWidgetGroup2Layoutb->addWidget(btnSend,                 0, Qt::AlignRight);


    auto centerQWidget = new QWidget(this);
    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);


    auto centerQWidgetGroupBox2 = new QGroupBox("接收信息", centerQWidget);
    auto centerQWidgetGroupBox2Layout = new QVBoxLayout(centerQWidgetGroupBox2);
    auto centerQWidgetGroupBox2Layouta = new QHBoxLayout();
    auto centerQWidgetGroupBox2Layoutb = new QHBoxLayout();
    centerQWidgetGroupBox2Layout->setAlignment(Qt::AlignTop);
    centerQWidgetGroupBox2Layouta->setAlignment(Qt::AlignLeft);
    centerQWidgetGroupBox2Layoutb->setAlignment(Qt::AlignLeft);
    centerQWidgetGroupBox2Layout->addLayout(centerQWidgetGroupBox2Layouta);
    centerQWidgetGroupBox2Layout->addLayout(centerQWidgetGroupBox2Layoutb);


    cbCharEncoding =  new QComboBox();

    cbCharEncoding->addItem("UTF-8", QVariant::fromValue(1));
    cbCharEncoding->addItem("GBK",   QVariant::fromValue(2));
    btnRecvClear = new QPushButton("清空信息");

    edtRecv = new QPlainTextEdit();
    edtRecv->setReadOnly(true);
    centerQWidgetGroupBox2Layoutb->addWidget(edtRecv);


    centerQWidgetGroupBox2Layouta->addWidget(new QLabel("编码:", this), 0, Qt::AlignLeft);
    centerQWidgetGroupBox2Layouta->addWidget(cbCharEncoding,          1, Qt::AlignLeft);
    centerQWidgetGroupBox2Layouta->addWidget(btnRecvClear,            0, Qt::AlignRight);


    auto centerQWidgetGroupBox1 = new QGroupBox("控制台设置", centerQWidget);
    auto centerQWidgetGroupBox1Layout = new QVBoxLayout(centerQWidgetGroupBox1);
    auto centerQWidgetGroupBox1Layouta = new QHBoxLayout();
    auto centerQWidgetGroupBox1Layoutb = new QHBoxLayout();
    centerQWidgetGroupBox1Layout->setAlignment(Qt::AlignTop);
    centerQWidgetGroupBox1Layout->addLayout(centerQWidgetGroupBox1Layouta);
    centerQWidgetGroupBox1Layout->addLayout(centerQWidgetGroupBox1Layoutb);
    ckConsoleEnable = new QCheckBox("启动控制台");
    btnConsoleClear = new QPushButton("清空控制台");
    edtMsg = new QPlainTextEdit();
    edtMsg->setReadOnly(true);
    centerQWidgetGroupBox1Layouta->addWidget(ckConsoleEnable, 0, Qt::AlignLeft);
    centerQWidgetGroupBox1Layouta->addWidget(btnConsoleClear, 0, Qt::AlignRight);
    centerQWidgetGroupBox1Layoutb->addWidget(edtMsg, 0, Qt::AlignTop);


    centerQWidgetLayout->addWidget(centerQWidgetGroupBox2);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox1, 0, Qt::AlignTop);


    auto rightQWidget = new QWidget(this);
    auto rightQWidgetLayout = new QVBoxLayout(rightQWidget);

    auto rightQWidgetGroupBox1 = new QGroupBox("客户端列表", rightQWidget);
    auto rightQWidgetGroupBox1Layout = new QHBoxLayout(rightQWidgetGroupBox1);
    auto rightQWidgetGroupBox2 = new QGroupBox("套接字选项", rightQWidget);
    auto rightQWidgetGroupBox2Layout = new QHBoxLayout(rightQWidgetGroupBox2);


    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox2);


    //    ckRefreshClients = new QCheckBox("刷新客户端");
    btnStartStop = new QPushButton("打开连接");

    //    btnClients = new QPushButton("刷新客户端");
    edtHost = new QLineEdit("127.0.0.1");
    edtPort = new QLineEdit("1024");


    infoTableView = new QTableView(this);
    infoGridModel = new QStandardItemModel(this);

    /* 设置表格标题行(输入数据为QStringList类型) */
    infoGridModel->setHorizontalHeaderLabels({  "类型", "ID", "地址", "远程服务端地址", "状态" });
    infoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    infoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    infoTableView->setModel(infoGridModel);


    optionTableView = new QTableView(this);
    optionGridModel = new QStandardItemModel(this);

    optionGridModel->setHorizontalHeaderLabels({  "LEVEL", "选项名称", "选项值", "说明", "数据类型" });
    optionTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    optionTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    optionTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    optionTableView->setModel(optionGridModel);


    leftQWidgetGroup1Layouta->addWidget(new QLabel("host:"), 0, Qt::AlignLeft);
    leftQWidgetGroup1Layoutb->addWidget(new QLabel("port:"), 0, Qt::AlignLeft);
    leftQWidgetGroup1Layouta->addWidget(edtHost, 0, Qt::AlignLeft);
    leftQWidgetGroup1Layoutb->addWidget(edtPort, 0, Qt::AlignLeft);
    leftQWidgetGroup1Layoutc->addWidget(btnStartStop, 0, Qt::AlignLeft);


    //    rightQWidgetGroupBox1Layout->addWidget(ckRefreshClients);
    //    rightQWidgetGroupBox1Layout->addWidget(btnClients);
    rightQWidgetGroupBox1Layout->addWidget(infoTableView);

    rightQWidgetGroupBox2Layout->addWidget(optionTableView);


    layout->addWidget(leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget(rightQWidget);

    int rowIndex = 0;

    optionGridModel->setItem(rowIndex,   0, new QStandardItem("套接字层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("SO_ACCEPTCONN"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("监听模式"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[BOOL][只读]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("套接字层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("SO_KEEPALIVE"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("心跳"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[BOOL][读写]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("套接字层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("SO_RCVBUF"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("接收缓冲区长度"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[int][读写]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("套接字层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("SO_SNDBUF"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("发送缓冲区长度"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[int][读写]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("套接字层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("SO_RCVTIMEO"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("接收超时时间"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[int][读写]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("套接字层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("SO_SNDTIMEO"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("发送超时时间"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[int][读写]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("套接字层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("SO_TYPE"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("套接字类型"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[int][只读]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("TCP层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("SO_TYPE"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("禁用Naglc算法"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[BOOL][读写]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("IP层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("IP_TTL"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("TTL"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[int][读写]"));
    optionGridModel->setItem(rowIndex,   0, new QStandardItem("IP层"));
    optionGridModel->setItem(rowIndex,   1, new QStandardItem("IP_TOS"));
    optionGridModel->setItem(rowIndex,   3, new QStandardItem("TOS(IP头内1字节)"));
    optionGridModel->setItem(rowIndex++, 4, new QStandardItem("[int][读写]"));
}

void ItemView6TcpC::initConnect()
{
    connect(infoTableView, &QTableView::clicked, [this](const QModelIndex& current) {
        if (current.isValid()) {
            currentSocketId = current.siblingAtColumn(1).data().toInt();

            // 获取套接字选项
            int rowIndex = 0;
            BOOL b_SO_ACCEPTCONN;
            socklen_t len =  sizeof(BOOL);
            getsockopt(currentSocketId, SOL_SOCKET, SO_ACCEPTCONN, (char *)&b_SO_ACCEPTCONN, &len);

            // BOOL, 只能获取, 判断是否处于监听模式
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(b_SO_ACCEPTCONN ? "True" : "False"));


            BOOL b_SO_KEEPALIVE;
            len =  sizeof(BOOL);
            getsockopt(currentSocketId, SOL_SOCKET, SO_KEEPALIVE, (char *)&b_SO_KEEPALIVE, &len);

            // BOOL, 均可, 保持活跃消息
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(b_SO_KEEPALIVE ? "True" : "False"));


            int i_SO_RCVBUF;
            len =  sizeof(int);
            getsockopt(currentSocketId, SOL_SOCKET, SO_RCVBUF, (char *)&i_SO_RCVBUF, &len);
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(QString::number(i_SO_RCVBUF)));

            int i_SO_SNDBUF;
            len =  sizeof(int);
            getsockopt(currentSocketId, SOL_SOCKET, SO_SNDBUF, (char *)&i_SO_SNDBUF, &len);
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(QString::number(i_SO_SNDBUF)));

            int i_SO_RCVTIMEO;
            len =  sizeof(int);
            getsockopt(currentSocketId, SOL_SOCKET, SO_RCVTIMEO, (char *)&i_SO_RCVTIMEO, &len);
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(QString::number(i_SO_RCVTIMEO)));


            int i_SO_SNDTIMEO;
            len =  sizeof(int);
            getsockopt(currentSocketId, SOL_SOCKET, SO_SNDTIMEO, (char *)&i_SO_SNDTIMEO, &len);
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(QString::number(i_SO_SNDTIMEO)));


            int i_SO_TYPE;
            len =  sizeof(int);
            getsockopt(currentSocketId, SOL_SOCKET, SO_TYPE, (char *)&i_SO_TYPE, &len);
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(QString::number(i_SO_TYPE)));


            BOOL b_TCP_NODELAY;
            len =  sizeof(BOOL);
            getsockopt(currentSocketId, IPPROTO_TCP, TCP_NODELAY, (char *)&b_TCP_NODELAY, &len);
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(b_TCP_NODELAY ? "True" : "False"));


            struct ip_option_hdr {
                unsigned char code;
                unsigned char length;
                unsigned char offset;
                unsigned long addrs[9];
            } opthdr;

            //            BOOL ca_IP_OPTIONS;
            //            len =  sizeof(BOOL);
            //            getsockopt(currentSocketId, IPPROTO_IP, IP_OPTIONS, (char *)&ca_IP_OPTIONS, &len);
            //            optionGridModel->setItem(6, 2, new QStandardItem(b_TCP_NODELAY ? "True" : "False"));

            //            u_char ca_IP_OPTIONS[44];
            //            len =  sizeof(ca_IP_OPTIONS);
            //            len =  sizeof(opthdr);
            //            getsockopt(currentSocketId, IPPROTO_IP, IP_OPTIONS, (char *)&opthdr, &len);

            // ip选项一般用不到

            int i_IP_TTL;
            len =  sizeof(int);
            getsockopt(currentSocketId, IPPROTO_IP, IP_TTL, (char *)&i_IP_TTL, &len);
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(QString::number(i_IP_TTL)));


            int i_IP_TOS;
            len =  sizeof(int);
            getsockopt(currentSocketId, IPPROTO_IP, IP_TOS, (char *)&i_IP_TOS, &len);
            optionGridModel->setItem(rowIndex++, 2, new QStandardItem(QString::number(i_IP_TOS)));
        }
    });

    connect(btnConsoleClear, &QPushButton::clicked, [this]() {
        clearConsole();
    });

    connect(btnRecvClear, &QPushButton::clicked, [this]() {
        clearRecvMsg();
    });

    connect(btnSend, &QPushButton::clicked, [this]() {
        if (!currentSocketId) {
            appendConsole(tr("请先选择一个TCP连接！"));
            return;
        }
        QString text = edtSend->toPlainText();

        if (text.isEmpty()) {
            appendConsole(tr("请输入发送内容！"));
            return;
        }


        QByteArray ba;

        if (cbCharEncodingS->currentIndex() == 0) {
            ba = text.toUtf8();
        } else {
            ba = text.toLocal8Bit();
        }
        char *data = ba.data();
        int ret = send(currentSocketId, data, ba.size(), 0);

        if (ret == SOCKET_ERROR) {
            appendConsole(tr("发送失败! errorcode:%1 [id:%2]").arg(WSAGetLastError()).arg(ret));
            return;
        }

        appendConsole(tr("发送成功[ret:%3]:%1[id:%2]").arg(edtSend->toPlainText()).arg(currentSocketId).arg(ret));

        clearSendMsg();
    });


    connect(btnStartStop, &QPushButton::clicked, [this]() {
        auto thread = new ThreadTcpC(this);


        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        //        connect(thread, &QThread::finished, []() {
        //            int d = 1;
        //            int a = d;
        //        });


        thread->ip = edtHost->text();
        thread->port = edtPort->text().toUShort();
        thread->setParent(this);

        thread->start();


        return;
    });

    //    connect(btnClients, &QPushButton::clicked, [this]() {});
}

void ItemView6TcpC::clientAdd(int socketid)
{
    // 获取地址


    struct sockaddr_storage ss;
    struct sockaddr_storage ss_r;
    socklen_t len;
    socklen_t len_r;

    len = sizeof(ss);
    len_r = sizeof(ss_r);

    if (SOCKET_ERROR == getsockname(socketid, (PSOCKADDR)&ss, &len)) {
        appendConsole(tr("getsockname error! %1").arg(WSAGetLastError()));
        return;
    }

    if (SOCKET_ERROR == getpeername(socketid, (PSOCKADDR)&ss_r, &len_r)) {
        appendConsole(tr("getsockname error! %1").arg(WSAGetLastError()));
        return;
    }

    PSOCKADDR_IN sa = (PSOCKADDR_IN)&ss;
    PSOCKADDR_IN sa_r = (PSOCKADDR_IN)&ss_r;


    WCHAR buff[17];
    WCHAR buff_r[17];

    auto p = InetNtop(AF_INET,  &sa->sin_addr, buff, sizeof(buff));

    if (p == NULL) {
        appendConsole(tr("InetPton error! %1 ").arg(WSAGetLastError()));
        return;
    }


    p = InetNtop(AF_INET,  &sa_r->sin_addr, buff_r, sizeof(buff_r));

    if (p == NULL) {
        appendConsole(tr("InetPton error! %1 ").arg(WSAGetLastError()));
        return;
    }


    int i = infoGridModel->rowCount();

    infoGridModel->setItem(i, 0, new QStandardItem("TCP"));
    infoGridModel->setItem(i, 1, new QStandardItem(QString::number(socketid)));
    infoGridModel->setItem(i, 2, new QStandardItem(tr("%1:%2").arg(QString::fromWCharArray(buff)).arg(ntohs(sa->sin_port))));
    infoGridModel->setItem(i, 3, new QStandardItem(tr("%1:%2").arg(QString::fromWCharArray(buff_r)).arg(ntohs(sa_r->sin_port))));
    infoGridModel->setItem(i, 4, new QStandardItem("在线"));
}

void ItemView6TcpC::clientRemove(int socketid)
{
    auto items = infoGridModel->findItems(QString::number(socketid), Qt::MatchExactly, 1);

    if (items.count() >= 1) {
        auto *firstItem = items.first();
        infoGridModel->removeRow(firstItem->row());
    }
}

void ItemView6TcpC::clearConsole()
{
    edtMsg->clear();
}

void ItemView6TcpC::clearRecvMsg()
{
    edtRecv->clear();
}

void ItemView6TcpC::clearSendMsg()
{
    edtSend->clear();
}

void ItemView6TcpC::postAppendRecvMsg(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg, qEventRecvMsg);

    QApplication::postEvent(this, event);
}

void ItemView6TcpC::postAppendConsole(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg, qEventAppendConsole);

    QApplication::postEvent(this, event);
}

void ItemView6TcpC::appendRecvMsg(const QString& msg)
{
    QString text = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    text += msg;
    edtRecv->appendPlainText(text);
    QTextCursor cursor = edtRecv->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtRecv->setTextCursor(cursor);
    edtRecv->repaint();
}

void ItemView6TcpC::appendConsole(const QString& msg)
{
    QString text = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    text += msg;
    edtMsg->appendPlainText(text);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void ItemView6TcpC::writeConsole(const QString& msg)
{
    QString text = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    text += msg;
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void ItemView6TcpC::customEvent(QEvent *e)
{
    switch (e->type())
    {
    case qEventTcpCAdd: {
        EventTcpCGrid *event = dynamic_cast<EventTcpCGrid *>(e);
        clientAdd(event->socketid);
        e->accept();
        break;
    }

    case qEventTcpCRemove: {
        EventTcpCGrid *event = dynamic_cast<EventTcpCGrid *>(e);
        clientRemove(event->socketid);
        e->accept();
        break;
    }


    case qEventAppendConsole:
    {
        QStringEvent *event = dynamic_cast<QStringEvent *>(e);
        appendConsole(event->message);
        e->accept();
        break;
    }

    case qEventRecvMsg:
    {
        QStringEvent *event = dynamic_cast<QStringEvent *>(e);
        appendRecvMsg(event->message);
        e->accept();
        break;
    }


    default:
        break;
    }
}

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
    auto rightQWidgetGroupBox1 = new QGroupBox("客户端控制", rightQWidget);
    auto rightQWidgetGroupBox1Layout = new QHBoxLayout(rightQWidgetGroupBox1);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    ckRefreshClients = new QCheckBox("刷新客户端");
    btnStartStop = new QPushButton("打开连接");
    btnClients = new QPushButton("刷新客户端");
    edtHost = new QLineEdit("127.0.0.1");
    edtPort = new QLineEdit("1024");


    infoTableView = new QTableView(this);
    infoGridModel = new QStandardItemModel();

    /* 设置表格标题行(输入数据为QStringList类型) */
    infoGridModel->setHorizontalHeaderLabels({  "类型", "ID", "地址", "状态" });
    infoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    infoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    infoTableView->setModel(infoGridModel);


    leftQWidgetGroup1Layouta->addWidget(new QLabel("host:"), 0, Qt::AlignLeft);
    leftQWidgetGroup1Layoutb->addWidget(new QLabel("port:"), 0, Qt::AlignLeft);
    leftQWidgetGroup1Layouta->addWidget(edtHost, 0, Qt::AlignLeft);
    leftQWidgetGroup1Layoutb->addWidget(edtPort, 0, Qt::AlignLeft);
    leftQWidgetGroup1Layoutc->addWidget(btnStartStop, 0, Qt::AlignLeft);
    leftQWidgetGroup1Layoutc->addWidget(btnClients,   0, Qt::AlignLeft);


    rightQWidgetGroupBox1Layout->addWidget(ckRefreshClients);
    rightQWidgetGroupBox1Layout->addWidget(btnClients);
    rightQWidgetLayout->addWidget(infoTableView);


    layout->addWidget(leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget(rightQWidget);
}

void ItemView6TcpC::initConnect()
{
    connect(infoTableView, &QTableView::clicked, [this](const QModelIndex& current) {
        if (current.isValid()) {
            currentSocketId = current.siblingAtColumn(1).data().toInt();
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

        QByteArray ba;

        if (cbCharEncodingS->currentIndex() == 0) {
            ba = edtSend->toPlainText().toUtf8();
        } else {
            ba = edtSend->toPlainText().toLocal8Bit();
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

    connect(btnClients, &QPushButton::clicked, [this]() {});
}

void ItemView6TcpC::clientAdd(int socketid)
{
    int i = infoGridModel->rowCount();

    infoGridModel->setItem(i, 0, new QStandardItem("TCP"));
    infoGridModel->setItem(i, 1, new QStandardItem(QString::number(socketid)));

    //    infoGridModel->setItem(i, 2, new QStandardItem(channel->peeraddr().c_str()));
    infoGridModel->setItem(i, 3, new QStandardItem("在线"));
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

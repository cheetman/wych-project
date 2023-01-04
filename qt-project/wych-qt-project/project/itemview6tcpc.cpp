#include "itemview6tcpc.h"

#include "mainwindow.h"
#include "events/customevent.h"

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


ItemView6TcpC::ItemView6TcpC(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnect();
}

ItemView6TcpC::~ItemView6TcpC()
{
    stop();
}

void ItemView6TcpC::initUI()
{
    auto layout = new QHBoxLayout(this);
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);
    auto leftQWidgetGroupBox1 = new QGroupBox("TCP服务设置", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout = new QGridLayout(leftQWidgetGroupBox1);
    leftQWidgetGroupBox1->setFixedHeight(200);

    //    leftQWidgetGroupBox1->setFixedWidth(400);


    auto centerQWidget = new QWidget(this);
    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);
    auto centerQWidgetGroupBox1 = new QGroupBox("控制台设置", centerQWidget);
    auto centerQWidgetGroupBox1Layout = new QHBoxLayout(centerQWidgetGroupBox1);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox1);


    auto rightQWidget = new QWidget(this);
    auto rightQWidgetLayout = new QVBoxLayout(rightQWidget);
    auto rightQWidgetGroupBox1 = new QGroupBox("客户端控制", rightQWidget);
    auto rightQWidgetGroupBox1Layout = new QHBoxLayout(rightQWidgetGroupBox1);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    ckConsoleEnable = new QCheckBox("启动控制台");
    ckRefreshClients = new QCheckBox("刷新客户端");
    btnStartStop = new QPushButton("start");
    btnConsoleClear = new QPushButton("清空控制台");
    btnClients = new QPushButton("刷新客户端");
    edtHost = new QLineEdit("127.0.0.1");
    edtPort = new QLineEdit("1024");

    //    edtMsg = new QTextEdit();
    edtMsg = new QPlainTextEdit();

    edtMsg->setReadOnly(true);


    infoTableView = new QTableView(this);
    infoGridModel = new QStandardItemModel();

    /* 设置表格标题行(输入数据为QStringList类型) */
    infoGridModel->setHorizontalHeaderLabels({  "类型", "ID", "地址", "状态" });
    infoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    infoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    infoTableView->setModel(infoGridModel);


    leftQWidgetGroup1Layout->addWidget(new QLabel("host:"), 0, 0);
    leftQWidgetGroup1Layout->addWidget(new QLabel("port:"), 1, 0);
    leftQWidgetGroup1Layout->addWidget(btnStartStop,        2, 0);
    leftQWidgetGroup1Layout->addWidget(btnClients,          2, 1);
    leftQWidgetGroup1Layout->addWidget(edtHost,             0, 1);
    leftQWidgetGroup1Layout->addWidget(edtPort,             1, 1);

    centerQWidgetLayout->addWidget(edtMsg);
    centerQWidgetGroupBox1Layout->addWidget(ckConsoleEnable);
    centerQWidgetGroupBox1Layout->addWidget(btnConsoleClear);

    rightQWidgetGroupBox1Layout->addWidget(ckRefreshClients);
    rightQWidgetGroupBox1Layout->addWidget(btnClients);
    rightQWidgetLayout->addWidget(infoTableView);


    layout->addWidget(leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget(rightQWidget);
}

void ItemView6TcpC::initConnect()
{
    connect(btnConsoleClear, &QPushButton::clicked, [this]() {
        clearConsole();
    });

    connect(btnStartStop, &QPushButton::clicked, [this]() {
        // Windows 必须先执行WSAStartup ，否则socket构造时会报错
        WSADATA wsadata;
        int Ret;

        if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0) {
            // 注意，因为winsock没有加载，不能使用WSAGetLastError来确定导致故障的特定错误，但可以根据返回值确定。
            appendConsole(tr("WSAStartup error RetCode: %1!").arg(Ret));
            return;
        }


        int sockfd, n;
        char recvline[200 + 1];

        // struct sockaddr servaddr; 这个结构没有把ip与端口分开
        // struct sockaddr_in servaddr;
        sockaddr_in servaddr;

        // IPPROTO_TCP = 6
        if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
            appendConsole(tr("socket error RetCode: %1!").arg(sockfd));
            return;
        }

        // bzero(&servaddr, sizeof(servaddr)); windows 下没有bzero函数
        ZeroMemory(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(13);


        wchar_t ip[20] = { 0 };
        edtHost->text().toWCharArray(ip);

        // inet_pton UNIX下
        if (InetPton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
            appendConsole(tr("InetPton error !"));
            return;
        }


        // 这里强转sockaddr * 好像历史遗留问题
        if (::connect(sockfd, (PSOCKADDR)&servaddr, sizeof(servaddr)) < 0) {
            appendConsole(tr("connect error !").arg(WSAGetLastError()));
        }


        return;

        if (!isStart) {
            std::string host = edtHost->text().toStdString();
            int port = edtPort->text().toInt();

            if (start(port, host.c_str())) {
                btnStartStop->setText(tr("关闭"));
                appendConsole(QString::asprintf("TCP server running on %s:%d ...", host.c_str(), port));
            } else {
                appendConsole(QString::asprintf("TCP server start failed!"));
            }
            isStart = true;
        } else {
            stop();
            btnStartStop->setText(tr("打开"));

            //        g_mainwnd->
            appendConsole("TCP server stopped!");
            isStart = false;
        }
    });

    connect(btnClients, &QPushButton::clicked, [this]() {
        //        if (server == NULL) {
        //            return;
        //        }

        //        int i = 0;
        //        int count = server->foreachChannel([this, &i](const hv::SocketChannelPtr& channel) {
        //            this->infoGridModel->setItem(i, 0, new QStandardItem("TCP"));
        //            this->infoGridModel->setItem(i, 1, new QStandardItem(QString::number(channel->fd())));
        //            this->infoGridModel->setItem(i, 2, new QStandardItem(channel->peeraddr().c_str()));
        //            this->infoGridModel->setItem(i++, 3, new QStandardItem("在线"));
        //        });
        //        auto removeCount = infoGridModel->rowCount() - count;

        //        if (removeCount > 0) {
        //            infoGridModel->removeRows(i, removeCount);
        //        }
    });

    QTimer *clientTimer = new QTimer(this);
    connect(clientTimer, &QTimer::timeout, [this]() {
        if (!ckRefreshClients->isChecked()) {
            return;
        }
    });
    clientTimer->start(2000);
}

bool ItemView6TcpC::start(int port, const char *host)
{
    return true;
}

void ItemView6TcpC::stop()
{}

void ItemView6TcpC::clearConsole()
{
    edtMsg->clear();
}

void ItemView6TcpC::postMessage(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg);

    QApplication::postEvent(this, event);
}

void ItemView6TcpC::appendConsole(const QString& msg)
{
    QString text = edtMsg->toPlainText();

    text += QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    //    text += QDateTime::currentDateTime().toString("[yyyy-MM-dd
    // hh:mm:ss.zzz] ");
    text += msg;

    if (text.back() != '\n') {
        text += "\n";
    }
    writeConsole(text);
}

void ItemView6TcpC::writeConsole(const QString& msg)
{
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
    case qEventRecvMsg:
    {
        QStringEvent *event = dynamic_cast<QStringEvent *>(e);
        appendConsole(event->message);
    }
        e->accept();
        break;

    default:
        break;
    }
}

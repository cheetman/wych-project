#include "itemview9tcp.h"

#include "mainwindow.h"
#include "customevent.h"

// #pragma comment(lib, "libhv.lib")


ItemView9Tcp::ItemView9Tcp(QWidget *parent)
    : QWidget{parent}
{
    server = nullptr;
    initUI();
    initConnect();
}

ItemView9Tcp::~ItemView9Tcp()
{
    stop();
}

void ItemView9Tcp::initUI()
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
    edtHost = new QLineEdit("0.0.0.0");
    edtPort = new QLineEdit("1234");

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

void ItemView9Tcp::initConnect()
{
    connect(btnConsoleClear, &QPushButton::clicked, [this]() {
        clearMessage();
    });

    connect(btnStartStop, &QPushButton::clicked, [this]() {
        if (!isStart) {
            std::string host = edtHost->text().toStdString();
            int port = edtPort->text().toInt();

            if (start(port, host.c_str())) {
                btnStartStop->setText(tr("关闭"));
                appendMessage(QString::asprintf("TCP server running on %s:%d ...", host.c_str(), port));
            } else {
                appendMessage(QString::asprintf("TCP server start failed!"));
            }
            isStart = true;
        } else {
            stop();
            btnStartStop->setText(tr("打开"));

            //        g_mainwnd->
            appendMessage("TCP server stopped!");
            isStart = false;
        }
    });

    connect(btnClients, &QPushButton::clicked, [this]() {
        if (server == NULL) {
            return;
        }

        int i = 0;
        int count = server->foreachChannel([this, &i](const hv::SocketChannelPtr& channel) {
            this->infoGridModel->setItem(i, 0, new QStandardItem("TCP"));
            this->infoGridModel->setItem(i, 1, new QStandardItem(QString::number(channel->fd())));
            this->infoGridModel->setItem(i, 2, new QStandardItem(channel->peeraddr().c_str()));
            this->infoGridModel->setItem(i++, 3, new QStandardItem("在线"));
        });
        auto removeCount = infoGridModel->rowCount() - count;

        if (removeCount > 0) {
            infoGridModel->removeRows(i, removeCount);
        }
    });

    QTimer *clientTimer = new QTimer(this);
    connect(clientTimer, &QTimer::timeout, [this]() {
        if (!ckRefreshClients->isChecked()) {
            return;
        }

        if (server == NULL) {
            return;
        }


        //        infoGridModel->clear();
        //        infoGridModel->setHorizontalHeaderLabels({  "类型","ID", "地址",
        // "状态"});


        int i = 0;
        int count = server->foreachChannel([this, &i](const hv::SocketChannelPtr& channel) {
            this->infoGridModel->setItem(i, 0, new QStandardItem("TCP"));
            this->infoGridModel->setItem(i, 1, new QStandardItem(QString::number(channel->fd())));
            this->infoGridModel->setItem(i, 2, new QStandardItem(channel->peeraddr().c_str()));
            this->infoGridModel->setItem(i++, 3, new QStandardItem("在线"));
        });
        auto removeCount = infoGridModel->rowCount() - count;

        if (removeCount > 0) {
            infoGridModel->removeRows(i, removeCount);
        }
    });
    clientTimer->start(2000);
}

bool ItemView9Tcp::start(int port, const char *host)
{
    server = new hv::TcpServer;
    int listenfd = server->createsocket(port, host);

    if (listenfd < 0) {
        return false;
    }
    server->setThreadNum(1);
    server->onConnection = [this](const hv::SocketChannelPtr& channel) {
                               if (!ckConsoleEnable->isChecked()) {
                                   return;
                               }

                               std::string peeraddr = channel->peeraddr();

                               if (channel->isConnected()) {
                                   this->postMessage(QString::asprintf("%s connected! connfd=%d", peeraddr.c_str(), channel->fd()));
                               } else {
                                   this->postMessage(QString::asprintf("%s disconnected! connfd=%d", peeraddr.c_str(), channel->fd()));
                               }
                           };
    server->onMessage = [this](const hv::SocketChannelPtr& channel, hv::Buffer *buf) {
                            if (!ckConsoleEnable->isChecked()) {
                                return;
                            }

                            QTextCodec::ConverterState state;
                            auto str =  QTextCodec::codecForName("GBK")->toUnicode((char *)buf->data(), (int)buf->size(), &state);


                            //        this->postMessage(QString::asprintf("<
                            // %.*s", (int)buf->size(), (char*)buf->data()));
                            this->postMessage(tr("< %1").arg(str));

                            // echo
                            //        channel->write(buf);
                        };
    server->start();


    return true;
}

void ItemView9Tcp::stop()
{
    SAFE_DELETE(server);
}

void ItemView9Tcp::clearMessage()
{
    edtMsg->clear();
}

void ItemView9Tcp::postMessage(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg);

    QApplication::postEvent(this, event);
}

void ItemView9Tcp::appendMessage(const QString& msg)
{
    QString text = edtMsg->toPlainText();

    text += QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    //    text += QDateTime::currentDateTime().toString("[yyyy-MM-dd
    // hh:mm:ss.zzz] ");
    text += msg;

    if (text.back() != '\n') {
        text += "\n";
    }
    showMessage(text);
}

void ItemView9Tcp::showMessage(const QString& msg)
{
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void ItemView9Tcp::customEvent(QEvent *e)
{
    switch (e->type())
    {
    case qEventRecvMsg:
    {
        QStringEvent *event = dynamic_cast<QStringEvent *>(e);
        appendMessage(event->message);
    }
        e->accept();
        break;

    default:
        break;
    }
}

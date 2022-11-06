#include "customevent.h"
#include "itemview11cmd.h"

#include "utils.h"
#include <QTextCodec>
#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>

#include "winapi.h"
#include "process.h"

ItemView11Cmd::ItemView11Cmd(QWidget *parent)
    : QWidget{parent}
{
    initUI();

    initConnect();
}

void ItemView11Cmd::initUI()
{
    auto layout = new QHBoxLayout(this);

    // 第一层
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);

    leftQWidget->setFixedWidth(300);

    // 第二层
    auto leftQWidgetGroupBox1 = new QGroupBox("按钮", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout = new QGridLayout(leftQWidgetGroupBox1);
    leftQWidgetGroupBox1->setFixedHeight(150);


    //    auto leftQWidgetGroupBox2 = new QGroupBox("进程", this);
    //    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    //    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    //    auto leftQWidgetGroup1Layout2 = new QGridLayout(leftQWidgetGroupBox2);


    //    auto leftQWidgetGroupBox3 = new QGroupBox("模块", this);
    //    leftQWidgetLayout->addWidget(leftQWidgetGroupBox3);
    //    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    //    auto leftQWidgetGroup1Layout3 = new QGridLayout(leftQWidgetGroupBox3);


    //    processTableView = new QTableView(this);
    //    processGridModel = new QStandardItemModel();
    //    processGridModel->setHorizontalHeaderLabels({  "进程", "PID",  "镜像基址", "镜像大小" });
    //    processTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    processTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    processTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    processTableView->setModel(processGridModel);
    //    leftQWidgetGroup1Layout2->addWidget(processTableView);


    //    moduleTableView = new QTableView(this);
    //    moduleGridModel = new QStandardItemModel();
    //    moduleGridModel->setHorizontalHeaderLabels({  "模块名称", "模块基址",  "模块大小" });
    //    moduleTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    moduleTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    moduleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    moduleTableView->setModel(moduleGridModel);
    //    leftQWidgetGroup1Layout3->addWidget(moduleTableView);


    // 第一层
    auto centerQWidget = new QWidget(this);
    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);


    // 第二层

    auto centerQWidgetGroupBox1 = new QGroupBox("控制台设置", centerQWidget);
    auto centerQWidgetGroupBox1Layout = new QHBoxLayout(centerQWidgetGroupBox1);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox1);

    edtMsg = new QPlainTextEdit();
    edtMsg->setReadOnly(true);
    centerQWidgetLayout->addWidget(edtMsg);


    leInputCmd = new QLineEdit();
    centerQWidgetLayout->addWidget(leInputCmd);


    // 第一层
    auto rightQWidget = new QWidget(this);
    auto rightQWidgetLayout = new QVBoxLayout(rightQWidget);

    rightQWidget->setFixedWidth(500);

    // 第二层
    auto rightQWidgetGroupBox1 = new QGroupBox("数据目录表明细", rightQWidget);
    auto rightQWidgetGroupBox1Layout = new QVBoxLayout(rightQWidgetGroupBox1);

    // 第三层
    auto tabTabWidget = new QTabWidget(rightQWidgetGroupBox1);

    // auto tab = new QWidget(tabTabWidget);
    // auto exportTabTabWidgetLayout = new QVBoxLayout(tab);
    // auto tab2 = new QWidget(tabTabWidget);
    // auto importTabTabWidgetLayout = new QVBoxLayout(tab2);
    auto tab3 = new QWidget(tabTabWidget);
    auto relocationTabTabWidgetLayout = new QVBoxLayout(tab3);

    // tabTabWidget->addTab(tab,  tr("导出表"));
    // tabTabWidget->addTab(tab2, tr("导入表"));
    tabTabWidget->addTab(tab3, tr("安卓命令"));
    rightQWidgetGroupBox1Layout->addWidget(tabTabWidget);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    // 第四层(重定位)
    auto relocationTabTabWidgetGroupBox = new QGroupBox("cmd", tab3);
    relocationTabTabWidgetLayout->addWidget(relocationTabTabWidgetGroupBox);
    relocationTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto relocationTabTabWidgetGroupBoxLayout = new QGridLayout(relocationTabTabWidgetGroupBox);
    relocationTabTabWidgetGroupBox->setFixedHeight(350);
    android1TableView = new QTableView(this);
    android1GridModel = new QStandardItemModel();
    android1GridModel->setHorizontalHeaderLabels({  "命令",  "说明", "类型", "回车" });
    android1TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    android1TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    android1TableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    android1TableView->setModel(android1GridModel);
    relocationTabTabWidgetGroupBoxLayout->addWidget(android1TableView);

    auto relocationTabTabWidgetGroupBox2 = new QGroupBox("adb", tab3);
    relocationTabTabWidgetLayout->addWidget(relocationTabTabWidgetGroupBox2);
    relocationTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto relocationTabTabWidgetGroupBoxLayout2 = new QGridLayout(relocationTabTabWidgetGroupBox2);
    android2TableView = new QTableView(this);
    android2GridModel = new QStandardItemModel();
    android2GridModel->setHorizontalHeaderLabels({  "命令",  "说明", "类型", "回车"  });
    android2TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    android2TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    android2TableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    android2TableView->setModel(android2GridModel);
    relocationTabTabWidgetGroupBoxLayout2->addWidget(android2TableView);


    // 控件
    ckConsoleEnable = new QCheckBox("启动控制台");

    btnStart = new QPushButton("启动连接控制台");


    btnExit = new QPushButton("退出");

    btnConsoleClear = new QPushButton("清空控制台");

    leftQWidgetGroup1Layout->addWidget(btnStart, 0, 0);
    leftQWidgetGroup1Layout->addWidget( btnExit, 1, 0);


    layout->addWidget(  leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget( rightQWidget);

    // 用于toLocal8Bit支持中文
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));


    // 初始化
    // 读取配置
    QSettings settings("configs/setting.ini", QSettings::IniFormat);
    QFile     file(settings.value("DefaultDir/Cmd", "D:\\cmdConsoloList.json").toString());

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString value = file.readAll();
        file.close();
        QJsonParseError parseJsonErr;
        auto document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);

        if (!(parseJsonErr.error == QJsonParseError::NoError)) {
            QMessageBox::about(this, "提示", "配置文件错误！");
            return;
        }

        if (document.isObject()) {
            QJsonObject jsonObj =  document.object();

            if (jsonObj.contains("android1")) {
                QJsonArray jsonArray = jsonObj["android1"].toArray();

                for (int i = 0; i < jsonArray.size(); i++) {
                    QJsonObject jsonObj2 = jsonArray.at(i).toObject();

                    android1GridModel->setItem(i, 0, new QStandardItem(jsonObj2["name"].toString()));
                    android1GridModel->setItem(i, 1, new QStandardItem(jsonObj2["cmd"].toString()));
                    android1GridModel->setItem(i, 2, new QStandardItem(jsonObj2["type"].toString()));
                    android1GridModel->setItem(i, 3, new QStandardItem(jsonObj2["return"].toString()));
                }
            }

            if (jsonObj.contains("android2")) {
                QJsonArray jsonArray = jsonObj["android2"].toArray();

                for (int i = 0; i < jsonArray.size(); i++) {
                    QJsonObject jsonObj2 = jsonArray.at(i).toObject();

                    android2GridModel->setItem(i, 0, new QStandardItem(jsonObj2["name"].toString()));
                    android2GridModel->setItem(i, 1, new QStandardItem(jsonObj2["cmd"].toString()));
                    android2GridModel->setItem(i, 2, new QStandardItem(jsonObj2["type"].toString()));
                    android2GridModel->setItem(i, 3, new QStandardItem(jsonObj2["return"].toString()));
                }
            }
        }
    } else {
        // QMessageBox::warning(this, tr("错误"), tr("配置文件打开失败！"));
    }
}

HANDLE hRead, hWrite;
HANDLE hRead2, hWrite2;

void ItemView11Cmd::inputCmd()
{
    if (isStart) {
        // QByteArray ba = leInputCmd->text().toLatin1();
        QByteArray ba = leInputCmd->text().toLocal8Bit();
        char *input = ba.data();

        //        int   count = strlen(input) + 2;
        char newInput[50];
        sprintf(newInput, "%s\r\n", input);
        DWORD dwWrited;
        WriteFile(hWrite2, newInput, strlen(newInput), &dwWrited, NULL);
    }
}

void ItemView11Cmd::initConnect()
{
    // 事件 - 输入回车
    connect(leInputCmd, &QLineEdit::returnPressed, [this]() {
        if (isStart) {
            inputCmd();
        }
    });


    // 退出
    connect(btnExit, &QPushButton::clicked, [this]() {
        if (isStart) {
            const char *dd = "exit\r\n";
            DWORD dwWrited;
            WriteFile(hWrite2, "exit\r\n", strlen(dd), &dwWrited, NULL);
            isStart = false;
        }
    });


    connect(btnStart, &QPushButton::clicked, [this]() {
        if (!isStart) {
            edtMsg->setPlainText("");

            unsigned threadid;
            HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &ItemView11Cmd::start, this, NULL, &threadid);
            CloseHandle(hThread);
            isStart = true;
        }
    });

    connect(android1TableView, &QTableView::doubleClicked, [this](const QModelIndex& current) {
        if (isStart) {
            auto name =  android1GridModel->item(current.row(), 1)->text();
            auto returnFlag =  android1GridModel->item(current.row(), 3)->text();
            leInputCmd->setText(name);

            if (returnFlag == "true") {
                inputCmd();
            }
        }
    });

    connect(android2TableView, &QTableView::doubleClicked, [this](const QModelIndex& current) {
        if (isStart) {
            auto name =  android2GridModel->item(current.row(), 1)->text();
            auto returnFlag =  android2GridModel->item(current.row(), 3)->text();
            leInputCmd->setText(name);

            if (returnFlag == "true") {
                inputCmd();
            }
        }
    });
}

unsigned __stdcall ItemView11Cmd::start(void *param) {
    auto obj = (ItemView11Cmd *)param;

    // 创建匿名管道
    //    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;           // 使用系统默认的安全描述符
    sa.bInheritHandle = TRUE;                 // 创建的进程继承句柄

    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) // 创建匿名管道
    {
        QMessageBox::critical(obj, "错误", "CreatePipe Failed!");
        return 1;
    }

    if (!CreatePipe(&hRead2, &hWrite2, &sa, 0)) // 创建匿名管道(用于写入)
    {
        QMessageBox::critical(obj, "错误", "CreatePipe Failed!");
        return 1;
    }


    STARTUPINFO si = { 0 };

    // ZeroMemory(&si, sizeof(STARTUPINFO)); 上面一行与这行的汇编完全一样
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfo(&si);

    si.hStdError = hWrite;
    si.hStdOutput = hWrite;   // 新创建进程的标准输出连在写管道一端
    // si.wShowWindow = SW_SHOW; // 显示窗口 显示好像没啥用，不显示内容，关闭也不触发信号
    si.wShowWindow = SW_HIDE; // 隐藏窗口
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdInput = hRead2;

    // si.dwFlags = STARTF_USESHOWWINDOW;

    PROCESS_INFORMATION pi;

    wchar_t cmdLine[256] = { 0 };
    GetSystemDirectory(cmdLine, sizeof(cmdLine));
    wcscat(cmdLine, (L"\\cmd.exe"));

    wchar_t lpCommandLine[200];
    wcsncpy(lpCommandLine, TEXT("cmd.exe /k ping baidu.com"), 40);

    if (!CreateProcess(cmdLine, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) // 创建子进程

    // if (!CreateProcess(NULL, lpCommandLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) // 创建子进程
    {
        QMessageBox::critical(obj, "错误", "CreateProcess Failed!");
        return 1;
    }
    CloseHandle(hWrite);
    CloseHandle(hRead2);
    CloseHandle(    pi.hThread);
    CloseHandle(    pi.hProcess);

    // CloseHandle(hWrite); // 关闭管道句柄

    char buffer[4096] = { 0 };
    QString strOutput;
    DWORD   bytesRead;

    while (true)
    {
        if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL) // 读取管道
        {
            Sleep(1000);
            break;
        }

        buffer[bytesRead] = '\0';

        obj->postMessage(QString::fromLocal8Bit(buffer));

        // 退出
        if (!strncmp("exit", buffer, 4))
        {
            break;
        }
        Sleep(50);
    }

    CloseHandle(  hRead);
    CloseHandle(hWrite2);
    return 0;
}

void ItemView11Cmd::appendConsolo(const QString& msg)
{
    QString text = edtMsg->toPlainText();

    text += msg;
    edtMsg->setPlainText(text);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void ItemView11Cmd::postMessage(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg);

    QApplication::postEvent(this, event);
}

void ItemView11Cmd::customEvent(QEvent *e)
{
    switch (e->type())
    {
    case qEventRecvMsg:
    {
        QStringEvent *event = dynamic_cast<QStringEvent *>(e);

        appendConsolo(event->message);
    }
        e->accept();
        break;

    default:
        break;
    }
}

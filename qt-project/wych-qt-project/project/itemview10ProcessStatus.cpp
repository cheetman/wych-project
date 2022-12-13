

#include <QTextCodec>
#include <QDateTime>
#include <vector>
#include <QScrollArea>
#include <QImage>
#include <QtWin>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDebug>
#include <QTreeView>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSplitter>
#include <QInputDialog>
#include <TlHelp32.h>
#include <process.h>
#include <winuser.h>
#include <Windows.h>
#include "utils.h"
#include "winapi.h"
#include <QComboBox>
#include <QSpinBox>
#include <QTableWidget>
#include <tchar.h>
#include "itemview10ProcessStatus.h"
#include "components/pixmapwidget.h"
#include "components/scripttypedialog.h"
#include "events/customevent.h"
#include "events/eventwinmessage.h"

Itemview10ProcessStatus *g_itemview10ProcessStatus;

Itemview10ProcessStatus::Itemview10ProcessStatus(QWidget *parent)
    : QWidget{parent}
{
    g_itemview10ProcessStatus = this;
    initUI();
    initConnect();
}

Itemview10ProcessStatus::~Itemview10ProcessStatus()
{}

void Itemview10ProcessStatus::initUI()
{
    // 控件

    tb_window_size = new QLineEdit("");
    tb_window_size->setFixedWidth(150);
    tb_window2_size = new QLineEdit("");
    tb_window2_size->setFixedWidth(150);
    tb_window_position = new QLineEdit("");
    tb_window_position->setFixedWidth(180);
    tb_window2_position = new QLineEdit("");
    tb_window2_position->setFixedWidth(180);
    tb_export_foa = new QLineEdit("");
    tb_import_foa = new QLineEdit("");
    tb_resource_foa = new QLineEdit("");
    tb_base_relocation_foa = new QLineEdit("");
    tb_process_name = new QLineEdit("");
    tb_process_name->setFixedWidth(220);
    tb_process_handle = new QLineEdit("");
    tb_window_name = new QLineEdit("");
    tb_window_name->setFixedWidth(150);
    tb_window_handle = new QLineEdit("");
    tb_window_handle->setFixedWidth(100);
    tb_window_handle2 = new QLineEdit("");
    tb_window_handle2->setFixedWidth(100);
    btnRefreshWindow = new QPushButton("刷新");
    btnFindWindow = new CaptureBtn("捕获");
    btnRefreshWindow->setFixedWidth(60);
    btnFindWindow->setFixedWidth(60);


    //    this->setMouseTracking(true);
    auto layout = new QHBoxLayout(this);

    // 第一层
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);

    // 第二层
    auto leftQWidgetGroupBox1 = new QGroupBox("状态", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto layout_l_1_m = new QVBoxLayout();
    auto leftQWidgetGroup1aLayout = new QHBoxLayout();
    auto leftQWidgetGroup1bLayout = new QHBoxLayout();
    auto leftQWidgetGroup1cLayout = new QHBoxLayout();
    auto leftQWidgetGroup1dLayout = new QHBoxLayout();
    auto leftQWidgetGroup1eLayout = new QHBoxLayout();
    leftQWidgetGroup1aLayout->setAlignment(Qt::AlignLeft);
    leftQWidgetGroup1bLayout->setAlignment(Qt::AlignLeft);
    leftQWidgetGroup1cLayout->setAlignment(Qt::AlignLeft);
    leftQWidgetGroup1dLayout->setAlignment(Qt::AlignLeft);
    leftQWidgetGroupBox1->setLayout(layout_l_1_m);
    layout_l_1_m->addLayout(leftQWidgetGroup1aLayout);
    layout_l_1_m->addLayout(leftQWidgetGroup1bLayout);
    layout_l_1_m->addLayout(leftQWidgetGroup1cLayout);
    layout_l_1_m->addLayout(leftQWidgetGroup1dLayout);
    layout_l_1_m->addLayout(leftQWidgetGroup1eLayout);


    leftQWidgetGroupBox2 = new QGroupBox("进程信息", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout2 = new QVBoxLayout(leftQWidgetGroupBox2);


    auto tabWidget = new QTabWidget(this);
    auto tabMessage = new QWidget(tabWidget);
    auto tabMessageLayout = new QHBoxLayout(this);
    auto tabMessage1Layout = new QVBoxLayout(this);
    auto tabMessage2Layout = new QVBoxLayout(this);
    tabMessageLayout->addLayout(tabMessage1Layout);
    tabMessageLayout->addLayout(tabMessage2Layout);
    tabMessage->setLayout(tabMessageLayout);
    tabWidget->addTab(tabMessage, tr("消息捕获"));


    edtMessage = new QPlainTextEdit();
    edtMessage->setReadOnly(true);
    tabMessage1Layout->addWidget(edtMessage);

    auto gbMessageButton = new QGroupBox("操作", this);
    auto gbMessageButtonLayout = new QVBoxLayout(this);
    gbMessageButton->setLayout(gbMessageButtonLayout);


    auto gbMessageButtonLayout1 = new QHBoxLayout();
    auto gbMessageButtonLayout2 = new QHBoxLayout();
    auto gbMessageButtonLayout3 = new QHBoxLayout();
    auto gbMessageButtonLayout4 = new QHBoxLayout();
    auto gbMessageButtonLayout5 = new QHBoxLayout();
    gbMessageButtonLayout1->setAlignment(Qt::AlignLeft);
    gbMessageButtonLayout2->setAlignment(Qt::AlignLeft);
    gbMessageButtonLayout3->setAlignment(Qt::AlignLeft);
    gbMessageButtonLayout4->setAlignment(Qt::AlignLeft);
    gbMessageButtonLayout5->setAlignment(Qt::AlignLeft);
    gbMessageButtonLayout->addLayout(gbMessageButtonLayout1);
    gbMessageButtonLayout->addLayout(gbMessageButtonLayout2);
    gbMessageButtonLayout->addLayout(gbMessageButtonLayout3);
    gbMessageButtonLayout->addLayout(gbMessageButtonLayout4);
    gbMessageButtonLayout->addLayout(gbMessageButtonLayout5);


    gbMessageButtonLayout1->addWidget(             new QLabel("窗口名称:"), 0, Qt::AlignLeft);
    gbMessageButtonLayout1->addWidget(  tb_window_name,                 0, Qt::AlignLeft);
    gbMessageButtonLayout1->addWidget(             new QLabel("句柄:"),   0, Qt::AlignLeft);
    gbMessageButtonLayout1->addWidget(tb_window_handle,                 0, Qt::AlignLeft);
    gbMessageButtonLayout1->addWidget(btnRefreshWindow,                 0, Qt::AlignLeft);
    gbMessageButtonLayout1->addWidget(   btnFindWindow,                 0, Qt::AlignLeft);

    gbMessageButtonLayout2->addWidget(               new QLabel("窗口大小:", this), 0, Qt::AlignLeft);
    gbMessageButtonLayout2->addWidget(    tb_window_size,                       0, Qt::AlignLeft);
    gbMessageButtonLayout2->addWidget(               new QLabel("坐标:", this),   0, Qt::AlignLeft);
    gbMessageButtonLayout2->addWidget(tb_window_position,                       0, Qt::AlignLeft);

    gbMessageButtonLayout3->addWidget(                new QLabel("内部大小:", this), 0, Qt::AlignLeft);
    gbMessageButtonLayout3->addWidget(    tb_window2_size,                       0, Qt::AlignLeft);
    gbMessageButtonLayout3->addWidget(                new QLabel("坐标:", this),   0, Qt::AlignLeft);
    gbMessageButtonLayout3->addWidget(tb_window2_position,                       0, Qt::AlignLeft);


    btnMouseHookStart = new QPushButton("鼠标钩子", this);
    btnMessageHookStart = new QPushButton("消息钩子", this);


    gbMessageButtonLayout4->addWidget(  btnMouseHookStart, 0, Qt::AlignLeft);
    gbMessageButtonLayout4->addWidget(btnMessageHookStart, 0, Qt::AlignLeft);

    auto gbWindows = new QGroupBox("窗口明细", this);
    auto gbWindowsLayout = new QHBoxLayout(this);
    gbWindows->setLayout(gbWindowsLayout);

    tvWindows = new QTreeView(this);
    imWindows = new QStandardItemModel(this);

    tvWindows->setTreePosition(1);
    imWindows->setHorizontalHeaderLabels({ "选择",  "句柄",  "编号", "标题", "类", "窗口大小" });

    tvWindows->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //    WindowsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tvWindows->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tvWindows->setSelectionBehavior(QAbstractItemView::SelectRows);
    tvWindows->setModel(imWindows);
    gbWindowsLayout->addWidget(tvWindows);


    tabMessage2Layout->addWidget(gbMessageButton);
    tabMessage2Layout->addWidget(      gbWindows);


    leftQWidgetGroup1Layout2->addWidget(tabWidget);


    auto leftQWidgetGroupBox3 = new QGroupBox("控制台设置", this);
    auto leftQWidgetGroup1Layout3 = new QHBoxLayout(leftQWidgetGroupBox3);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox3);

    edtMsg = new QPlainTextEdit();
    edtMsg->setFixedHeight(150);
    edtMsg->setReadOnly(true);
    leftQWidgetLayout->addWidget(edtMsg);


    // 第一层 (中间列)
    //    auto centerQWidget = new QWidget(this);
    //    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);


    // 第二层(中间列)


    // 第一层(右边列)

    // 第二层(右边列)


    // 第三层(右边列)


    // 第四层(窗口状态)


    // 第四层(窗口明细)


    ckConsoleEnable = new QCheckBox("启动控制台");
    ckConsoleEnable->setChecked(true);
    btnConsoleClear = new QPushButton("清空控制台");


    leftQWidgetGroup1aLayout->addWidget(              new QLabel("进程名称:"), 0, Qt::AlignLeft);
    leftQWidgetGroup1aLayout->addWidget(  tb_process_name,                 0, Qt::AlignLeft);
    leftQWidgetGroup1aLayout->addWidget(              new QLabel("窗口句柄:"), 0, Qt::AlignLeft);
    leftQWidgetGroup1aLayout->addWidget(tb_window_handle2,                 0, Qt::AlignLeft);


    leftQWidgetGroup1Layout3->addWidget(ckConsoleEnable);
    leftQWidgetGroup1Layout3->addWidget(btnConsoleClear);


    layout->addWidget(leftQWidget);

    g_moduleMessage = LoadLibrary(TEXT("C:\\WorkMe\\wych-csharp-project\\Debug\\HookMessageDll.dll"));

    if (g_moduleMessage)
    {
        hkMouseProc = (HOOKPROC)GetProcAddress(g_moduleMessage, "MouseProc");

        if (hkMouseProc == NULL) {
            appendConsole(tr("函数MouseProc获取失败！ LastError: %1").arg(GetLastError()));
            qDebug() << "函数：MouseProc获取失败！ LastError" << GetLastError();
        }

        hkMessageProc = (HOOKPROC)GetProcAddress(g_moduleMessage, "MessageProc");

        if (hkMessageProc == NULL) {
            appendConsole(tr("函数MessageProc获取失败！ LastError: %1").arg(GetLastError()));
            qDebug() << "函数：MessageProc获取失败！ LastError" << GetLastError();
        }

        //        hkSetHook = (SetHook)GetProcAddress(g_moduleMessage, "SetHook");

        //        if (hkSetHook == NULL) {
        //            qDebug() << "SetHook LastError" << GetLastError();
        //        }


        // 没用
        //        hkSetSysMsgCall = (SetSysMsgCall)GetProcAddress(g_moduleMessage, "setSysMsgCall");
    }

    //    hkSetSysMsgCall((SysMsgCallBack)MysysMsgCallBack);


    // 这行必须放下面
}

int __stdcall Itemview10ProcessStatus::MysysMsgCallBack(int    nCode,
                                                        WPARAM wParam,
                                                        LPARAM lParam) {
    qDebug() << "nCode:" << nCode << "wParam:" << wParam << "lParam:" << lParam;
    return 1;
}

void Itemview10ProcessStatus::initConnect()
{
    // 事件 - 启动hook
    connect(btnMouseHookStart, &QPushButton::clicked, [this]() {
        if (g_messageHook) {
            if (!UnhookWindowsHookEx(g_messageHook)) {
                appendConsole(tr("UnhookWindowsHookEx LastError: %1").arg(GetLastError()));
                qDebug() << "UnhookWindowsHookEx LastError" << GetLastError();
            } else {
                g_messageHook = NULL;
                btnMouseHookStart->setText("鼠标钩子");
            }
            return;
        }

        if (windowInfo.HandleWindow) {
            // 获取线程id
            DWORD dwThreadId = GetWindowThreadProcessId(windowInfo.HandleWindow, NULL);

            g_messageHook = SetWindowsHookEx(WH_MOUSE, hkMouseProc, g_moduleMessage, dwThreadId);

            if (g_messageHook == NULL) {
                appendConsole(tr("hkMouseProc LastError: %1").arg(GetLastError()));
                qDebug() << "LastError" << GetLastError() << "hkSysMsgProc" << hkMouseProc;
                qDebug() << "g_moduleMessage" << g_moduleMessage << "dwThreadId" << dwThreadId;
            } else {
                btnMouseHookStart->setText("关闭钩子");
            }
        }
    });

    connect(btnMessageHookStart, &QPushButton::clicked, [this]() {
        if (g_messageHook) {
            if (!UnhookWindowsHookEx(g_messageHook)) {
                appendConsole(tr("UnhookWindowsHookEx LastError: %1").arg(GetLastError()));
                qDebug() << "UnhookWindowsHookEx LastError" << GetLastError();
            } else {
                g_messageHook = NULL;
                btnMessageHookStart->setText("消息钩子");
            }
            return;
        }

        if (windowInfo.HandleWindow) {
            // 获取线程id
            DWORD dwThreadId = GetWindowThreadProcessId(windowInfo.HandleWindow, NULL);

            g_messageHook = SetWindowsHookEx(WH_GETMESSAGE, hkMessageProc, g_moduleMessage, dwThreadId);

            if (g_messageHook == NULL) {
                appendConsole(tr("hkMessageProc！ LastError: %1").arg(GetLastError()));
                qDebug() << "LastError" << GetLastError() << "hkSysMsgProc" << hkMessageProc;
                qDebug() << "g_moduleMessage" << g_moduleMessage << "dwThreadId" << dwThreadId;
            } else {
                btnMessageHookStart->setText("关闭钩子");
            }
        }
    });

    // 事件 - 刷新窗口
    connect(btnRefreshWindow, &QPushButton::clicked, [this]() {
        updateWindowInfo(windowInfo.HandleWindow);
    });


    // 事件 - 窗口表双击
    connect(tvWindows, &QTreeView::doubleClicked, [this](const QModelIndex& current) {
        // auto rowIndex = WindowsGridModel->itemFromIndex(current)->row();


        bool ok;
        auto currentHandleV = current.sibling(current.row(), 1).data().toString();
        auto currentHandle = currentHandleV.toInt(&ok, 16);

        // 打勾
        QList<QStandardItem *>items =  imWindows->findItems("✔", Qt::MatchExactly  | Qt::MatchRecursive, 0);

        if (items.count() == 1) {
            auto *firstItem = items.first();
            firstItem->setData("", Qt::DisplayRole);
        }
        auto item0 = imWindows->itemFromIndex(current.siblingAtColumn(0));
        item0->setData("✔", Qt::DisplayRole);

        // 打勾

        if (ok) {
            updateWindowInfo((HWND)currentHandle);
        }
    });
}

bool Itemview10ProcessStatus::buildProcess(DWORD pid) {
    BOOL success = WinAPI::get_process_info(pid, &processInfo);

    if (success) {
        success = WinAPI::get_window_main(pid, &windowInfo);

        if (success) {
            tb_process_name->setText(tr("[%1] %2 ").arg(pid).arg(QString::fromWCharArray(processInfo.PName)));
            tb_window_name->setText(QString::fromWCharArray(windowInfo.TitleName));
            tb_window_handle->setText(QString::number((intptr_t)(windowInfo.HandleWindow), 16).toUpper());
            tb_window_handle2->setText(QString::number((intptr_t)(windowInfo.HandleWindow), 16).toUpper());
            tb_window_size->setText(tr("[ %1 x %2 ]").arg(windowInfo.WindowRect.right - windowInfo.WindowRect.left).arg(windowInfo.WindowRect.bottom - windowInfo.WindowRect.top));
            tb_window_position->setText(tr("(%1,%2),(%3,%4)").arg(windowInfo.WindowRect.left).arg(windowInfo.WindowRect.top).arg(windowInfo.WindowRect.right).arg(windowInfo.WindowRect.bottom));
            tb_window2_size->setText(tr("[ %1 x %2 ]").arg(windowInfo.ClientRect.right - windowInfo.ClientRect.left).arg(windowInfo.ClientRect.bottom - windowInfo.ClientRect.top));
            tb_window2_position->setText(tr("(%1,%2),(%3,%4)").arg(windowInfo.ClientToScreen.x).arg(windowInfo.ClientToScreen.y).arg(windowInfo.ClientToScreen.x + windowInfo.ClientRect.right).arg(windowInfo.ClientToScreen.y + windowInfo.ClientRect.bottom));


            imWindows->setItem(0, 0, new QStandardItem(""));
            imWindows->setItem(0, 1, new QStandardItem(QString::number((intptr_t)(windowInfo.HandleWindow), 16).toUpper()));
            imWindows->setItem(0, 2, new QStandardItem(QString::number(0)));
            imWindows->setItem(0, 3, new QStandardItem(QString::fromWCharArray(windowInfo.TitleName)));
            imWindows->setItem(0, 4, new QStandardItem(QString::fromWCharArray(windowInfo.ClassName)));


            // 子窗口
            std::vector<WIN32_WINDOW_INFO> children;
            success = WinAPI::get_window_child(windowInfo.HandleWindow, children);

            for (std::vector<WIN32_WINDOW_INFO>::iterator iter = children.begin(); iter != children.end(); ++iter) {
                int index = std::distance(children.begin(), iter) + 1;

                QString itemHandle = QString::number((intptr_t)(iter->HandleParentWindow), 16).toUpper();

                QList<QStandardItem *> parents =  imWindows->findItems(itemHandle, Qt::MatchExactly  | Qt::MatchRecursive, 1);

                if (parents.count() == 1) {
                    QStandardItem *rowItem = parents.first();

                    // 只能这么转
                    QModelIndex index1 =  imWindows->indexFromItem(rowItem);
                    QModelIndex index0 =  index1.siblingAtColumn(0);
                    QStandardItem *item0 = imWindows->itemFromIndex(index0);

                    int rowCount = item0->rowCount();
                    item0->setChild(rowCount, 1, new QStandardItem(QString::number((intptr_t)(iter->HandleWindow), 16).toUpper()));

                    //                    rowItem->setChild(0, 1, new QStandardItem(itemHandle));
                    item0->setChild(rowCount, 3, new QStandardItem(QString::fromWCharArray(iter->TitleName)));
                    item0->setChild(rowCount, 4, new QStandardItem(QString::fromWCharArray(iter->ClassName)));
                    item0->setChild(rowCount, 2, new QStandardItem(QString::number(index)));
                    item0->setChild(rowCount, 0, new QStandardItem(""));
                }
            }


            tvWindows->expandAll();

            imWindows->item(0)->setData("✔", Qt::DisplayRole);
            return true;
        }
    }

    return false;
}

bool Itemview10ProcessStatus::updateWindowInfo(HWND handleWindow) {
    BOOL success = WinAPI::get_window_info(handleWindow, &windowInfo);

    tb_window_name->setText(QString::fromWCharArray(windowInfo.TitleName));
    tb_window_handle->setText(QString::number((intptr_t)(windowInfo.HandleWindow), 16).toUpper());
    tb_window_handle2->setText(QString::number((intptr_t)(windowInfo.HandleWindow), 16).toUpper());
    tb_window_size->setText(tr("[ %1 x %2 ]").arg(windowInfo.WindowRect.right - windowInfo.WindowRect.left).arg(windowInfo.WindowRect.bottom - windowInfo.WindowRect.top));
    tb_window_position->setText(tr("(%1,%2),(%3,%4)").arg(windowInfo.WindowRect.left).arg(windowInfo.WindowRect.top).arg(windowInfo.WindowRect.right).arg(windowInfo.WindowRect.bottom));
    tb_window2_size->setText(tr("[ %1 x %2 ]").arg(windowInfo.ClientRect.right - windowInfo.ClientRect.left).arg(windowInfo.ClientRect.bottom - windowInfo.ClientRect.top));
    tb_window2_position->setText(tr("(%1,%2),(%3,%4)").arg(windowInfo.ClientToScreen.x).arg(windowInfo.ClientToScreen.y).arg(windowInfo.ClientToScreen.x + windowInfo.ClientRect.right).arg(windowInfo.ClientToScreen.y + windowInfo.ClientRect.bottom));


    return TRUE;
}

bool Itemview10ProcessStatus::readConfigDefaultHWND(QString process, int *HWND)
{
    QFile file(tr("./configs/defaultHWND.json"));

    if (!file.exists()) {
        appendConsole(tr("读取异常！配置文件不存在！"));
        return false;
    }
    QByteArray fileText;

    if (file.open(QIODevice::ReadOnly)) {
        fileText = file.readAll();
        file.close();
    }

    // 读取json
    QJsonParseError parseError;
    QJsonDocument   document = QJsonDocument::fromJson(fileText, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        appendConsole(tr("读取异常！配置文件无法读取！"));
        return false;
    }
    QJsonObject json = document.object();

    if (!json.contains(process)) {
        return false;
    }
    *HWND = json[process].toInt();
    return true;
}

bool Itemview10ProcessStatus::writeConfigDefaultHWND(QString process, int HWND)
{
    QDir dir(tr("./configs/"));

    if (!dir.exists()) {
        dir.mkpath("./");
    }

    QFile file(tr("./configs/defaultHWND.json"));

    QJsonObject   json;
    QJsonDocument document;

    if (file.exists()) {
        QByteArray fileText;

        if (file.open(QIODevice::ReadOnly)) {
            fileText = file.readAll();
            file.close();
        }

        // 读取json
        QJsonParseError parseError;
        QJsonDocument   document2 = QJsonDocument::fromJson(fileText, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            appendConsole(tr("读取异常！配置文件无法读取！"));
            return false;
        }

        json = document2.object();
    }


    json[process] = HWND;

    document.setObject(json);
    QByteArray jsonStr = document.toJson(QJsonDocument::Compact);


    QFile file2(tr("./configs/defaultHWND.json"));

    if (file2.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file2.write(jsonStr);
        file2.close();
    }
    return true;
}

void Itemview10ProcessStatus::postAppendConsole(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg, qEventAppendConsole);

    QApplication::postEvent(this, event);
}

// void Itemview10ProcessStatus::postAppendMessageText(EventWinMessage *event) {
//    EventWinMessage *event2 = new EventWinMessage(qEventMouseProc, 1, 1);


//    QApplication::postEvent(this, event2);
// }


void Itemview10ProcessStatus::buildMessageText(QEvent::Type type, EventWinMessage *event) {
    switch (type) {
    case qEventMouseProc:
    {
        //不能用LPMOUSEHOOKSTRUCT。。。。 也不能用LOWORD HIWORD
        WPARAM *wParam =  (WPARAM *)(event->data + sizeof(int));
        PMOUSEHOOKSTRUCT lParam = (PMOUSEHOOKSTRUCT)(event->data + sizeof(int) + sizeof(WPARAM));

        QString mouseType;

        switch (*wParam) {
        case WM_LBUTTONDOWN:
            mouseType = "左键按下";
            break;

        case WM_LBUTTONUP:
            mouseType = "左键释放";
            break;

        case WM_LBUTTONDBLCLK:
            mouseType = "左键双击";
            break;

        case WM_RBUTTONDOWN:
            mouseType = "右键按下";
            break;

        case WM_RBUTTONUP:
            mouseType = "右键释放";
            break;

        case WM_RBUTTONDBLCLK:
            mouseType = "右键双击";
            break;

        case WM_MBUTTONDOWN:
            mouseType = "中键按下";
            break;

        case WM_MBUTTONUP:
            mouseType = "中键释放";
            break;

        case WM_MBUTTONDBLCLK:
            mouseType = "中键双击";
            break;

        case WM_MOUSEWHEEL:
            mouseType = "滚轮";
            break;

        case WM_MOUSEMOVE:
            mouseType = "移动";
            break;

        case WM_NCLBUTTONDOWN:
            mouseType = "NC左键按下";
            break;

        case WM_NCLBUTTONUP:
            mouseType = "NC左键释放";
            break;

        case WM_NCLBUTTONDBLCLK:
            mouseType = "NC左键双击";
            break;

        case WM_NCRBUTTONDOWN:
            mouseType = "NC右键按下";
            break;

        case WM_NCRBUTTONUP:
            mouseType = "NC右键释放";
            break;

        case WM_NCRBUTTONDBLCLK:
            mouseType = "NC右键双击";
            break;

        case WM_NCMBUTTONDOWN:
            mouseType = "NC中键按下";
            break;

        case WM_NCMBUTTONUP:
            mouseType = "NC中键释放";
            break;


        case WM_NCMBUTTONDBLCLK:
            mouseType = "NC中键双击";
            break;


        case WM_NCMOUSEMOVE:
            mouseType = "NC移动";
            break;


        default:
            mouseType = "未知" + QString::number(*wParam, 16);
            break;
        }

        //        qDebug() << mouseType;
        //        qDebug() << lParam->pt.x;
        //        qDebug() << lParam->pt.y;
        //        qDebug() << lParam->hwnd;

        QString msg = tr("[鼠标] %1 坐标：(%2,%3) hwnd：%4 命中测试值：%5")
                      .arg(mouseType).arg(lParam->pt.x).arg(lParam->pt.y)
                      .arg(  QString::number(UINT(lParam->hwnd), 16))
                      .arg(   lParam->wHitTestCode);
        appendMessageText(msg);
        break;
    }

    case qEventMessageMouseProc:
    {
        char *p = event->data;

        HWND   *hwnd =  (HWND *)(p += sizeof(int));
        UINT   *msg =  (UINT *)(p += sizeof(HWND));
        WPARAM *wParam =  (WPARAM *)(p += sizeof(UINT));
        int    *x =  (int *)(p += sizeof(int));
        int    *y =  (int *)(p += sizeof(int));


        QString mouseType;

        switch (*msg) {
        case WM_LBUTTONDOWN:
            mouseType = "左键按下";
            break;

        case WM_LBUTTONUP:
            mouseType = "左键释放";
            break;

        case WM_LBUTTONDBLCLK:
            mouseType = "左键双击";
            break;

        case WM_RBUTTONDOWN:
            mouseType = "右键按下";
            break;

        case WM_RBUTTONUP:
            mouseType = "右键释放";
            break;

        case WM_RBUTTONDBLCLK:
            mouseType = "右键双击";
            break;

        case WM_MBUTTONDOWN:
            mouseType = "中键按下";
            break;

        case WM_MBUTTONUP:
            mouseType = "中键释放";
            break;

        case WM_MBUTTONDBLCLK:
            mouseType = "中键双击";
            break;

        case WM_MOUSEWHEEL:
            mouseType = "滚轮";
            break;

        case WM_MOUSEMOVE:
            mouseType = "移动";
            break;

        case WM_NCLBUTTONDOWN:
            mouseType = "NC左键按下";
            break;

        case WM_NCLBUTTONUP:
            mouseType = "NC左键释放";
            break;

        case WM_NCLBUTTONDBLCLK:
            mouseType = "NC左键双击";
            break;

        case WM_NCRBUTTONDOWN:
            mouseType = "NC右键按下";
            break;

        case WM_NCRBUTTONUP:
            mouseType = "NC右键释放";
            break;

        case WM_NCRBUTTONDBLCLK:
            mouseType = "NC右键双击";
            break;

        case WM_NCMBUTTONDOWN:
            mouseType = "NC中键按下";
            break;

        case WM_NCMBUTTONUP:
            mouseType = "NC中键释放";
            break;


        case WM_NCMBUTTONDBLCLK:
            mouseType = "NC中键双击";
            break;


        case WM_NCMOUSEMOVE:
            mouseType = "NC移动";
            break;


        default:
            mouseType = "未知" + QString::number(*wParam, 16);
            break;
        }


        QString text = tr("[消息-鼠标] %1 坐标：(%2,%3) hwnd：%4 wParam：%5")
                       .arg(mouseType)
                       .arg(        *x)
                       .arg(        *y)
                       .arg(  QString::number(UINT(*hwnd), 16))
                       .arg(  QString::number(UINT(*wParam), 16));
        appendMessageText(text);
        break;
    }
    }
}

void Itemview10ProcessStatus::appendMessageText(const QString& msg) {
    QString text = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    text += msg;

    edtMessage->appendPlainText(text);
    QTextCursor cursor = edtMessage->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMessage->setTextCursor(cursor);
    edtMessage->repaint();
}

void Itemview10ProcessStatus::appendConsole(const QString& msg)
{
    QString text = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    text += msg;
    edtMsg->appendPlainText(text);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void Itemview10ProcessStatus::writeConsole(const QString& msg)
{
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void Itemview10ProcessStatus::clearConsole()
{
    edtMsg->clear();
}

void Itemview10ProcessStatus::postUpdateScriptStatus(QStandardItem *item, EventStatusGrid::GridType type)
{
    EventStatusGrid *event = new EventStatusGrid(item, type, qEventStatusGridScript);

    QApplication::postEvent(this, event);
}

void Itemview10ProcessStatus::postUpdateScriptCount(QStandardItem *item)
{
    EventStatusGrid *event = new EventStatusGrid(item, qEventCountGridScript);

    QApplication::postEvent(this, event);
}

void Itemview10ProcessStatus::updateScriptStatus(QStandardItem *item, EventStatusGrid::GridType type) {
    switch (type) {
    case EventStatusGrid::Default:

        item->setData(    "",            Qt::DisplayRole);
        item->setData(QColor(Qt::black), Qt::ForegroundRole);
        break;

    case EventStatusGrid::Success:

        item->setData(  "成功",            Qt::DisplayRole);
        item->setData(QColor(Qt::green), Qt::ForegroundRole);
        break;

    case EventStatusGrid::Running:

        item->setData( "执行中",           Qt::DisplayRole);
        item->setData(QColor(Qt::blue), Qt::ForegroundRole);
        break;

    case EventStatusGrid::Failure:

        item->setData(  "失败",          Qt::DisplayRole);
        item->setData(QColor(Qt::red), Qt::ForegroundRole);
        break;
    }
}

void Itemview10ProcessStatus::updateScriptCount(QStandardItem *item) {
    auto item4 = scriptGridModel->itemFromIndex(scriptGridModel->indexFromItem(item).siblingAtColumn(4));
    int  count = item4->data(Qt::DisplayRole).toInt();

    item4->setData(count + 1, Qt::DisplayRole);
}

void Itemview10ProcessStatus::customEvent(QEvent *e)
{
    switch (e->type())
    {
    case qEventAppendConsole:
    {
        QStringEvent *event = dynamic_cast<QStringEvent *>(e);
        appendConsole(event->message);
        e->accept();
        break;
    }

    case qEventMouseProc:
    case qEventMessageMouseProc:
    {
        EventWinMessage *event = dynamic_cast<EventWinMessage *>(e);
        buildMessageText(e->type(), event);
        e->accept();
        break;
    }


    default:
        break;
    }
}

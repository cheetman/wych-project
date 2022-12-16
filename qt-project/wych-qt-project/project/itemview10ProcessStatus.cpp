

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
#include <QByteArray>
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


    auto topSplitter = new QSplitter(Qt::Horizontal, this);


    auto top1QGroupBox = new QGroupBox("状态", this);

    auto top1QGroupBoxLayout = new QVBoxLayout();
    auto top1QGroupBoxLayout1 = new QHBoxLayout();
    auto top1QGroupBoxLayout2 = new QHBoxLayout();
    auto top1QGroupBoxLayout3 = new QHBoxLayout();
    auto top1QGroupBoxLayout4 = new QHBoxLayout();
    auto top1QGroupBoxLayout5 = new QHBoxLayout();
    top1QGroupBoxLayout->setAlignment(Qt::AlignTop);
    top1QGroupBoxLayout1->setAlignment(Qt::AlignLeft);
    top1QGroupBoxLayout2->setAlignment(Qt::AlignLeft);
    top1QGroupBoxLayout3->setAlignment(Qt::AlignLeft);
    top1QGroupBoxLayout4->setAlignment(Qt::AlignLeft);
    top1QGroupBoxLayout5->setAlignment(Qt::AlignLeft);
    top1QGroupBox->setLayout(top1QGroupBoxLayout);
    top1QGroupBoxLayout->addLayout(top1QGroupBoxLayout1);
    top1QGroupBoxLayout->addLayout(top1QGroupBoxLayout2);
    top1QGroupBoxLayout->addLayout(top1QGroupBoxLayout3);
    top1QGroupBoxLayout->addLayout(top1QGroupBoxLayout4);
    top1QGroupBoxLayout->addLayout(top1QGroupBoxLayout5);

    topSplitter->addWidget(top1QGroupBox);


    auto top2QGroupBox = new QGroupBox("控制台设置2", this);
    auto top2QGroupBoxLayout = new QVBoxLayout(this);
    auto top2QGroupBoxLayout1 = new QHBoxLayout(this);
    auto top2QGroupBoxLayout2 = new QHBoxLayout(this);
    top2QGroupBoxLayout->addLayout(top2QGroupBoxLayout1);
    top2QGroupBoxLayout->addLayout(top2QGroupBoxLayout2);
    top2QGroupBox->setLayout(top2QGroupBoxLayout);

    ckConsoleEnable = new QCheckBox("启动控制台");
    ckConsoleEnable->setChecked(true);
    btnConsoleClear = new QPushButton("清空控制台");
    edtMsg = new QPlainTextEdit();
    edtMsg->setFixedHeight(150);
    edtMsg->setReadOnly(true);

    top2QGroupBoxLayout1->addWidget(ckConsoleEnable);
    top2QGroupBoxLayout1->addWidget(btnConsoleClear);
    top2QGroupBoxLayout2->addWidget(edtMsg);


    topSplitter->addWidget(top2QGroupBox);


    leftQWidgetLayout->addWidget(topSplitter);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);


    leftQWidgetGroupBox2 = new QGroupBox("进程信息", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout2 = new QVBoxLayout(leftQWidgetGroupBox2);


    auto tabWidget = new QTabWidget(this);
    auto tabMessage = new QWidget(tabWidget);
    auto tabMessageLayout = new QHBoxLayout(this);

    //    auto tabMessage1Layout = new QVBoxLayout(this);
    auto tabMessage2Layout = new QVBoxLayout(this);
    auto tabMessageSplitter = new QSplitter(Qt::Horizontal, this);
    tabMessageLayout->addWidget(tabMessageSplitter);

    //    tabMessageLayout->addLayout(tabMessage1Layout);
    //    tabMessageLayout->addLayout(tabMessage2Layout);
    auto tabMessageSplitterWidget2 = new QWidget(this);
    tabMessageSplitterWidget2->setLayout(tabMessage2Layout);
    tabMessage->setLayout(tabMessageLayout);
    tabWidget->addTab(tabMessage, tr("消息捕获"));


    edtMessage = new QPlainTextEdit();
    edtMessage->setReadOnly(true);
    tabMessageSplitter->addWidget(edtMessage);

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
    btnShowStyle = new QPushButton("查看窗口样式", this);
    btnHideWindow = new QPushButton("隐藏窗口", this);
    ckHideWindow = new QCheckBox("轮询");

    //    ckHideWindow->setCheckState(Qt::Checked);

    gbMessageButtonLayout4->addWidget(       btnShowStyle, 0, Qt::AlignLeft);
    gbMessageButtonLayout4->addWidget(  btnMouseHookStart, 0, Qt::AlignLeft);
    gbMessageButtonLayout4->addWidget(btnMessageHookStart, 0, Qt::AlignLeft);

    gbMessageButtonLayout5->addWidget(btnHideWindow, 0, Qt::AlignLeft);
    gbMessageButtonLayout5->addWidget( ckHideWindow, 0, Qt::AlignLeft);

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

    tabMessageSplitter->addWidget(tabMessageSplitterWidget2);

    tabMessage2Layout->addWidget(gbMessageButton);
    tabMessage2Layout->addWidget(      gbWindows);


    leftQWidgetGroup1Layout2->addWidget(tabWidget);


    // 第一层 (中间列)
    //    auto centerQWidget = new QWidget(this);
    //    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);


    // 第二层(中间列)


    // 第一层(右边列)

    // 第二层(右边列)


    // 第三层(右边列)


    // 第四层(窗口状态)


    // 第四层(窗口明细)


    top1QGroupBoxLayout1->addWidget(              new QLabel("进程名称:"), 0, Qt::AlignLeft);
    top1QGroupBoxLayout1->addWidget(  tb_process_name,                 0, Qt::AlignLeft);
    top1QGroupBoxLayout1->addWidget(              new QLabel("窗口句柄:"), 0, Qt::AlignLeft);
    top1QGroupBoxLayout1->addWidget(tb_window_handle2,                 0, Qt::AlignLeft);


    layout->addWidget(leftQWidget);

    //    g_moduleMessage = LoadLibrary(TEXT("C:\\WorkMe\\wych-csharp-project\\Debug\\HookMessageDll.dll"));
    //    g_moduleMessage = LoadLibrary(TEXT("C:\\WorkMe\\wych-csharp-project\\Release\\HookMessageDll.dll"));
    g_moduleMessage = LoadLibrary(TEXT("HookMessageDll.dll"));

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
    } else {
        appendConsole(tr("钩子DLL加载失败！ LastError: %1").arg(GetLastError()));
    }

    //    hkSetSysMsgCall((SysMsgCallBack)MysysMsgCallBack);


    threadWindowHide = new ThreadWindowHide(this);
    threadWindowHide->setParent(this);
}

int __stdcall Itemview10ProcessStatus::MysysMsgCallBack(int    nCode,
                                                        WPARAM wParam,
                                                        LPARAM lParam) {
    qDebug() << "nCode:" << nCode << "wParam:" << wParam << "lParam:" << lParam;
    return 1;
}

bool Itemview10ProcessStatus::showWindow(HWND hwnd) {
    long style = GetWindowLong(hwnd, GWL_EXSTYLE);

    if (style & WS_EX_TOOLWINDOW) {
        style &= ~(WS_EX_TOOLWINDOW);
        style |= WS_EX_APPWINDOW;
        SetWindowLong(hwnd, GWL_EXSTYLE, style);


        WIN32_WINDOW_INFO windowInfo = { 0 };
        BOOL success = WinAPI::get_window_info(hwnd, &windowInfo);

        if (success) {
            // 确认下是不是我拖的
            if (windowInfo.WindowRect.right == 0) {
                int w =  windowInfo.WindowRect.right -  windowInfo.WindowRect.left;
                int h =  windowInfo.WindowRect.bottom -  windowInfo.WindowRect.top;

                // 稍微修正一下
                if (lastX < 0) {
                    lastX = 10;
                }

                BOOL result = MoveWindow(hwnd, lastX, windowInfo.WindowRect.top, w, h, true);

                return true;
            } else {
                return false;
            }
        }
    }
}

bool Itemview10ProcessStatus::hideWindow(HWND hwnd) {
    long style = GetWindowLong(hwnd, GWL_EXSTYLE);

    qDebug() << hwnd;

    style &= ~(WS_EX_APPWINDOW);
    style |= WS_EX_TOOLWINDOW;
    SetWindowLong(hwnd, GWL_EXSTYLE, style);


    WIN32_WINDOW_INFO windowInfo = { 0 };
    bool success = WinAPI::get_window_info(hwnd, &windowInfo);
    int  w =  windowInfo.WindowRect.right -  windowInfo.WindowRect.left;
    int  h =  windowInfo.WindowRect.bottom -  windowInfo.WindowRect.top;

    qDebug() << w;
    qDebug() << h;

    // 应该是已经被我隐藏了
    if (windowInfo.WindowRect.right == 0) {
        return true;
    }

    BOOL result = MoveWindow(hwnd, -w, windowInfo.WindowRect.top, w, h, true);

    lastX = windowInfo.WindowRect.left;

    if (windowInfo.WindowRect.right <= 0) {
        lastX = 10;
    }
    qDebug() << result;
    return true;
}

void Itemview10ProcessStatus::initConnect()
{
    connect(btnFindWindow, &CaptureBtn::hwndEvent, [this](HWND hwnd) {
        if (updateWindowInfo(hwnd)) {
            showWindowStyle(windowInfo.HandleWindow);
        }
    });

    // 事件 - 线程通知
    connect(threadWindowHide, &ThreadWindowHide::threadFinished, [this]() {
        btnHideWindow->setText(tr("隐藏窗口"));
        hideStatus = 0;
    });


    // 事件 -
    connect(btnHideWindow, &QPushButton::clicked, [this]() {
        if (hideStatus == 1) {
            if (!IsWindow(lasthwnd)) {
                appendConsole(tr("被隐藏的程序已退出！"));
                btnHideWindow->setText("隐藏窗口");
                hideStatus = 0;
                return;
            }

            if (showWindow(lasthwnd)) {
                btnHideWindow->setText("隐藏窗口");
                appendConsole(tr("窗口已还原！"));
                hideStatus = 0;
            } else {
                appendConsole(tr("还原失败！"));
                btnHideWindow->setText("隐藏窗口");
                hideStatus = 0;
            }
        } else if (hideStatus == 0) {
            if (!IsWindow(windowInfo.HandleWindow)) {
                appendConsole(tr("窗口不存在！"));
                return;
            }
            HWND hwnd = GetAncestor(windowInfo.HandleWindow, GA_ROOT);

            if (ckHideWindow->isChecked()) {
                WIN32_WINDOW_INFO windowInfo = { 0 };
                bool success = WinAPI::get_window_info(hwnd, &lastWindowInfo);
                lasthwnd = hwnd;
                btnHideWindow->setText("正在隐藏中..");
                hideStatus = 2;
                threadWindowHide->start();
            } else {
                if (hideWindow(hwnd)) {
                    hideStatus = 1;
                    lasthwnd = hwnd;
                    btnHideWindow->setText("还原窗口");
                    appendConsole(tr("窗口已隐藏！"));
                }
            }
        }
        else if (hideStatus == 2) {
            btnHideWindow->setText("正在取消中..");
            hideStatus = 3;
        }
    });

    // 事件 -
    connect(btnShowStyle, &QPushButton::clicked, [this]() {
        if (windowInfo.HandleWindow) {
            qDebug() << windowInfo.HandleWindow;

            showWindowStyle(windowInfo.HandleWindow);
        }
    });


    // 事件 - 启动hook
    connect(btnMouseHookStart, &QPushButton::clicked, [this]() {
        if (g_mouseHook) {
            if (!UnhookWindowsHookEx(g_mouseHook)) {
                appendConsole(tr("UnhookWindowsHookEx LastError: %1").arg(GetLastError()));
                qDebug() << "UnhookWindowsHookEx LastError" << GetLastError();
            } else {
                g_mouseHook = NULL;
                btnMouseHookStart->setText("鼠标钩子");
            }
            return;
        }

        if (windowInfo.HandleWindow) {
            // 获取线程id
            DWORD dwThreadId = GetWindowThreadProcessId(windowInfo.HandleWindow, NULL);

            g_mouseHook = SetWindowsHookEx(WH_MOUSE, hkMouseProc, g_moduleMessage, dwThreadId);

            if (g_mouseHook == NULL) {
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


            showWindowStyle(windowInfo.HandleWindow);
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

void Itemview10ProcessStatus::showWindowStyle(HWND handleWindow) {
    clearMessageText();
    LONG l_GWL_STYLE = GetWindowLongPtr(handleWindow, GWL_STYLE);
    appendMessageText(tr("Style: %1").arg(QString::number(l_GWL_STYLE, 2)));
    int index = 1;

    if (l_GWL_STYLE & WS_POPUP) {
        // 此样式不能与 WS_CHILD样式一 起使用。
        appendMessageText(tr("%2 0x%1L WS_POPUP 窗口是弹出窗口。").arg(QString::number(WS_POPUP, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_CHILD) {
        // 具有此样式的窗口不能有菜单栏。 此样式不能与 WS_POPUP样式一 起使用。
        appendMessageText(tr("%2 0x%1L WS_CHILD 窗口是子窗口。").arg(QString::number(WS_CHILD, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_CAPTION) {
        appendMessageText(tr("%2 0x%1L WS_CAPTION 窗口具有标题栏 (包括 WS_BORDER 样式) 。").arg(QString::number(WS_CAPTION, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_CLIPCHILDREN) {
        appendMessageText(tr("%2 0x%1L WS_CLIPCHILDREN 在父窗口中绘制时，排除子窗口占用的区域。 创建父窗口时会使用此样式。").arg(QString::number(WS_CLIPCHILDREN, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_MAXIMIZEBOX) {
        appendMessageText(tr("%2 0x%1L WS_MAXIMIZEBOX 窗口具有最大化按钮。").arg(QString::number(WS_MAXIMIZEBOX, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_MINIMIZEBOX) {
        appendMessageText(tr("%2 0x%1L WS_MINIMIZEBOX 窗口具有最小化按钮。").arg(QString::number(WS_MINIMIZEBOX, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_SIZEBOX) {
        appendMessageText(tr("%2 0x%1L WS_SIZEBOX 窗口具有大小调整边框。 与 WS_THICKFRAME 样式相同。").arg(QString::number(WS_SIZEBOX, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_SYSMENU) {
        appendMessageText(tr("%2 0x%1L WS_SYSMENU 窗口的标题栏上有一个窗口菜单。 还必须指定 WS_CAPTION 样式。").arg(QString::number(WS_SYSMENU, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_BORDER) {
        appendMessageText(tr("%2 0x%1L WS_BORDER 窗口具有细线边框。").arg(QString::number(WS_BORDER, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_DLGFRAME) {
        appendMessageText(tr("%2 0x%1L WS_BORDER 窗口具有通常与对话框一起使用的样式边框。").arg(QString::number(WS_DLGFRAME, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_CLIPSIBLINGS) {
        // 也就是说，当特定子窗口收到 WM_PAINT 消息时， WS_CLIPSIBLINGS 样式会将所有其他重叠子窗口剪辑到要更新的子窗口区域。 如果未指定 WS_CLIPSIBLINGS 并且子窗口重叠，则当在子窗口的工作区内绘制时，可以在相邻子窗口的工作区内绘制。
        appendMessageText(tr("%2 0x%1L WS_CLIPSIBLINGS 将子窗口相对于彼此剪裁;").arg(QString::number(WS_CLIPSIBLINGS, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_STYLE & WS_VISIBLE) {
        // 可以使用 ShowWindow 或 SetWindowPos 函数打开和关闭此样式。
        appendMessageText(tr("%2 0x%1L WS_VISIBLE 窗口最初可见。").arg(QString::number(WS_VISIBLE, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }


    LONG l_GWL_EXSTYLE = GetWindowLongPtr(handleWindow, GWL_EXSTYLE);
    appendMessageText(tr("StyleEx: %1").arg(QString::number(l_GWL_EXSTYLE, 2)));
    index = 1;

    if (l_GWL_EXSTYLE & WS_EX_TOPMOST) {
        // 即使窗口已停用也是如此。 若要添加或删除此样式，请使用 SetWindowPos 函数。
        appendMessageText(tr("%2 0x%1L WS_EX_TOPMOST 该窗口应放置在所有非最顶层窗口上方，并且应保持其上方。").arg(QString::number(WS_EX_TOPMOST, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_TRANSPARENT) {
        // 也就是说透明度跟WS_EX_TRANSPARENT没关系，它所做的只是事件穿过窗口，操作后面的窗口（窗口像是透明的，不起作用）
        // 若要在不使用这些限制的情况下实现透明度，请使用 SetWindowRgn 函数。
        appendMessageText(tr("%2 0x%1L WS_EX_TRANSPARENT 在窗口下方（由同一个线程创建）的兄弟姐妹被绘制之前，不应该对窗口进行绘制。窗口显示为透明，因为底层兄弟窗口的位已经被绘制。").arg(QString::number(WS_EX_TRANSPARENT, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_COMPOSITED) {
        //  从下到上绘制顺序允许后代窗口具有半透明 (alpha) 和透明度 (颜色键) 效果，但前提是后代窗口还设置了WS_EX_TRANSPARENT位。 双缓冲允许不闪烁地绘制窗口及其后代。 如果窗口的 类样式 为 CS_OWNDC 或 CS_CLASSDC，则不能使用此样式。
        appendMessageText(tr("%2 0x%1L WS_EX_COMPOSITED 使用双缓冲按从下到上绘制顺序绘制窗口的所有后代。").arg(QString::number(WS_EX_COMPOSITED, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_WINDOWEDGE) {
        appendMessageText(tr("%2 0x%1L WS_EX_WINDOWEDGE 窗口具有带有凸起边缘的边框。").arg(QString::number(WS_EX_WINDOWEDGE, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_ACCEPTFILES) {
        appendMessageText(tr("%2 0x%1L WS_EX_ACCEPTFILES 窗口接受拖放文件。").arg(QString::number(WS_EX_ACCEPTFILES, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_LAYERED) {
        // https://learn.microsoft.com/zh-cn/windows/win32/winmsg/window-features
        appendMessageText(tr("%2 0x%1L WS_EX_LAYERED 窗口是分层窗口。").arg(QString::number(WS_EX_LAYERED, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_CLIENTEDGE) {
        appendMessageText(tr("%2 0x%1L WS_EX_CLIENTEDGE 窗口有一个边框，带有沉没边缘。").arg(QString::number(WS_EX_CLIENTEDGE, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_STATICEDGE) {
        appendMessageText(tr("%2 0x%1L WS_EX_STATICEDGE 该窗口具有一个三维边框样式，用于不接受用户输入的项目。").arg(QString::number(WS_EX_STATICEDGE, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_APPWINDOW) {
        appendMessageText(tr("%2 0x%1L WS_EX_APPWINDOW 当窗口可见时，将顶级窗口强制到任务栏上。").arg(QString::number(WS_EX_APPWINDOW, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GWL_EXSTYLE & WS_EX_TOOLWINDOW) {
        // 工具窗口具有短于普通标题栏的标题栏和使用较小的字体绘制的窗口标题。 工具窗口不会显示在任务栏中，也不会显示在用户按下 Alt+TAB 时出现的对话框中。 如果工具窗口有系统菜单，则其图标不会显示在标题栏上。 但是，可以通过右键单击或键入 Alt+SPACE 来显示系统菜单。
        appendMessageText(tr("%2 0x%1L WS_EX_TOOLWINDOW 该窗口旨在用作浮动工具栏。").arg(QString::number(WS_EX_TOOLWINDOW, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    LONG l_GCL_STYLE = GetClassLongPtr(handleWindow, GCL_STYLE);
    appendMessageText(tr("StyleClass: %1").arg(QString::number(l_GCL_STYLE, 2)));
    index = 1;

    if (l_GCL_STYLE & CS_VREDRAW) {
        appendMessageText(tr("%2 0x%1L CS_VREDRAW 如果移动或大小调整更改工作区的高度，则重新绘制整个窗口。").arg(QString::number(CS_VREDRAW, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GCL_STYLE & CS_HREDRAW) {
        appendMessageText(tr("%2 0x%1L CS_HREDRAW 如果移动或大小调整更改了工作区的宽度，则重新绘制整个窗口。").arg(QString::number(CS_HREDRAW, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GCL_STYLE & CS_DBLCLKS) {
        appendMessageText(tr("%2 0x%1L CS_DBLCLKS 当用户双击鼠标位于属于该类的窗口中时，将双击消息发送到窗口过程。").arg(QString::number(CS_DBLCLKS, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GCL_STYLE & CS_NOCLOSE) {
        appendMessageText(tr("%2 0x%1L CS_NOCLOSE 如果移动或大小调整更改了工作区的宽度，则重新绘制整个窗口。").arg(QString::number(CS_NOCLOSE, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GCL_STYLE & CS_PARENTDC) {
        //  具有 CS_PARENTDC 样式位的窗口从系统的设备上下文缓存接收常规设备上下文。 它不会为子级提供父级的设备上下文或设备上下文设置。 指定 CS_PARENTDC 可增强应用程序的性能。
        appendMessageText(tr("%2 0x%1L CS_PARENTDC 将子窗口的剪裁矩形设置为父窗口的剪裁矩形，以便子窗口可以绘制父窗口。可增强应用程序的性能。").arg(QString::number(CS_PARENTDC, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }

    if (l_GCL_STYLE & CS_GLOBALCLASS) {
        //  有关详细信息，请参阅 “关于窗口类”的“应用程序全局类”部分。
        appendMessageText(tr("%2 0x%1L CS_GLOBALCLASS 指示窗口类是应用程序全局类。").arg(QString::number(CS_GLOBALCLASS, 16), 8, QLatin1Char('0')).arg(QString::number(index++), 2, QLatin1Char('0')));
    }
}

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

    case qEventMessageKeyBoardProc:
    {
        char *p = event->data;

        HWND   *hwnd =  (HWND *)(p += sizeof(int));
        UINT   *msg =  (UINT *)(p += sizeof(HWND));
        WPARAM *wParam =  (WPARAM *)(p += sizeof(UINT));
        LPARAM *lParam =  (LPARAM *)(p += sizeof(WPARAM));


        QString keyBoardType;

        switch (*msg) {
        case WM_KEYDOWN:
            keyBoardType = "按键按下";
            break;

        case WM_KEYUP:
            keyBoardType = "按键释放";
            break;

        case WM_SYSKEYDOWN:
            keyBoardType = "ALT-按键按下";
            break;

        case WM_SYSKEYUP:
            keyBoardType = "ALT-按键释放";
            break;


        default:
            keyBoardType = "未知" + QString::number(*msg, 16);
            break;
        }


        QString key;

        switch (*wParam) {
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
        case 0x4C:
        case 0x4D:
        case 0x4E:
        case 0x4F:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5A:

            key =   QString(*wParam);
            break;


        case  0x60:
        case  0x61:
        case  0x62:
        case  0x63:
        case  0x64:
        case  0x65:
        case  0x66:
        case  0x67:
        case  0x68:
        case  0x69:

            key =   tr("小键盘%1").arg(*wParam - 0x5F);
            break;

        case  0x70:
        case  0x71:
        case  0x72:
        case  0x73:
        case  0x74:
        case  0x75:
        case  0x76:
        case  0x77:
        case  0x78:
        case  0x79:
        case  0x7A:
        case  0x7B:
        case  0x7C:
        case  0x7D:
        case  0x7E:
        case  0x7F:
        case  0x80:
        case  0x81:
        case  0x82:
        case  0x83:
        case  0x84:
        case  0x85:
        case  0x86:
        case  0x87:
            key =   tr("F%1").arg(*wParam - 0x6F);
            break;


        case VK_ESCAPE:
            key = "ESC";
            break;

        case VK_RETURN:
            key = "Enter";
            break;

        case VK_SPACE:
            key = "空格";
            break;

        case VK_DELETE:
            key = "DEL";
            break;

        case VK_LEFT:
            key = "向左";
            break;

        case VK_UP:
            key = "向上";
            break;

        case VK_RIGHT:
            key = "向右";
            break;

        case VK_DOWN:
            key = "向下";
            break;

        case VK_SHIFT:
            key = "SHIFT";
            break;

        case VK_LSHIFT:
            key = "左SHIFT";
            break;

        case VK_RSHIFT:
            key = "右SHIFT";
            break;

        case VK_CONTROL:
            key = "Ctrl";
            break;

        case VK_LCONTROL:
            key = "左Ctrl";
            break;

        case VK_RCONTROL:
            key = "右Ctrl";
            break;


        case VK_MENU:
            key = "Alt";
            break;

        case VK_LMENU:
            key = "左Alt";
            break;

        case VK_RMENU:
            key = "右Alt";
            break;

        case VK_LWIN:
            key = "左Win";
            break;

        case VK_RWIN:
            key = "右Win";
            break;

        default:
            key = "未知" + QString::number(*wParam, 16);
            break;
        }

        // 同时按下 用GetKeyState(VK_CONTROL)判断
        WORD repeatCount = LOWORD(*lParam);
        WORD keyFlags = HIWORD(*lParam);

        //        WORD scanCode = LOBYTE(keyFlags);
        //        BOOL repeatFlag = (keyFlags & KF_REPEAT) == KF_REPEAT;
        //        BOOL upFlag = (keyFlags & KF_UP) == KF_UP;

        QString text = tr("[消息-键盘] %1 虚拟键码：%2 hwnd：%3 重复计数：%4")
                       .arg(keyBoardType)
                       .arg(         key)
                       .arg(     QString::number(UINT(*hwnd), 16))
                       .arg( repeatCount);
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
    QString text = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    text += msg;
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void Itemview10ProcessStatus::clearMessageText()
{
    edtMessage->clear();
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
    case qEventKeyBoardProc:
    case qEventMessageMouseProc:
    case qEventMessageKeyBoardProc:
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

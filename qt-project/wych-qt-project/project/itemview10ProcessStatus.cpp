

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
#include <events/eventstatusgrid.h>


Itemview10ProcessStatus::Itemview10ProcessStatus(QWidget *parent)
    : QWidget{parent}
{
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


    btnMessageHookStart = new QPushButton("钩子", this);


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


    // 这行必须放下面
}

void Itemview10ProcessStatus::initConnect()
{
    // 事件 - 刷新窗口
    connect(btnRefreshWindow, &QPushButton::clicked, [this]() {
        updateWindowInfo(windowInfo.HandleWindow);
    });
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

void Itemview10ProcessStatus::appendConsole(const QString& msg)
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

    case qEventStatusGridScript:
    {
        EventStatusGrid *event = dynamic_cast<EventStatusGrid *>(e);
        updateScriptStatus(event->item, event->gridType);
        e->accept();
        break;
    }

    case qEventCountGridScript:
    {
        EventStatusGrid *event = dynamic_cast<EventStatusGrid *>(e);
        updateScriptCount(event->item);
        e->accept();
        break;
    }

    default:
        break;
    }
}

// void Itemview10ProcessStatus::mouseMoveEvent(QMouseEvent *event) {
//    int mouseX = event->x();
//    int mouseY = event->y();

//    qDebug() << mouseX << ":" << mouseY;
// }


#include <QTextCodec>
#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>
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
#include "utils.h"
#include "winapi.h"
#include "itemview10Script.h"
#include "components/pixmapwidget.h"


Itemview10Script::Itemview10Script(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnect();
}

Itemview10Script::~Itemview10Script()
{}

void Itemview10Script::initUI()
{
    auto layout = new QHBoxLayout(this);

    // 第一层
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);

    // 第二层
    auto leftQWidgetGroupBox1 = new QGroupBox("状态", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    leftQWidgetGroupBox1->setFixedHeight(140);
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


    auto leftQWidgetGroupBox2 = new QGroupBox("截图", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout2 = new QGridLayout(leftQWidgetGroupBox2);

    auto scrollArea = new QScrollArea(this);
    leftQWidgetGroup1Layout2->addWidget(scrollArea);
    pixmapWidget = new PixmapWidget(scrollArea);
    scrollArea->setWidget(pixmapWidget);

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
    auto rightQWidget = new QWidget(this);
    auto rightQWidgetLayout = new QVBoxLayout(rightQWidget);


    // 第二层(右边列)
    auto rightQWidgetGroupBox1 = new QGroupBox("数据目录表明细", rightQWidget);
    auto rightQWidgetGroupBox1Layout = new QVBoxLayout(rightQWidgetGroupBox1);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    // 第三层(右边列)
    auto tabTabWidget = new QTabWidget(rightQWidgetGroupBox1);
    auto tab2 = new QWidget(tabTabWidget);
    auto importTabTabWidgetLayout = new QVBoxLayout(tab2);
    auto tab3 = new QWidget(tabTabWidget);
    auto scriptLayout = new QVBoxLayout(tab3);
    tabTabWidget->addTab(tab2, tr("窗口"));
    tabTabWidget->addTab(tab3, tr("脚本"));
    rightQWidgetGroupBox1Layout->addWidget(tabTabWidget);


    // 第四层(窗口)

    auto importTabTabWidgetGroupBox3 = new QGroupBox("窗口状态", tab2);
    importTabTabWidgetLayout->addWidget(importTabTabWidgetGroupBox3);
    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto window1Layout = new QVBoxLayout();
    auto window1Layouta = new QHBoxLayout();
    auto window1Layoutb = new QHBoxLayout();
    auto window1Layoutc = new QHBoxLayout();
    auto window1Layoutd = new QHBoxLayout();
    auto window1Layoute = new QHBoxLayout();
    window1Layout->setAlignment(Qt::AlignLeft);
    window1Layouta->setAlignment(Qt::AlignLeft);
    window1Layoutb->setAlignment(Qt::AlignLeft);
    window1Layoutc->setAlignment(Qt::AlignLeft);
    window1Layoutd->setAlignment(Qt::AlignLeft);
    window1Layoute->setAlignment(Qt::AlignLeft);
    importTabTabWidgetGroupBox3->setLayout(window1Layout);
    window1Layout->addLayout(window1Layouta);
    window1Layout->addLayout(window1Layoutb);
    window1Layout->addLayout(window1Layoutc);
    window1Layout->addLayout(window1Layoutd);
    window1Layout->addLayout(window1Layoute);


    //    importTabTabWidgetGroupBox3->setFixedHeight(60);
    //    importTabTabWidgetGroupBoxLayout3->addWidget(btnImportAdd);


    auto importTabTabWidgetGroupBox = new QGroupBox("窗口明细", tab2);
    importTabTabWidgetLayout->addWidget(importTabTabWidgetGroupBox);
    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto importTabTabWidgetGroupBoxLayout = new QGridLayout(importTabTabWidgetGroupBox);


    WindowsTableView = new QTreeView(this);
    WindowsGridModel = new QStandardItemModel(this);
    WindowsGridModel->setHorizontalHeaderLabels({  "句柄",  "编号", "标题", "类", "窗口大小" });

    WindowsTableView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //    WindowsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    WindowsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    WindowsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    WindowsTableView->setModel(WindowsGridModel);
    importTabTabWidgetGroupBoxLayout->addWidget(WindowsTableView);


    // 第四层(脚本)
    auto relocationTabTabWidgetGroupBox = new QGroupBox("操作", tab3);
    scriptLayout->addWidget(relocationTabTabWidgetGroupBox);
    scriptLayout->setAlignment(Qt::AlignTop);
    relocationTabTabWidgetGroupBox->setFixedHeight(100);

    auto layout_r_1_m = new QVBoxLayout();
    auto rightQWidgetGroup1aLayout = new QHBoxLayout();
    auto rightQWidgetGroup1bLayout = new QHBoxLayout();
    auto rightQWidgetGroup1cLayout = new QHBoxLayout();
    rightQWidgetGroup1aLayout->setAlignment(Qt::AlignLeft);
    rightQWidgetGroup1bLayout->setAlignment(Qt::AlignLeft);
    rightQWidgetGroup1cLayout->setAlignment(Qt::AlignLeft);
    relocationTabTabWidgetGroupBox->setLayout(layout_r_1_m);
    layout_r_1_m->addLayout(rightQWidgetGroup1aLayout);
    layout_r_1_m->addLayout(rightQWidgetGroup1bLayout);
    layout_r_1_m->addLayout(rightQWidgetGroup1cLayout);


    auto script2tGroupBox = new QGroupBox("脚本列表", tab3);
    scriptLayout->addWidget(script2tGroupBox);
    scriptLayout->setAlignment(Qt::AlignTop);
    auto script2Layout = new QGridLayout(script2tGroupBox);


    scriptTableView = new QTreeView(this);
    scriptGridModel = new QStandardItemModel();
    scriptGridModel->setHorizontalHeaderLabels({  "编号",  "状态", "类型", "信息",  "名称", "颜色" });

    scriptTableView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    scriptTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scriptTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    scriptTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    scriptTableView->setModel(scriptGridModel);

    script2Layout->addWidget(scriptTableView);


    auto script3GroupBox = new QGroupBox("脚本明细", tab3);
    scriptLayout->addWidget(script3GroupBox);
    auto script3Layout = new QVBoxLayout(script3GroupBox);

    auto script3Layouta = new QHBoxLayout();
    auto script3Layoutb = new QHBoxLayout();
    auto script3Layoutc = new QHBoxLayout();
    auto script3Layoutd = new QHBoxLayout();
    auto script3Layoute = new QHBoxLayout();
    script3Layouta->setAlignment(Qt::AlignLeft);
    script3Layoutb->setAlignment(Qt::AlignLeft);
    script3Layoutc->setAlignment(Qt::AlignLeft);
    script3Layoutd->setAlignment(Qt::AlignLeft);
    script3Layoute->setAlignment(Qt::AlignLeft);
    script3Layout->addLayout(script3Layouta);
    script3Layout->addLayout(script3Layoutb);
    script3Layout->addLayout(script3Layoutc);
    script3Layout->addLayout(script3Layoutd);
    script3Layout->addLayout(script3Layoute);


    // 控件

    tb_export_rva = new QLineEdit("");
    tb_import_rva = new QLineEdit("");
    tb_resource_rva = new QLineEdit("");
    tb_base_relocation_rva = new QLineEdit("");
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
    tb_process_name->setFixedWidth(240);
    tb_process_handle = new QLineEdit("");
    tb_window_name = new QLineEdit("");
    tb_window_name->setFixedWidth(150);
    tb_window_handle = new QLineEdit("");
    tb_window_handle->setFixedWidth(100);
    tb_mouse_position = new QLineEdit("");
    tb_mouse_position->setFixedWidth(150);
    tb_mouse_rgb = new QLineEdit("");
    tb_mouse_rgb->setFixedWidth(240);
    tb_mouse_ratio = new QLineEdit("");
    tb_mouse_ratio->setFixedWidth(150);
    tb_mouse_ratio_position = new QLineEdit("");
    tb_mouse_ratio_position->setFixedWidth(150);


    ckConsoleEnable = new QCheckBox("启动控制台");
    ckConsoleEnable->setChecked(true);
    btnConsoleClear = new QPushButton("清空控制台");
    btnWindowPrint = new QPushButton("截图");
    btnWindowPrint->setFixedWidth(60);
    rb_printWindow = new QRadioButton("窗口截图");
    rb_printClient = new QRadioButton("内容截图");
    btnScriptAdd = new QPushButton("新增脚本");
    btnScriptSave = new QPushButton("保存脚本");
    btnRefreshWindow = new QPushButton("刷新");
    btnRefreshWindow->setFixedWidth(60);
    rb_printClient->setChecked(true);

    bg_printConfig = new QButtonGroup(this);
    bg_printConfig->addButton(rb_printWindow, 0);
    bg_printConfig->addButton(rb_printClient, 1);


    rb_printWin32 = new QRadioButton("Win截图");
    rb_printWin32->setChecked(true);

    ckScriptDetailSleep = new QCheckBox("执行后延迟");

    rb_printAdb = new QRadioButton("Android截图");
    bg_printType = new QButtonGroup(this);
    bg_printType->addButton(rb_printWin32, 0);
    bg_printType->addButton(rb_printAdb,   1);


    btnScriptDetailSave = new QPushButton("脚本明细保存");

    rb_scriptTypeCondition = new QRadioButton("条件判断");
    rb_scriptTypeDeal = new QRadioButton("操作");
    rb_scriptTypeConditionAndDeal = new QRadioButton("条件判断+操作");
    bg_scriptType = new QButtonGroup(this);
    bg_scriptType->addButton(rb_scriptTypeCondition,        0);
    bg_scriptType->addButton(rb_scriptTypeDeal,             1);
    bg_scriptType->addButton(rb_scriptTypeConditionAndDeal, 2);

    rb_scriptConditionTypePixel = new QRadioButton("像素点检测");
    rb_scriptConditionTypeEdgeDetect = new QRadioButton("图片边缘检测");
    rb_scriptConditionTypeOCR = new QRadioButton("文字OCR检测");
    bg_scriptConditionType = new QButtonGroup(this);
    bg_scriptConditionType->addButton(rb_scriptConditionTypePixel,      0);
    bg_scriptConditionType->addButton(rb_scriptConditionTypeEdgeDetect, 1);
    bg_scriptConditionType->addButton(rb_scriptConditionTypeOCR,        2);


    action_addScript = new QAction(tr("新增脚本"), this);
    action_removeScript = new QAction(tr("删除脚本"), this);
    menu_rightClick = new QMenu(this);
    menu_rightClick->addAction(action_addScript);
    menu_rightClick->addAction(action_removeScript);


    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    auto lb_e_magic = new QLabel("e_magic:");
    auto lb_e_lfanew = new QLabel("e_lfanew:");
    lb_e_magic->setPalette(pe);
    lb_e_lfanew->setPalette(pe);


    leftQWidgetGroup1aLayout->addWidget(new QLabel("进程名称:"), 0, Qt::AlignLeft);
    leftQWidgetGroup1aLayout->addWidget(tb_process_name,     0, Qt::AlignLeft);


    leftQWidgetGroup1bLayout->addWidget(new QLabel("鼠标坐标:", this), 0, Qt::AlignLeft);
    leftQWidgetGroup1bLayout->addWidget(tb_mouse_position,         0, Qt::AlignLeft);
    leftQWidgetGroup1bLayout->addWidget(new QLabel("颜色:", this),   0, Qt::AlignLeft);
    leftQWidgetGroup1bLayout->addWidget(tb_mouse_rgb,              0, Qt::AlignLeft);
    leftQWidgetGroup1cLayout->addWidget(new QLabel("鼠标占比:", this), 0, Qt::AlignLeft);
    leftQWidgetGroup1cLayout->addWidget(tb_mouse_ratio,            0, Qt::AlignLeft);
    leftQWidgetGroup1cLayout->addWidget(new QLabel("->坐标:", this), 0, Qt::AlignLeft);
    leftQWidgetGroup1cLayout->addWidget(tb_mouse_ratio_position,   0, Qt::AlignLeft);


    window1Layouta->addWidget(new QLabel("窗口名称:"), 0, Qt::AlignLeft);
    window1Layouta->addWidget(tb_window_name,      0, Qt::AlignLeft);
    window1Layouta->addWidget(new QLabel("句柄:"),   0, Qt::AlignLeft);
    window1Layouta->addWidget(tb_window_handle,    0, Qt::AlignLeft);
    window1Layouta->addWidget(btnRefreshWindow,    0, Qt::AlignLeft);

    window1Layoutb->addWidget(new QLabel("窗口大小:", this), 0, Qt::AlignLeft);
    window1Layoutb->addWidget(tb_window_size,            0, Qt::AlignLeft);
    window1Layoutb->addWidget(new QLabel("坐标:", this),   0, Qt::AlignLeft);
    window1Layoutb->addWidget(tb_window_position,        0, Qt::AlignLeft);

    window1Layoutc->addWidget(new QLabel("内部大小:", this), 0, Qt::AlignLeft);
    window1Layoutc->addWidget(tb_window2_size,           0, Qt::AlignLeft);
    window1Layoutc->addWidget(new QLabel("坐标:", this),   0, Qt::AlignLeft);
    window1Layoutc->addWidget(tb_window2_position,       0, Qt::AlignLeft);

    window1Layoutd->addWidget(new QLabel("截图区域:", this));
    window1Layoutd->addWidget(rb_printWindow);
    window1Layoutd->addWidget(rb_printClient);

    window1Layoute->addWidget(new QLabel("截图方式:", this));
    window1Layoute->addWidget(rb_printWin32);
    window1Layoute->addWidget(rb_printAdb);
    window1Layoute->addWidget(btnWindowPrint);


    leftQWidgetGroup1Layout3->addWidget(ckConsoleEnable);
    leftQWidgetGroup1Layout3->addWidget(btnConsoleClear);


    rightQWidgetGroup1aLayout->addWidget(btnScriptAdd);
    rightQWidgetGroup1aLayout->addWidget(btnScriptSave);

    script3Layouta->addWidget(new QLabel("脚本类型:", this),     0, Qt::AlignLeft);
    script3Layouta->addWidget(rb_scriptTypeCondition,        0, Qt::AlignLeft);
    script3Layouta->addWidget(rb_scriptTypeDeal,             0, Qt::AlignLeft);
    script3Layouta->addWidget(rb_scriptTypeConditionAndDeal, 0, Qt::AlignLeft);


    script3Layoutb->addWidget(new QLabel("条件类型:", this),        0, Qt::AlignLeft);
    script3Layoutb->addWidget(rb_scriptConditionTypePixel,      0, Qt::AlignLeft);
    script3Layoutb->addWidget(rb_scriptConditionTypeEdgeDetect, 0, Qt::AlignLeft);
    script3Layoutb->addWidget(rb_scriptConditionTypeOCR,        0, Qt::AlignLeft);


    script3Layoutd->addWidget(ckScriptDetailSleep, 0, Qt::AlignLeft);

    script3Layoute->addWidget(btnScriptDetailSave, 0, Qt::AlignLeft);

    layout->addWidget(leftQWidget);

    layout->addWidget(rightQWidget);
}

void Itemview10Script::initConnect()
{
    connect(scriptGridModel, &QStandardItemModel::itemChanged, [this](QStandardItem *item) {
        qDebug() << item->data();

        if (item == nullptr) return;

        if (item->isCheckable())

        {
            // 如果条目是存在复选框的，那么就进行下面的操作
            Qt::CheckState state = item->checkState(); // 获取当前的选择状态

            if (item->isTristate())
            {
                // 如果条目是三态的，说明可以对子目录进行全选和全不选的设置
                if (state != Qt::PartiallyChecked)
                {
// 当前是选中状态，需要对其子项目进行全选
// treeItem_checkAllChild(item, state == Qt::Checked ? true : false);
                }
            }
            else
            {
// 说明是两态的，两态会对父级的三态有影响
// 判断兄弟节点的情况
// treeItem_CheckChildChanged(item);
            }
        }
    });


    connect(action_addScript, &QAction::triggered, [this]() {
        QModelIndex index = scriptTableView->currentIndex();

        if (index.isValid()) {
            QStandardItem *parentItem = scriptGridModel->itemFromIndex(index);

            if (index.column() != 0) {
                index = index.siblingAtColumn(0);
                parentItem = scriptGridModel->itemFromIndex(index);
            } else {
                parentItem = scriptGridModel->itemFromIndex(index);
            }

            //            QModelIndex parentIndex = index.parent();
            //            QStandardItem *parentItem = scriptGridModel->itemFromIndex(parentIndex);

            int rowCount = parentItem->rowCount();

            int j = 1;

            for (int i = 0; i < rowCount; i++) {
                QModelIndex childIndex2 = index.child(i, 2);

                if (childIndex2.data().toString() == tr("判断")) {
                    j++;
                }
            }

            QStandardItem *newItem = new QStandardItem(tr("%1-A%2").arg(index.data().toString()).arg(j));
            newItem->setCheckable(true);
            newItem->setCheckState(Qt::PartiallyChecked);

            // newItem->setData();

            parentItem->setChild(rowCount, 0, newItem);
            parentItem->setChild(rowCount, 2, new QStandardItem("判断"));


            //           scriptGridModel->index()
        }
    });


    // 事件 - 表格右键
    connect(scriptTableView, &QTreeView::customContextMenuRequested, [this](const QPoint& pos) {
        QModelIndex index = scriptTableView->indexAt(pos);

        //        QModelIndex index2 = scriptTableView->mapToGlobal(pos);
        if (index.isValid()) // 如果行数有效，则显示菜单
        {
            menu_rightClick->exec(QCursor::pos());
        }
    });

    // 事件 - 刷新
    connect(btnScriptAdd, &QPushButton::clicked, [this]() {
        //        scriptGridModel->clear();
        {
            scriptGridModel->setItem(0, 0, new QStandardItem("A01"));
            scriptGridModel->setItem(0, 2, new QStandardItem("判断"));


            QList<QStandardItem *>parents =  scriptGridModel->findItems("A01", Qt::MatchExactly  | Qt::MatchRecursive, 0);

            QStandardItem *rowItem = parents.first();
            int rowCount = rowItem->rowCount();

            // rowItem.ch

            rowItem->setCheckable(true);
            rowItem->setCheckState(Qt::PartiallyChecked);
            rowItem->setChild(rowCount, 0, new QStandardItem("A01-A01"));
            rowItem->setChild(rowCount, 2, new QStandardItem("判断"));

            rowCount = rowItem->rowCount();


            rowItem->setChild(rowCount, 0, new QStandardItem("A01-A02"));
            rowItem->setChild(rowCount, 2, new QStandardItem("操作"));
        }

        {
            scriptGridModel->setItem(1, 0, new QStandardItem("A02"));
            scriptGridModel->setItem(1, 2, new QStandardItem("判断+操作"));

            QList<QStandardItem *>parents =  scriptGridModel->findItems("A02", Qt::MatchExactly  | Qt::MatchRecursive, 0);

            QStandardItem *rowItem = parents.first();
            int rowCount = rowItem->rowCount();

            rowItem->setChild(rowCount, 0, new QStandardItem("A02-A01"));
            rowItem->setChild(rowCount, 2, new QStandardItem("判断"));

            rowCount = rowItem->rowCount();

            rowItem->setChild(rowCount, 0, new QStandardItem("A02-A02"));
            rowItem->setChild(rowCount, 2, new QStandardItem("操作"));
        }

        scriptTableView->expandAll();
    });


    // 事件 - 保存
    connect(btnScriptSave, &QPushButton::clicked, [this]() {
        scriptGridModel;
    });

    // 事件 - 刷新
    connect(btnRefreshWindow, &QPushButton::clicked, [this]() {
        updateWindowInfo(windowInfo.HandleWindow);
    });

    // 事件 - 双击
    connect(WindowsTableView, &QTreeView::doubleClicked, [this](const QModelIndex& current) {
        // auto rowIndex = WindowsGridModel->itemFromIndex(current)->row();


        bool ok;
        auto currentHandleV = current.sibling(current.row(), 0).data().toString();
        auto currentHandle = currentHandleV.toInt(&ok, 16);

        if (ok) {
            updateWindowInfo((HWND)currentHandle);
            print();
        }
    });


    // 事件 - 图片框的鼠标事件
    connect(pixmapWidget, &PixmapWidget::mousePositionEvent, [this](int mouseX, int mouseY, QColor& color, float   xr, float   yr) {
        tb_mouse_position->setText(tr("[x:%1, y:%2]").arg(mouseX).arg(mouseY));
        tb_mouse_rgb->setText(tr("[r:%1, g:%2, b:%3, a:%4]").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
        tb_mouse_rgb->setStyleSheet(tr("background-color:rgba(%1,%2,%3,%4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
        tb_mouse_ratio->setText(tr("[x:%1%, y:%2%]").arg(QString::number(xr, 'f', 2)).arg(QString::number(yr, 'f', 2)));
    });


    // 截图
    connect(btnWindowPrint, &QPushButton::clicked, [this]() {
        print();
    });


    connect(btnConsoleClear, &QPushButton::clicked, [this]() {
        clearMessage();
    });
}

bool Itemview10Script::updateWindowInfo(HWND handleWindow) {
    BOOL success = WinAPI::get_window_info(handleWindow, &windowInfo);

    tb_window_name->setText(QString::fromWCharArray(windowInfo.TitleName));
    tb_window_handle->setText(QString::number((intptr_t)(windowInfo.HandleWindow), 16).toUpper());
    tb_window_size->setText(tr("[ %1 x %2 ]").arg(windowInfo.WindowRect.right - windowInfo.WindowRect.left).arg(windowInfo.WindowRect.bottom - windowInfo.WindowRect.top));
    tb_window_position->setText(tr("(%1,%2),(%3,%4)").arg(windowInfo.WindowRect.left).arg(windowInfo.WindowRect.top).arg(windowInfo.WindowRect.right).arg(windowInfo.WindowRect.bottom));
    tb_window2_size->setText(tr("[ %1 x %2 ]").arg(windowInfo.ClientRect.right - windowInfo.ClientRect.left).arg(windowInfo.ClientRect.bottom - windowInfo.ClientRect.top));
    tb_window2_position->setText(tr("(%1,%2),(%3,%4)").arg(windowInfo.ClientToScreen.x).arg(windowInfo.ClientToScreen.y).arg(windowInfo.ClientToScreen.x + windowInfo.ClientRect.right).arg(windowInfo.ClientToScreen.y + windowInfo.ClientRect.bottom));


    return TRUE;
}

bool Itemview10Script::buildProcess(DWORD pid) {
    BOOL success = WinAPI::get_process_info(pid, &processInfo);

    if (success) {
        success = WinAPI::get_window_main(pid, &windowInfo);

        if (success) {
            tb_process_name->setText(tr("[%1] %2 ").arg(pid).arg(QString::fromWCharArray(processInfo.PName)));
            tb_window_name->setText(QString::fromWCharArray(windowInfo.TitleName));
            tb_window_handle->setText(QString::number((intptr_t)(windowInfo.HandleWindow), 16).toUpper());
            tb_window_size->setText(tr("[ %1 x %2 ]").arg(windowInfo.WindowRect.right - windowInfo.WindowRect.left).arg(windowInfo.WindowRect.bottom - windowInfo.WindowRect.top));
            tb_window_position->setText(tr("(%1,%2),(%3,%4)").arg(windowInfo.WindowRect.left).arg(windowInfo.WindowRect.top).arg(windowInfo.WindowRect.right).arg(windowInfo.WindowRect.bottom));
            tb_window2_size->setText(tr("[ %1 x %2 ]").arg(windowInfo.ClientRect.right - windowInfo.ClientRect.left).arg(windowInfo.ClientRect.bottom - windowInfo.ClientRect.top));
            tb_window2_position->setText(tr("(%1,%2),(%3,%4)").arg(windowInfo.ClientToScreen.x).arg(windowInfo.ClientToScreen.y).arg(windowInfo.ClientToScreen.x + windowInfo.ClientRect.right).arg(windowInfo.ClientToScreen.y + windowInfo.ClientRect.bottom));


            WindowsGridModel->setItem(0, 0, new QStandardItem(QString::number((intptr_t)(windowInfo.HandleWindow), 16).toUpper()));
            WindowsGridModel->setItem(0, 1, new QStandardItem(QString::number(0)));
            WindowsGridModel->setItem(0, 2, new QStandardItem(QString::fromWCharArray(windowInfo.TitleName)));
            WindowsGridModel->setItem(0, 3, new QStandardItem(QString::fromWCharArray(windowInfo.ClassName)));

            // 子窗口
            std::vector<WIN32_WINDOW_INFO> children;
            success = WinAPI::get_window_child(windowInfo.HandleWindow, children);

            for (std::vector<WIN32_WINDOW_INFO>::iterator iter = children.begin(); iter != children.end(); ++iter) {
                int index = std::distance(children.begin(), iter) + 1;

                QString itemHandle = QString::number((intptr_t)(iter->HandleParentWindow), 16).toUpper();

                QList<QStandardItem *> parents =  WindowsGridModel->findItems(itemHandle, Qt::MatchExactly  | Qt::MatchRecursive, 0);

                if (parents.count() == 1) {
                    QStandardItem *rowItem = parents.first();
                    int rowCount = rowItem->rowCount();
                    rowItem->setChild(rowCount, 0, new QStandardItem(QString::number((intptr_t)(iter->HandleWindow), 16).toUpper()));

                    //                    rowItem->setChild(0, 1, new QStandardItem(itemHandle));
                    rowItem->setChild(rowCount, 2, new QStandardItem(QString::fromWCharArray(iter->TitleName)));
                    rowItem->setChild(rowCount, 3, new QStandardItem(QString::fromWCharArray(iter->ClassName)));
                    rowItem->setChild(rowCount, 1, new QStandardItem(QString::number(index)));
                }
            }


            WindowsTableView->expandAll();

            print();
            return true;
        }
    }

    return false;
}

bool Itemview10Script::print() {
    //        QPixmap  m_pixmap = QPixmap("C:\\Users\\汪意超\\Pictures\\Saved Pictures\\001OdAkagy1gyybblipk3j60h00fvajh02.jpg");

    //        pixmapWidget->setPixmap(m_pixmap);

    //        return;


    HWND hwnd_ = windowInfo.HandleWindow;

    if (!hwnd_) {
        return FALSE;
    }
    HDC displayDC;
    HDC bitmapDC;

    if (rb_printWindow->isChecked()) {}

    displayDC = ::GetWindowDC(hwnd_);           // 获取窗口DC
    bitmapDC = ::CreateCompatibleDC(displayDC); // 缓冲内存DC

    HBITMAP bitmap = nullptr;
    int     w = -1, h = -1, x = 0, y = 0;
    RECT    r = { 0, 0, 0, 0 };
    GetWindowRect(hwnd_, &r);

    if (w < 0) w = r.right - r.left;

    if (h < 0) h = r.bottom - r.top;


    if (rb_printClient->isChecked()) {
        RECT cr = { 0, 0, 0, 0 };

        // 算内部坐标
        GetClientRect(hwnd_, &cr);


        POINT point = { 0, 0 };
        ClientToScreen(hwnd_, &point);


        w = cr.right;
        h = cr.bottom;

        x = point.x - r.left;
        y = point.y - r.top;


        //            x = 20;
        //            y = 20;
    }


    bitmap = CreateCompatibleBitmap(displayDC, w, h);

    HGDIOBJ null_bitmap = SelectObject(bitmapDC, bitmap);

    BitBlt(bitmapDC, 0, 0, w, h, displayDC, x, y, SRCCOPY | CAPTUREBLT);


    // clean up all but bitmap
    ReleaseDC(hwnd_, displayDC);
    SelectObject(bitmapDC, null_bitmap);
    DeleteDC(bitmapDC);

    QPixmap map = QtWin::fromHBITMAP(bitmap);
    pixmapWidget->setPixmap(map);

    //        QImage image = QImage::fromHBITMAP(bitmap);
    //        image.reinterpretAsFormat(QImage::QImage::Format_ARGB32_Premultiplied);
    //        QPixmap pixmap = QPixmap::fromImage(image);

    DeleteObject(bitmap);


    // this->update();

    //        auto rowIndex = processTableView->currentIndex().row();

    //        if (rowIndex < 0) {
    //            return;
    //        }

    //        auto isWindow =  processGridModel->item(rowIndex, 0)->text();

    //        if (isWindow.isEmpty()) {
    //            return;
    //        }

    //        auto pid =  processGridModel->item(rowIndex, 2)->text().toInt();


    //        const auto clientRectWidth = 200;
    //        const auto clientRectHeight = 200;

    //        // 位图信息
    //        BITMAPINFO bitmapInfo;
    //        bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo);
    //        bitmapInfo.bmiHeader.biWidth = clientRectWidth;
    //        bitmapInfo.bmiHeader.biHeight = clientRectHeight;
    //        bitmapInfo.bmiHeader.biPlanes = 1;
    //        bitmapInfo.bmiHeader.biBitCount = 32;
    //        bitmapInfo.bmiHeader.biSizeImage = clientRectWidth * clientRectHeight;
    //        bitmapInfo.bmiHeader.biCompression = BI_RGB;

    //        HWND hwnd_ = (HWND)0x2084A;
    //        HDC scrDc_;
    //        HDC memDc_;
    //        HBITMAP bitmap_;
    //        HBITMAP oldBitmap_;
    //        void *bitsPtr_;


    //        scrDc_ = ::GetWindowDC(hwnd_);         // 获取窗口DC
    //        memDc_ = ::CreateCompatibleDC(scrDc_); // 缓冲内存DC
    //        bitmap_ = ::CreateDIBSection(memDc_, &bitmapInfo, DIB_RGB_COLORS, &bitsPtr_, nullptr, 0);

    //        if (bitmap_ == nullptr)
    //        {
    //            ::DeleteDC(memDc_);
    //            ::ReleaseDC(hwnd_, scrDc_);
    //            return;
    //        }
    //        oldBitmap_ = static_cast<HBITMAP>(::SelectObject(memDc_, bitmap_));

    //        POINT bitbltStartPoint_ { 0, 0 };

    //        const auto ret = ::BitBlt(
    //            memDc_, 0, 0, clientRectWidth, clientRectHeight,
    //            scrDc_, bitbltStartPoint_.x, bitbltStartPoint_.y,
    //            SRCCOPY);

    //        if (ret == 0) {
    //            return;
    //        }

    //        //删除用过的对象
    //        ::SelectObject(memDc_, oldBitmap_);
    //        ::DeleteObject(bitmap_);
    //        ::DeleteDC(memDc_);
    //        ::ReleaseDC(hwnd_, scrDc_);
}

void Itemview10Script::appendMessage(const QString& msg)
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

void Itemview10Script::showMessage(const QString& msg)
{
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void Itemview10Script::clearMessage()
{
    edtMsg->clear();
}

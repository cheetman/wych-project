
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
#include <QIcon>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include "utils.h"
#include "winapi.h"
#include "itemview10Script.h"
#include "components/pixmapwidget.h"
#include "components/scripttypedialog.h"


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
    relocationTabTabWidgetGroupBox->setFixedHeight(60);

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
    scriptTableView->setTreePosition(2);
    scriptGridModel = new QStandardItemModel();
    scriptGridModel->setHorizontalHeaderLabels({ "状态",  "名称",  "编号",  "类型", "信息",  "备注", "颜色" });

    scriptTableView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    scriptTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scriptTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    scriptTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    scriptTableView->setModel(scriptGridModel);

    script2Layout->addWidget(scriptTableView);


    //    script3GroupBox = new QGroupBox("脚本明细", tab3);
    //    scriptLayout->addWidget(script3GroupBox);


    tabScriptWidget = new QTabWidget(tab3);
    tabScriptWidget->setFixedHeight(500);
    scriptLayout->addWidget(tabScriptWidget);
    auto scriptDetailWidget = new QWidget(tabScriptWidget);
    auto scriptEditWidget = new QWidget(tabScriptWidget);
    tabScriptWidget->addTab(scriptDetailWidget, "脚本明细列表");
    tabScriptWidget->addTab(scriptEditWidget,   "脚本明细编辑");

    auto script4Layout = new QVBoxLayout(scriptDetailWidget);

    scriptDetailTableView = new QTreeView(this);
    scriptDetailGridModel = new QStandardItemModel();
    scriptDetailTableView->setTreePosition(1);
    scriptDetailGridModel->setHorizontalHeaderLabels({  "状态",  "编号",  "类型", "信息",  "备注", "颜色" });

    scriptDetailTableView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    scriptDetailTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scriptDetailTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    scriptDetailTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    scriptDetailTableView->setModel(scriptDetailGridModel);

    script4Layout->addWidget(scriptDetailTableView);


    auto script5Layout = new QVBoxLayout();
    scriptEditWidget->setLayout(script5Layout);
    auto saScriptEdit = new QScrollArea(tab3);
    script5Layout->addWidget(saScriptEdit);
    auto saContentWidget = new QWidget();

    //    saContentWidget->setFixedHeight(500);
    auto script3Layout = new QVBoxLayout();

    saContentWidget->setLayout(script3Layout);

    auto script3Layouta = new QHBoxLayout();
    auto script3Layoutb = new QHBoxLayout();
    auto script3Layoutc = new QHBoxLayout();
    auto script3Layoutd = new QHBoxLayout();
    auto script3Layoute = new QHBoxLayout();
    auto script3Layoutf = new QHBoxLayout();
    auto script3Layoutg = new QHBoxLayout();
    auto script3Layouth = new QHBoxLayout();
    auto script3Layouti = new QHBoxLayout();
    auto script3Layoutj = new QHBoxLayout();
    auto script3Layoutk = new QHBoxLayout();
    auto script3Layoutl = new QHBoxLayout();
    script3Layouta->setAlignment(Qt::AlignLeft);
    script3Layoutb->setAlignment(Qt::AlignLeft);
    script3Layoutc->setAlignment(Qt::AlignLeft);
    script3Layoutd->setAlignment(Qt::AlignLeft);
    script3Layoute->setAlignment(Qt::AlignLeft);
    script3Layoutf->setAlignment(Qt::AlignLeft);
    script3Layoutg->setAlignment(Qt::AlignLeft);
    script3Layouth->setAlignment(Qt::AlignLeft);
    script3Layouti->setAlignment(Qt::AlignLeft);
    script3Layoutj->setAlignment(Qt::AlignLeft);
    script3Layoutk->setAlignment(Qt::AlignLeft);
    script3Layoutl->setAlignment(Qt::AlignLeft);
    script3Layout->addLayout(script3Layouta);
    script3Layout->addLayout(script3Layoutb);
    script3Layout->addLayout(script3Layoutc);
    script3Layout->addLayout(script3Layoutd);
    script3Layout->addLayout(script3Layoute);
    script3Layout->addLayout(script3Layoutf);
    script3Layout->addLayout(script3Layoutg);
    script3Layout->addLayout(script3Layouth);
    script3Layout->addLayout(script3Layouti);
    script3Layout->addLayout(script3Layoutj);
    script3Layout->addLayout(script3Layoutk);
    script3Layout->addLayout(script3Layoutl);


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
    tb_check_position_x = new QLineEdit("");
    tb_check_position_x->setFixedWidth(40);
    tb_check_position_y = new QLineEdit("");
    tb_check_position_y->setFixedWidth(40);
    tb_check_position_x_radio = new QLineEdit("");
    tb_check_position_x_radio->setFixedWidth(54);
    tb_check_position_y_radio = new QLineEdit("");
    tb_check_position_y_radio->setFixedWidth(54);
    tb_check_position_x_radio_v = new QLineEdit("");
    tb_check_position_x_radio_v->setFixedWidth(40);
    tb_check_position_y_radio_v = new QLineEdit("");
    tb_check_position_y_radio_v->setFixedWidth(40);
    tb_check_position_r = new QLineEdit("");
    tb_check_position_r->setFixedWidth(30);
    tb_check_position_g = new QLineEdit("");
    tb_check_position_g->setFixedWidth(30);
    tb_check_position_b = new QLineEdit("");
    tb_check_position_b->setFixedWidth(30);
    tb_check_position_a = new QLineEdit("");
    tb_check_position_a->setFixedWidth(30);
    tb_check_position_r_up = new QLineEdit("");
    tb_check_position_r_up->setFixedWidth(30);
    tb_check_position_g_up = new QLineEdit("");
    tb_check_position_g_up->setFixedWidth(30);
    tb_check_position_b_up = new QLineEdit("");
    tb_check_position_b_up->setFixedWidth(30);
    tb_check_position_a_up = new QLineEdit("");
    tb_check_position_a_up->setFixedWidth(30);
    tb_check_position_r_down = new QLineEdit("");
    tb_check_position_r_down->setFixedWidth(30);
    tb_check_position_g_down = new QLineEdit("");
    tb_check_position_g_down->setFixedWidth(30);
    tb_check_position_b_down = new QLineEdit("");
    tb_check_position_b_down->setFixedWidth(30);
    tb_check_position_a_down = new QLineEdit("");
    tb_check_position_a_down->setFixedWidth(30);


    ckConsoleEnable = new QCheckBox("启动控制台");
    ckConsoleEnable->setChecked(true);
    btnConsoleClear = new QPushButton("清空控制台");
    btnWindowPrint = new QPushButton("截图");
    btnWindowPrint->setFixedWidth(60);
    rb_printWindow = new QRadioButton("窗口截图");
    rb_printClient = new QRadioButton("内容截图");
    btnScriptAdd = new QPushButton("新增脚本");
    btnScriptAddRoot = new QPushButton("新增根节点");
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
    bg_scriptType->addButton(rb_scriptTypeCondition,        1);
    bg_scriptType->addButton(rb_scriptTypeDeal,             2);
    bg_scriptType->addButton(rb_scriptTypeConditionAndDeal, 3);

    rb_scriptConditionTypePixel = new QRadioButton("像素点检测");
    rb_scriptConditionTypeEdgeDetect = new QRadioButton("图片边缘检测[禁用]");
    rb_scriptConditionTypeOCR = new QRadioButton("文字OCR检测[禁用]");
    bg_scriptConditionType = new QButtonGroup(this);
    bg_scriptConditionType->addButton(rb_scriptConditionTypePixel,      1);
    bg_scriptConditionType->addButton(rb_scriptConditionTypeEdgeDetect, 2);
    bg_scriptConditionType->addButton(rb_scriptConditionTypeOCR,        3);


    action_addScript = new QAction(tr("新增子节点"), this);
    action_removeScript = new QAction(tr("删除节点"), this);
    menu_scriptContent = new QMenu(this);
    menu_scriptContent->addAction(action_addScript);
    menu_scriptContent->addAction(action_removeScript);
    action_addRootScript = new QAction(tr("新增根节点"), this);
    menu_script = new QMenu(this);
    menu_script->addAction(action_addRootScript);
    action_removeScriptDetail = new QAction(tr("删除"), this);
    menu_scriptDetailContent = new QMenu(this);
    menu_scriptDetailContent->addAction(action_removeScriptDetail);
    action_addRootScriptDetail = new QAction(tr("新增"), this);
    menu_scriptDetail = new QMenu(this);
    menu_scriptDetail->addAction(action_addRootScriptDetail);

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


    //    rightQWidgetGroup1aLayout->addWidget(btnScriptAdd);
    //    rightQWidgetGroup1aLayout->addWidget(btnScriptAddRoot);

    rightQWidgetGroup1aLayout->addWidget(btnScriptSave);

    script3Layouta->addWidget(new QLabel("脚本类型:", this),     0, Qt::AlignLeft);
    script3Layouta->addWidget(rb_scriptTypeCondition,        0, Qt::AlignLeft);
    script3Layouta->addWidget(rb_scriptTypeDeal,             0, Qt::AlignLeft);
    script3Layouta->addWidget(rb_scriptTypeConditionAndDeal, 0, Qt::AlignLeft);


    script3Layoutb->addWidget(new QLabel("条件类型:", this),        0, Qt::AlignLeft);
    script3Layoutb->addWidget(rb_scriptConditionTypePixel,      0, Qt::AlignLeft);
    script3Layoutb->addWidget(rb_scriptConditionTypeEdgeDetect, 0, Qt::AlignLeft);
    script3Layoutb->addWidget(rb_scriptConditionTypeOCR,        0, Qt::AlignLeft);


    rb_check_position = new QRadioButton("像素坐标:", this);
    rb_check_position->setChecked(true);
    rb_check_position_radio = new QRadioButton("像素占比:", this);
    bg_check_position_type = new QButtonGroup(this);
    bg_check_position_type->addButton(rb_check_position,       1);
    bg_check_position_type->addButton(rb_check_position_radio, 2);


    ck_check_color_get = new QCheckBox("允许从截图框选取像素和颜色", this);


    script3Layoutc->addWidget(rb_check_position,     0, Qt::AlignLeft);
    script3Layoutc->addWidget(new QLabel("X", this), 0, Qt::AlignLeft);
    script3Layoutc->addWidget(tb_check_position_x,   0, Qt::AlignLeft);
    script3Layoutc->addWidget(new QLabel("Y", this), 0, Qt::AlignLeft);
    script3Layoutc->addWidget(tb_check_position_y,   0, Qt::AlignLeft);

    script3Layoutd->addWidget(rb_check_position_radio,     0, Qt::AlignLeft);
    script3Layoutd->addWidget(new QLabel("X(%)", this),    0, Qt::AlignLeft);
    script3Layoutd->addWidget(tb_check_position_x_radio,   0, Qt::AlignLeft);
    script3Layoutd->addWidget(new QLabel("Y(%)", this),    0, Qt::AlignLeft);
    script3Layoutd->addWidget(tb_check_position_y_radio,   0, Qt::AlignLeft);
    script3Layoutd->addWidget(new QLabel(" -> ", this),    0, Qt::AlignLeft);
    script3Layoutd->addWidget(new QLabel("X", this),       0, Qt::AlignLeft);
    script3Layoutd->addWidget(tb_check_position_x_radio_v, 0, Qt::AlignLeft);
    script3Layoutd->addWidget(new QLabel("Y", this),       0, Qt::AlignLeft);
    script3Layoutd->addWidget(tb_check_position_y_radio_v, 0, Qt::AlignLeft);


    tb_check_color = new QLabel("");
    tb_check_color->setFixedWidth(24);
    tb_check_color->setStyleSheet(tr("background-color:rgba(0,0,0,255)"));
    script3Layoute->addWidget(new QLabel("像素颜色:", this), 0, Qt::AlignLeft);

    script3Layoute->addWidget(tb_check_color,            0, Qt::AlignLeft);
    script3Layoute->addWidget(new QLabel("R", this),     0, Qt::AlignLeft);
    script3Layoute->addWidget(tb_check_position_r,       0, Qt::AlignLeft);
    script3Layoute->addWidget(new QLabel("G", this),     0, Qt::AlignLeft);
    script3Layoute->addWidget(tb_check_position_g,       0, Qt::AlignLeft);
    script3Layoute->addWidget(new QLabel("B", this),     0, Qt::AlignLeft);
    script3Layoute->addWidget(tb_check_position_b,       0, Qt::AlignLeft);
    script3Layoute->addWidget(new QLabel("A", this),     0, Qt::AlignLeft);
    script3Layoute->addWidget(tb_check_position_a,       0, Qt::AlignLeft);

    ck_check_rgb_up = new QCheckBox("颜色范围(+):", this);
    ck_check_rgb_down = new QCheckBox("颜色范围(-):", this);

    script3Layoutf->addWidget(ck_check_rgb_up,        0, Qt::AlignLeft);
    script3Layoutf->addWidget(new QLabel("R", this),  0, Qt::AlignLeft);
    script3Layoutf->addWidget(tb_check_position_r_up, 0, Qt::AlignLeft);
    script3Layoutf->addWidget(new QLabel("G", this),  0, Qt::AlignLeft);
    script3Layoutf->addWidget(tb_check_position_g_up, 0, Qt::AlignLeft);
    script3Layoutf->addWidget(new QLabel("B", this),  0, Qt::AlignLeft);
    script3Layoutf->addWidget(tb_check_position_b_up, 0, Qt::AlignLeft);
    script3Layoutf->addWidget(new QLabel("A", this),  0, Qt::AlignLeft);
    script3Layoutf->addWidget(tb_check_position_a_up, 0, Qt::AlignLeft);

    script3Layoutg->addWidget(ck_check_rgb_down,        0, Qt::AlignLeft);
    script3Layoutg->addWidget(new QLabel("R", this),    0, Qt::AlignLeft);
    script3Layoutg->addWidget(tb_check_position_r_down, 0, Qt::AlignLeft);
    script3Layoutg->addWidget(new QLabel("G", this),    0, Qt::AlignLeft);
    script3Layoutg->addWidget(tb_check_position_g_down, 0, Qt::AlignLeft);
    script3Layoutg->addWidget(new QLabel("B", this),    0, Qt::AlignLeft);
    script3Layoutg->addWidget(tb_check_position_b_down, 0, Qt::AlignLeft);
    script3Layoutg->addWidget(new QLabel("A", this),    0, Qt::AlignLeft);
    script3Layoutg->addWidget(tb_check_position_a_down, 0, Qt::AlignLeft);


    tb_check_print_size = new QLabel("[截图框图片尺寸参考：]", this);
    script3Layouth->addWidget(ck_check_color_get,  0, Qt::AlignLeft);
    script3Layouth->addWidget(tb_check_print_size, 0, Qt::AlignLeft);


    script3Layouti->addWidget(btnScriptDetailSave, 0, Qt::AlignLeft);

    //    script3Layoutd->addWidget(ckScriptDetailSleep, 0, Qt::AlignLeft);


    layout->addWidget(leftQWidget);

    layout->addWidget(rightQWidget);


    // 这行必须放下面
    saScriptEdit->setWidget(saContentWidget);
}

void Itemview10Script::initConnect()
{
    // 事件 - 保存
    connect(btnScriptDetailSave, &QPushButton::clicked, [this]() {
        if (activeScriptDetailNo > 0) {
            QList<QStandardItem *>items =  scriptDetailGridModel->findItems(QString::number(activeScriptDetailNo), Qt::MatchExactly, 1);

            if (items.count() == 1) {
                QJsonObject json;
                json.insert("scriptType",             bg_scriptType->checkedId());
                json.insert("scriptConditionType",    bg_scriptConditionType->checkedId());
                json.insert("check_position_x",       tb_check_position_x->text().toInt());
                json.insert("check_position_y",       tb_check_position_y->text().toInt());
                json.insert("check_position_x_radio", tb_check_position_x_radio->text().toFloat());
                json.insert("check_position_y_radio", tb_check_position_y_radio->text().toFloat());
                json.insert("bg_check_position_type", bg_check_position_type->checkedId());
                json.insert("check_position_r",       tb_check_position_r->text().toInt());
                json.insert("check_position_g",       tb_check_position_g->text().toInt());
                json.insert("check_position_b",       tb_check_position_b->text().toInt());
                json.insert("check_position_a",       tb_check_position_a->text().toInt());
                json.insert("check_position_r_up",    tb_check_position_r_up->text().toInt());
                json.insert("check_position_g_up",    tb_check_position_g_up->text().toInt());
                json.insert("check_position_b_up",    tb_check_position_b_up->text().toInt());
                json.insert("check_position_a_up",    tb_check_position_a_up->text().toInt());
                json.insert("check_position_r_down",  tb_check_position_r_down->text().toInt());
                json.insert("check_position_g_down",  tb_check_position_g_down->text().toInt());
                json.insert("check_position_b_down",  tb_check_position_b_down->text().toInt());
                json.insert("check_position_a_down",  tb_check_position_a_down->text().toInt());
                json.insert("check_position_down",    ck_check_rgb_down->isChecked());
                json.insert("check_position_up",      ck_check_rgb_up->isChecked());

                json.insert("check_print_width",      check_print_width);
                json.insert("check_print_height",     check_print_height);
                QJsonDocument document;
                document.setObject(json);
                QByteArray jsonStr = document.toJson(QJsonDocument::Compact);


                QStandardItem *item = items.first();

                item->setData(1,       Qt::UserRole + 1); // 设置状态
                item->setData(jsonStr, Qt::UserRole + 2); // 设置配置

                QStandardItem *item0 = scriptDetailGridModel->item(item->row(), 0);
                item0->setData(tr("已配置"), Qt::DisplayRole);

                QStandardItem *item2 = scriptDetailGridModel->item(item->row(), 2);

                if (bg_scriptConditionType->checkedId() == 1) {
                    item2->setData(tr("像素点检测"), Qt::DisplayRole);
                } else if (bg_scriptConditionType->checkedId() == 2) {
                    item2->setData(tr("图形边缘检测"), Qt::DisplayRole);
                }
                activeScriptDetailNo = -1;
                tabScriptWidget->setCurrentIndex(0);
            }
        }
    });


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


    // 事件 - 表格右键
    connect(bg_scriptConditionType, &QButtonGroup::idToggled, [this](int id, bool b) {
        qDebug() << id << b;
    });

    // 事件 -新增子脚本
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


            QModelIndex idIndex = index.siblingAtColumn(2);

            //            QModelIndex parentIndex = index.parent();
            //            QStandardItem *parentItem = scriptGridModel->itemFromIndex(parentIndex);

            int rowCount = parentItem->rowCount();

            int j = 1;

            for (int i = 0; i < rowCount; i++) {
                QModelIndex childIndex2 = index.child(i, 3);

                if (childIndex2.data().toString() == tr("判断")) {
                    j++;
                }
            }

            QStandardItem *newItem = new QStandardItem(tr("%1-A%2").arg(idIndex.data().toString()).arg(j));
            newItem->setCheckable(true);
            newItem->setCheckState(Qt::Unchecked);

            // newItem->setData();

            parentItem->setChild(rowCount, 0, new QStandardItem("未设置"));
            parentItem->setChild(rowCount, 1, new QStandardItem("333"));
            parentItem->setChild(rowCount, 2, newItem);
            parentItem->setChild(rowCount, 3, new QStandardItem("判断"));
        }
    });

    // 事件 -新增根脚本
    connect(action_addRootScript, &QAction::triggered, [this]() {
        QString name;
        int type;
        ScriptTypeDialog scriptTypeDialog(this);
        int ret = scriptTypeDialog.exec(); // 以模态方式显示对话框

        if (ret == QDialog::Accepted)      // OK键被按下
        {
            name = scriptTypeDialog.le_name->text();
            type = scriptTypeDialog.rb_scriptTypeCondition->isChecked() ? 1 : 3;
        } else {
            return;
        }


        int count =  scriptGridModel->rowCount();

        scriptGridModel->setItem(count, 0, new QStandardItem("未设置"));
        scriptGridModel->setItem(count, 1, new QStandardItem(name));

        auto keyItem = new QStandardItem("A1");
        keyItem->setCheckable(true);
        keyItem->setCheckState(Qt::Unchecked);
        keyItem->setData(type);

        scriptGridModel->setItem(count, 2, keyItem);
        scriptGridModel->setItem(count, 3, new QStandardItem(type == 1 ? "判断" : "判断+操作"));
    });


    // 事件 - 表格右键
    connect(scriptTableView, &QTreeView::customContextMenuRequested, [this](const QPoint& pos) {
        QModelIndex index = scriptTableView->indexAt(pos);

        if (index.isValid())
        {
            menu_scriptContent->exec(QCursor::pos());
        } else {
            menu_script->exec(QCursor::pos());
        }
    });


    // 事件 - 双击
    connect(scriptTableView, &QTreeView::clicked, [this](const QModelIndex& current) {
        selectedIdIndex = current.siblingAtColumn(2);


        activeScriptType = selectedIdIndex.data(Qt::UserRole + 1).toInt();
        tabScriptWidget->setTabText(0, tr("脚本明细列表 [%1]").arg(selectedIdIndex.data().toString()));
    });


    // 事件 - 表格右键
    connect(scriptDetailTableView, &QTreeView::customContextMenuRequested, [this](const QPoint& pos) {
        QModelIndex index = scriptDetailTableView->indexAt(pos);

        if (index.isValid())
        {
            menu_scriptDetailContent->exec(QCursor::pos());
        } else {
            menu_scriptDetail->exec(QCursor::pos());
        }
    });


    // 事件 -新增根脚本
    connect(action_addRootScriptDetail, &QAction::triggered, [this]() {
        if (selectedIdIndex.isValid()) {
            int count =  scriptDetailGridModel->rowCount();

            scriptDetailGridModel->setItem(count, 0, new QStandardItem("未设置"));
            auto key = new QStandardItem(tr("%1").arg(count + 1));
            key->setData(-1, Qt::UserRole + 1); // 设置状态
            scriptDetailGridModel->setItem(count, 1, key);
            scriptDetailGridModel->setItem(count, 2, new QStandardItem("待设置"));
            scriptDetailGridModel->setItem(count, 3, new QStandardItem("无"));
            tabScriptWidget->setCurrentIndex(1);
            tabScriptWidget->setTabText(1, tr("脚本明细编辑 [%1]").arg(count + 1));
            activeScriptDetailType = -1;
            activeScriptDetailStatus = -1;
            activeScriptDetailNo = count + 1;
            buildScriptDetailEdit();
        } else {
            QMessageBox::warning(this, "警告", "请先选中一条脚本再编辑");
        }
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


    // 事件 - 图片框的鼠标移动
    connect(pixmapWidget, &PixmapWidget::mousePositionEvent, [this](int mouseX, int mouseY, QColor& color, float   xr, float   yr) {
        tb_mouse_position->setText(tr("[x:%1, y:%2]").arg(mouseX).arg(mouseY));
        tb_mouse_rgb->setText(tr("[r:%1, g:%2, b:%3, a:%4]").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
        tb_mouse_rgb->setStyleSheet(tr("background-color:rgba(%1,%2,%3,%4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
        xr = round(xr * 100 * 100) / 100.0;
        yr = round(yr * 100 * 100) / 100.0;
        tb_mouse_ratio->setText(tr("[x:%1%, y:%2%]").arg(QString::number(xr, 'f', 2)).arg(QString::number(yr, 'f', 2)));
        xr = xr / 100.0;
        yr = yr / 100.0;
        xr = xr * pixmapSize.width();
        yr = yr * pixmapSize.height();
        tb_mouse_ratio_position->setText(tr("[x:%1, y:%2]").arg(round(xr)).arg(round(yr)));
        qDebug() << round(xr) << ":" << (int)xr;
        qDebug() << round(yr) << ":" << (int)yr;
    });


    // 事件 - 图片框的鼠标点击
    connect(pixmapWidget, &PixmapWidget::mouseClicked, [this](int mouseX, int mouseY, QColor& color, float   xr, float   yr) {
        if (ck_check_color_get->isChecked()) {
            tb_check_position_x->setText(tr("%1").arg(mouseX));
            tb_check_position_y->setText(tr("%1").arg(mouseY));
            tb_check_position_x_radio->setText(tr("%1").arg(QString::number(xr * 100, 'f', 2)));
            tb_check_position_y_radio->setText(tr("%1").arg(QString::number(yr * 100, 'f', 2)));

            xr = xr * pixmapSize.width();
            yr = yr * pixmapSize.height();
            tb_check_position_x_radio_v->setText(tr("%1").arg(round(xr)));
            tb_check_position_y_radio_v->setText(tr("%1").arg(round(yr)));
            tb_check_position_r->setText(tr("%1").arg(color.red()));
            tb_check_position_g->setText(tr("%1").arg(color.green()));
            tb_check_position_b->setText(tr("%1").arg(color.blue()));
            tb_check_position_a->setText(tr("%1").arg(color.alpha()));
            tb_check_color->setStyleSheet(tr("background-color:rgba(%1,%2,%3,%4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
            check_print_height = pixmapSize.height();
            check_print_width = pixmapSize.width();
            tb_check_print_size->setText(tr("[截图框图片尺寸参考：%1 x %2]").arg(pixmapSize.width()).arg(pixmapSize.height()));
        }
    });


    // 截图
    connect(btnWindowPrint, &QPushButton::clicked, [this]() {
        print();
    });


    connect(btnConsoleClear, &QPushButton::clicked, [this]() {
        clearMessage();
    });
}

void Itemview10Script::buildScriptDetailEdit() {
    rb_scriptTypeCondition->setVisible(activeScriptType == 1 ? true : false);
    rb_scriptTypeDeal->setVisible(activeScriptType == 2 ? true : false);
    rb_scriptTypeConditionAndDeal->setVisible(activeScriptType == 3 ? true : false);
    rb_scriptTypeCondition->setCheckable(activeScriptType == 1 ? true : false);
    rb_scriptTypeDeal->setCheckable(activeScriptType == 2 ? true : false);
    rb_scriptTypeConditionAndDeal->setCheckable(activeScriptType == 3 ? true : false);
    rb_scriptTypeCondition->setChecked(activeScriptType == 1 ? true : false);
    rb_scriptTypeDeal->setChecked(activeScriptType == 2 ? true : false);
    rb_scriptTypeConditionAndDeal->setChecked(activeScriptType == 3 ? true : false);


    rb_scriptConditionTypePixel->setChecked(activeScriptDetailType == 1 ? true : false);
    rb_scriptConditionTypeEdgeDetect->setChecked(activeScriptDetailType == 2 ? true : false);
    rb_scriptConditionTypeOCR->setChecked(activeScriptDetailType == 3 ? true : false);
    rb_scriptConditionTypeEdgeDetect->setCheckable(false);
    rb_scriptConditionTypeOCR->setCheckable(false);


    if (activeScriptDetailStatus == -1) {
        tb_check_position_r_down->setText("0");
        tb_check_position_g_down->setText("0");
        tb_check_position_b_down->setText("0");
        tb_check_position_r_up->setText("0");
        tb_check_position_g_up->setText("0");
        tb_check_position_b_up->setText("0");
        rb_scriptConditionTypePixel->setChecked(true);
    }
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
    pixmapSize = map.size();
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

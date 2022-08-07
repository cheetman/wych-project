#include "itemview10pe.h"
#include "mainwindow.h"
#include "utils.h"

#include <QTextCodec>


ItemView10PE::ItemView10PE(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnect();
}

ItemView10PE::~ItemView10PE()
{}

void ItemView10PE::initUI()
{
    auto layout = new QHBoxLayout(this);
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);
    auto leftQWidgetGroupBox1 = new QGroupBox("DOS头", this);

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
    btnStart = new QPushButton("选择文件");
    btnConsoleClear = new QPushButton("清空控制台");
    btnClients = new QPushButton("刷新客户端");


    dos_e_magic = new QLineEdit("e_magic");
    dos_e_lfanew = new QLineEdit("e_lfanew");


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


    leftQWidgetGroup1Layout->addWidget(new QLabel("e_magic:"),  0, 0);
    leftQWidgetGroup1Layout->addWidget(new QLabel("e_lfanew:"), 1, 0);
    leftQWidgetGroup1Layout->addWidget(btnStart,                2, 0);
    leftQWidgetGroup1Layout->addWidget(dos_e_magic,             0, 1);
    leftQWidgetGroup1Layout->addWidget(dos_e_lfanew,            1, 1);

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

void ItemView10PE::initConnect()
{
    connect(btnStart, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("文件对话框！"), "F:", tr("动态链接库(*dll *jpg);;" "执行文件(*exe)"));
        auto str = fileName.toStdString();
        auto path = str.c_str();
        LPVOID pFileBuffer = NULL;

        auto size = Utils::ReadFile(path, &pFileBuffer);

        if ((size == NULL) || (pFileBuffer == NULL)) {
            return;
        }


        // 获取DosHeader
        if (*((PWORD)pFileBuffer) != IMAGE_DOS_SIGNATURE) {
            QMessageBox::warning(this, "警告", "the first word is not MZ!");
        }
    });
}

#include "itemview10pe.h"

// #include "mainwindow.h"
#include "utils.h"

#include <QDateTime>
#include <QTextCodec>
#include <tchar.h>

#include <structs/pe.h>


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

    // 第一层
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);

    //    leftQWidget->setFixedWidth(300);

    // 第二层
    auto leftQWidgetGroupBox1 = new QGroupBox("DOS头", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout = new QGridLayout(leftQWidgetGroupBox1);

    //    leftQWidgetGroupBox1->setFixedHeight(120);


    auto leftQWidgetGroupBox2 = new QGroupBox("PE头", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout2 = new QGridLayout(leftQWidgetGroupBox2);
    leftQWidgetGroupBox2->setFixedHeight(150);


    auto leftQWidgetGroupBox3 = new QGroupBox("可选PE头", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox3);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout3 = new QGridLayout(leftQWidgetGroupBox3);
    leftQWidgetGroupBox3->setFixedHeight(500);


    // 第一层(中间列)
    auto centerQWidget = new QWidget(this);
    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);


    // 第二层(中间列)

    auto centerQWidgetGroupBox2 = new QGroupBox("数据目录表", centerQWidget);
    auto centerQWidgetGroupBox2Layout = new QGridLayout(centerQWidgetGroupBox2);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox2);

    auto centerQWidgetGroupBox3 = new QGroupBox("节表", centerQWidget);
    auto centerQWidgetGroupBox3Layout = new QVBoxLayout(centerQWidgetGroupBox3);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox3);

    tableTableView = new QTableView(this);
    tableGridModel = new QStandardItemModel();
    tableGridModel->setHorizontalHeaderLabels({  "Name", "VirtualSize",  "SizeOfRawData", "VirtualAddress", "PointerToRawData", "Characteristics" });
    tableTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableTableView->setModel(tableGridModel);
    centerQWidgetGroupBox3Layout->addWidget(tableTableView);

    auto centerQWidgetGroupBox1 = new QGroupBox("控制台设置", centerQWidget);
    auto centerQWidgetGroupBox1Layout = new QHBoxLayout(centerQWidgetGroupBox1);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox1);

    edtMsg = new QPlainTextEdit();
    edtMsg->setReadOnly(true);
    centerQWidgetLayout->addWidget(edtMsg);


    // 第一层(右边列)
    auto rightQWidget = new QWidget(this);
    auto rightQWidgetLayout = new QVBoxLayout(rightQWidget);


    // 第二层(右边列)
    auto rightQWidgetGroupBox1 = new QGroupBox("数据目录表明细", rightQWidget);
    auto rightQWidgetGroupBox1Layout = new QVBoxLayout(rightQWidgetGroupBox1);

    // 第三层
    auto tabTabWidget = new QTabWidget(rightQWidgetGroupBox1);
    auto tab = new QWidget(tabTabWidget);
    auto exportTabTabWidgetLayout = new QVBoxLayout(tab);
    auto tab2 = new QWidget(tabTabWidget);
    auto importTabTabWidgetLayout = new QVBoxLayout(tab2);
    auto tab3 = new QWidget(tabTabWidget);
    auto relocationTabTabWidgetLayout = new QVBoxLayout(tab3);
    tabTabWidget->addTab(tab,  tr("导出表"));
    tabTabWidget->addTab(tab2, tr("导入表"));
    tabTabWidget->addTab(tab3, tr("重定位表"));
    rightQWidgetGroupBox1Layout->addWidget(tabTabWidget);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    // 第四层(导出)
    auto exportTabTabWidgetGroupBox1 = new QGroupBox("头部", tab);
    exportTabTabWidgetLayout->addWidget(exportTabTabWidgetGroupBox1);

    exportTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto exportTabTabWidgetGroupBox1Layout = new QGridLayout(exportTabTabWidgetGroupBox1);
    exportTabTabWidgetGroupBox1->setFixedHeight(200);


    auto exportTabTabWidgetGroupBox2 = new QGroupBox("明细", tab);
    exportTabTabWidgetLayout->addWidget(exportTabTabWidgetGroupBox2);
    exportTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto exportTabTabWidgetGroupBox2Layout = new QGridLayout(exportTabTabWidgetGroupBox2);
    exportTabTabWidgetGroupBox1->setFixedHeight(200);
    exportTableView = new QTableView(this);
    exportGridModel = new QStandardItemModel();
    exportGridModel->setHorizontalHeaderLabels({  "索引",  "地址_RVA", "地址_FOA", "序号表",  "函数名" });
    exportTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    exportTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    exportTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    exportTableView->setModel(exportGridModel);

    //    exportTableView->setColumnHidden(0, true);
    exportTabTabWidgetGroupBox2Layout->addWidget(exportTableView);


    // 第四层(重定位)
    auto relocationTabTabWidgetGroupBox = new QGroupBox("区段", tab3);
    relocationTabTabWidgetLayout->addWidget(relocationTabTabWidgetGroupBox);
    relocationTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto relocationTabTabWidgetGroupBoxLayout = new QGridLayout(relocationTabTabWidgetGroupBox);
    relocationTabTabWidgetGroupBox->setFixedHeight(250);
    relocationTableView = new QTableView(this);
    relocationGridModel = new QStandardItemModel();
    relocationGridModel->setHorizontalHeaderLabels({  "所属节",  "RVA", "数量", "大小(字节)" });
    relocationTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    relocationTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    relocationTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    relocationTableView->setModel(relocationGridModel);
    relocationTabTabWidgetGroupBoxLayout->addWidget(relocationTableView);

    auto relocationTabTabWidgetGroupBox2 = new QGroupBox("块", tab3);
    relocationTabTabWidgetLayout->addWidget(relocationTabTabWidgetGroupBox2);
    relocationTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto relocationTabTabWidgetGroupBoxLayout2 = new QGridLayout(relocationTabTabWidgetGroupBox2);
    relocation2TableView = new QTableView(this);
    relocation2GridModel = new QStandardItemModel();
    relocation2GridModel->setHorizontalHeaderLabels({  "RVA",  "FOA" });
    relocation2TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    relocation2TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    relocation2TableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    relocation2TableView->setModel(relocation2GridModel);
    relocationTabTabWidgetGroupBoxLayout2->addWidget(relocation2TableView);


    // 第四层(导入)

    auto importTabTabWidgetGroupBox3 = new QGroupBox("操作", tab2);
    importTabTabWidgetLayout->addWidget(importTabTabWidgetGroupBox3);
    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto importTabTabWidgetGroupBoxLayout3 = new QHBoxLayout(importTabTabWidgetGroupBox3);

    //    importTabTabWidgetGroupBox3->setFixedHeight(60);
    btnImportAdd = new QPushButton("新增DLL");
    importTabTabWidgetGroupBoxLayout3->addWidget(btnImportAdd);

    auto importTabTabWidgetGroupBox = new QGroupBox("DLL", tab2);
    importTabTabWidgetLayout->addWidget(importTabTabWidgetGroupBox);
    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto importTabTabWidgetGroupBoxLayout = new QGridLayout(importTabTabWidgetGroupBox);
    importTabTabWidgetGroupBox->setFixedHeight(250);
    importTableView = new QTableView(this);
    importGridModel = new QStandardItemModel();
    importGridModel->setHorizontalHeaderLabels({  "DLL名称",  "OriginFirsthunk[INT]", "FOA", "Firsthunk[IAT]", "FOA" });
    importTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    importTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    importTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    importTableView->setModel(importGridModel);
    importTabTabWidgetGroupBoxLayout->addWidget(importTableView);

    auto importTabWidgetGroupBox2 = new QGroupBox("函数", tab2);
    importTabTabWidgetLayout->addWidget(importTabWidgetGroupBox2);
    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto importTabWidgetGroupBoxLayout2 = new QGridLayout(importTabWidgetGroupBox2);
    import2TableView = new QTableView(this);
    import2GridModel = new QStandardItemModel();
    import2GridModel->setHorizontalHeaderLabels({ "查找表FOA(+4/+8)",  "提示/名称表RVA",  "提示/名称表FOA", "函数名称", "HINT[导出表索引]" });
    import2TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    import2TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    import2TableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    import2TableView->setModel(import2GridModel);
    importTabWidgetGroupBoxLayout2->addWidget(import2TableView);


    // 控件
    ckConsoleEnable = new QCheckBox("启动控制台");

    //    ckRefreshClients = new QCheckBox("刷新客户端");
    btnStart = new QPushButton("选择文件");

    btnConsoleClear = new QPushButton("清空控制台");

    //    btnClients = new QPushButton("刷新客户端");


    dos_e_magic = new QLineEdit("");
    dos_e_lfanew = new QLineEdit("");
    pe_Machine = new QLineEdit("");
    pe_NumberOfSections = new QLineEdit("");
    pe_TimeDateStamp = new QLineEdit("");
    pe_SizeOfOptionHeader = new QLineEdit("");
    pe_Characteristics = new QLineEdit("");

    ope_Magic = new QLineEdit("");
    ope_SizeOfCode = new QLineEdit("");
    ope_SizeOfInitializedData = new QLineEdit("");
    ope_SizeOfUninitializedData = new QLineEdit("");
    ope_AddressOfEntryPoint_RVA = new QLineEdit("");
    ope_BaseOfCode = new QLineEdit("");
    ope_BaseOfData = new QLineEdit("");
    ope_ImageBase = new QLineEdit("");
    ope_SectionAlignment = new QLineEdit("");
    ope_FileAlignment = new QLineEdit("");
    ope_SizeOfImage = new QLineEdit("");
    ope_SizeOfHeaders = new QLineEdit("");
    ope_CheckSum = new QLineEdit("");
    ope_SizeOfStackReserve = new QLineEdit("");
    ope_SizeOfStackCommit = new QLineEdit("");
    ope_SizeOfHeapReserve = new QLineEdit("");
    ope_SizeOfHeapCommit = new QLineEdit("");
    ope_NumberOfRvaAndSizes = new QLineEdit("");

    tb_export_rva = new QLineEdit("");
    tb_import_rva = new QLineEdit("");
    tb_resource_rva = new QLineEdit("");
    tb_base_relocation_rva = new QLineEdit("");
    tb_export_size = new QLineEdit("");
    tb_import_size = new QLineEdit("");
    tb_resource_size = new QLineEdit("");
    tb_base_relocation_size = new QLineEdit("");
    tb_export_foa = new QLineEdit("");
    tb_import_foa = new QLineEdit("");
    tb_resource_foa = new QLineEdit("");
    tb_base_relocation_foa = new QLineEdit("");

    export_Name = new QLineEdit("");
    export_Base = new QLineEdit("");
    export_NumberOfFunctions = new QLineEdit("");
    export_NumberOfNames = new QLineEdit("");
    export_AddressOfFunctions = new QLineEdit("");
    export_AddressOfNames = new QLineEdit("");
    export_AddressOfNameOrdinals = new QLineEdit("");
    export_AddressOfFunctions_foa = new QLineEdit("");
    export_AddressOfNames_foa = new QLineEdit("");
    export_AddressOfNameOrdinals_foa = new QLineEdit("");


    //    infoTableView = new QTableView(this);
    //    infoGridModel = new QStandardItemModel();

    //    /* 设置表格标题行(输入数据为QStringList类型) */
    //    infoGridModel->setHorizontalHeaderLabels({  "类型", "ID", "地址", "状态" });
    //    infoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    infoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    infoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    infoTableView->setModel(infoGridModel);


    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    auto lb_e_magic = new QLabel("e_magic:");
    auto lb_e_lfanew = new QLabel("e_lfanew:");
    lb_e_magic->setPalette(pe);
    lb_e_lfanew->setPalette(pe);
    leftQWidgetGroup1Layout->addWidget(lb_e_magic,   0, 0);
    leftQWidgetGroup1Layout->addWidget(lb_e_lfanew,  1, 0);
    leftQWidgetGroup1Layout->addWidget(btnStart,     2, 0);
    leftQWidgetGroup1Layout->addWidget(dos_e_magic,  0, 1);
    leftQWidgetGroup1Layout->addWidget(dos_e_lfanew, 1, 1);


    auto lb_Machine = new QLabel("Machine:");
    auto lb_NumberOfSections = new QLabel("NumberOfSections:");
    auto lb_SizeOfOptionHeader = new QLabel("SizeOfOptionHeader:");
    lb_Machine->setPalette(pe);
    lb_NumberOfSections->setPalette(pe);
    lb_SizeOfOptionHeader->setPalette(pe);
    leftQWidgetGroup1Layout2->addWidget(lb_Machine,                     0, 0);
    leftQWidgetGroup1Layout2->addWidget(lb_NumberOfSections,            1, 0);
    leftQWidgetGroup1Layout2->addWidget(new QLabel("TimeDateStamp:"),   2, 0);
    leftQWidgetGroup1Layout2->addWidget(lb_SizeOfOptionHeader,          3, 0);
    leftQWidgetGroup1Layout2->addWidget(new QLabel("Characteristics:"), 4, 0);
    leftQWidgetGroup1Layout2->addWidget(pe_Machine,                     0, 1);
    leftQWidgetGroup1Layout2->addWidget(pe_NumberOfSections,            1, 1);
    leftQWidgetGroup1Layout2->addWidget(pe_TimeDateStamp,               2, 1);
    leftQWidgetGroup1Layout2->addWidget(pe_SizeOfOptionHeader,          3, 1);
    leftQWidgetGroup1Layout2->addWidget(pe_Characteristics,             4, 1);


    auto lb_Magic = new QLabel("Magic:");
    auto lb_ImageBase = new QLabel("ImageBase:");
    auto lb_SectionAlignment = new QLabel("SectionAlignment:");
    auto lb_FileAlignment = new QLabel("FileAlignment:");
    auto lb_AddressOfEntryPoint_RVA = new QLabel("AddressOfEntryPoint_RVA:");
    auto lb_SizeOfImage = new QLabel("SizeOfImage:");
    auto lb_SizeOfHeaders = new QLabel("SizeOfHeaders:");
    lb_Magic->setPalette(pe);
    lb_AddressOfEntryPoint_RVA->setPalette(pe);
    lb_ImageBase->setPalette(pe);
    lb_SectionAlignment->setPalette(pe);
    lb_FileAlignment->setPalette(pe);
    lb_SizeOfImage->setPalette(pe);
    lb_SizeOfHeaders->setPalette(pe);
    leftQWidgetGroup1Layout3->addWidget(lb_Magic,                               0,  0); // 10B 32位，20B 64位文件
    leftQWidgetGroup1Layout3->addWidget(new QLabel("SizeOfCode:"),              1,  0); // 没用 FileAlignment 整数倍
    leftQWidgetGroup1Layout3->addWidget(new QLabel("SizeOfInitializedData:"),   2,  0); // 没用 FileAlignment 整数倍
    leftQWidgetGroup1Layout3->addWidget(new QLabel("SizeOfUninitializedData:"), 3,  0); // 没用 FileAlignment 整数倍
    leftQWidgetGroup1Layout3->addWidget(lb_AddressOfEntryPoint_RVA,             4,  0); // 入口
    leftQWidgetGroup1Layout3->addWidget(new QLabel("BaseOfCode:"),              5,  0); // 没用 代码开始的基址
    leftQWidgetGroup1Layout3->addWidget(new QLabel("BaseOfData:"),              6,  0); // 没用 数据开始的基址
    leftQWidgetGroup1Layout3->addWidget(lb_ImageBase,                           7,  0); // 内存镜像基址
    leftQWidgetGroup1Layout3->addWidget(lb_SectionAlignment,                    8,  0); // 内存对齐
    leftQWidgetGroup1Layout3->addWidget(lb_FileAlignment,                       9,  0); // 文件对齐
    leftQWidgetGroup1Layout3->addWidget(lb_SizeOfImage,                         10, 0); // 内存中整个PE文件的映射尺寸，SectionAlignment整数倍
    leftQWidgetGroup1Layout3->addWidget(lb_SizeOfHeaders,                       11, 0); // 所有头+节表按照文件对齐后的大小，按照FileAlignment对齐
    leftQWidgetGroup1Layout3->addWidget(new QLabel("CheckSum:"),                12, 0); // 有些有校验
    leftQWidgetGroup1Layout3->addWidget(new QLabel("SizeOfStackReserve:"),      13, 0); // 初始化保留的堆栈大小
    leftQWidgetGroup1Layout3->addWidget(new QLabel("SizeOfStackCommit:"),       14, 0); // 初始化实际提交的大小
    leftQWidgetGroup1Layout3->addWidget(new QLabel("SizeOfHeapReserve:"),       15, 0); // 初始化保留的堆大小
    leftQWidgetGroup1Layout3->addWidget(new QLabel("SizeOfHeapCommit:"),        16, 0); // 初始化实际提交的大小
    leftQWidgetGroup1Layout3->addWidget(new QLabel("NumberOfRvaAndSizes:"),     17, 0); //

    leftQWidgetGroup1Layout3->addWidget(ope_Magic,                              0,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfCode,                         1,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfInitializedData,              2,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfUninitializedData,            3,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_AddressOfEntryPoint_RVA,            4,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_BaseOfCode,                         5,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_BaseOfData,                         6,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_ImageBase,                          7,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_SectionAlignment,                   8,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_FileAlignment,                      9,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfImage,                        10, 1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfHeaders,                      11, 1);
    leftQWidgetGroup1Layout3->addWidget(ope_CheckSum,                           12, 1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfStackReserve,                 13, 1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfStackCommit,                  14, 1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfHeapReserve,                  15, 1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfHeapCommit,                   16, 1);
    leftQWidgetGroup1Layout3->addWidget(ope_NumberOfRvaAndSizes,                17, 1);

    centerQWidgetGroupBox2Layout->addWidget(new QLabel("导出表:"),      1, 0);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("导入表:"),      2, 0);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("资源表:"),      3, 0);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("重定位表:"),     4, 0);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("大小:"),       0, 1);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("RVA:"),      0, 2);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("FOA:"),      0, 3);
    centerQWidgetGroupBox2Layout->addWidget(tb_export_size,          1, 1);
    centerQWidgetGroupBox2Layout->addWidget(tb_import_size,          2, 1);
    centerQWidgetGroupBox2Layout->addWidget(tb_resource_size,        3, 1);
    centerQWidgetGroupBox2Layout->addWidget(tb_base_relocation_size, 4, 1);
    centerQWidgetGroupBox2Layout->addWidget(tb_export_rva,           1, 2);
    centerQWidgetGroupBox2Layout->addWidget(tb_import_rva,           2, 2);
    centerQWidgetGroupBox2Layout->addWidget(tb_resource_rva,         3, 2);
    centerQWidgetGroupBox2Layout->addWidget(tb_base_relocation_rva,  4, 2);
    centerQWidgetGroupBox2Layout->addWidget(tb_export_foa,           1, 3);
    centerQWidgetGroupBox2Layout->addWidget(tb_import_foa,           2, 3);
    centerQWidgetGroupBox2Layout->addWidget(tb_resource_foa,         3, 3);
    centerQWidgetGroupBox2Layout->addWidget(tb_base_relocation_foa,  4, 3);

    centerQWidgetGroupBox1Layout->addWidget(ckConsoleEnable);
    centerQWidgetGroupBox1Layout->addWidget(btnConsoleClear);

    //    rightQWidgetGroupBox1Layout->addWidget(ckRefreshClients);
    //    rightQWidgetGroupBox1Layout->addWidget(      btnClients);
    //    rightQWidgetLayout->addWidget(infoTableView);

    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("名称:"),                    1, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("Base:"),                  2, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("NumberOfFunctions:"),     3, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("NumberOfNames:"),         4, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("AddressOfFunctions:"),    5, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("AddressOfNameOrdinals:"), 6, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("AddressOfNames:"),        7, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_Name,                          1, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_Base,                          2, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_NumberOfFunctions,             3, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_NumberOfNames,                 4, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_AddressOfFunctions,            5, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_AddressOfNames,                6, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_AddressOfNameOrdinals,         7, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_AddressOfFunctions_foa,        5, 3);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_AddressOfNameOrdinals_foa,     6, 3);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_AddressOfNames_foa,            7, 3);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("FOA:"),                   5, 2);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("FOA:"),                   6, 2);
    exportTabTabWidgetGroupBox1Layout->addWidget(new QLabel("FOA:"),                   7, 2);


    layout->addWidget(leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget(rightQWidget);
}

void ItemView10PE::initConnect()
{
    connect(btnImportAdd, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("文件对话框！"), "F:", tr("动态链接库(*dll *exe)"));

        if (fileName.isEmpty()) {
            return;
        }
        auto path = (wchar_t *)fileName.utf16();

        InjectImportTable();

        //        CreateNewSection(pFileBuffer, pDosHeader, pNTHeader32, pSectionHeader, fileSize, 0x2000, t_NameOfNewSectionHeader);
    });


    connect(btnStart, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("文件对话框！"), "F:", tr("动态链接库(*dll *exe);;" "执行文件(*exe)"));

        if (fileName.isEmpty()) {
            appendConsole("请选择一个文件");
            return;
        }
        auto path = (wchar_t *)fileName.utf16();

        // 转char*
        //        auto str = fileName.toStdString();
        //        auto path = str.c_str();
        //        if (pFileBuffer) {
        //            free(pFileBuffer);
        //        }

        if (pe) {
            free(pe);
            pe = NULL;
        }
        pe = new PE(path, this);

        if (!pe->build()) {
            return;
        }

        //        fileSize = Utils::ReadFile(path, &pFileBuffer);

        //        if ((fileSize == NULL) || (pFileBuffer == NULL)) {
        //            return;
        //        }


        wcscpy(FilePath, path);


        dos_e_magic->setText(QString::number(pe->m_lpDosHeader->e_magic, 16));
        dos_e_lfanew->setText(QString::number(pe->m_lpDosHeader->e_lfanew, 16));

        // 判断32位
        if (pe->isX86()) {
            // 1.文件头
            pe_Machine->setText(QString::number(pe->m_lpNtHeader32->FileHeader.Machine, 16).toUpper());                         // 程序运行的CPU型号
            pe_NumberOfSections->setText(QString::number(pe->m_lpNtHeader32->FileHeader.NumberOfSections, 16).toUpper());       // 节的数量
            pe_TimeDateStamp->setText(QString::number(pe->m_lpNtHeader32->FileHeader.TimeDateStamp, 16).toUpper());             // 时间戳，编译器填写
            pe_SizeOfOptionHeader->setText(QString::number(pe->m_lpNtHeader32->FileHeader.SizeOfOptionalHeader, 16).toUpper()); // 可选PE头的大小 32位文件默认E0h，64位PE文件默认为F0h，可自定义
            pe_Characteristics->setText(QString::number(pe->m_lpNtHeader32->FileHeader.Characteristics, 16).toUpper());

            // 2.可选文件头
            ope_Magic->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.Magic, 16).toUpper());
            ope_SizeOfCode->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfCode, 16).toUpper());
            ope_SizeOfInitializedData->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfInitializedData, 16).toUpper());
            ope_SizeOfUninitializedData->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfUninitializedData, 16).toUpper());
            ope_AddressOfEntryPoint_RVA->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.AddressOfEntryPoint, 16).toUpper());
            ope_BaseOfCode->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.BaseOfCode, 16).toUpper());
            ope_BaseOfData->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.BaseOfData, 16).toUpper());
            ope_ImageBase->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.ImageBase, 16).toUpper());
            ope_SectionAlignment->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SectionAlignment, 16).toUpper());
            ope_FileAlignment->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.FileAlignment, 16).toUpper());
            ope_SizeOfImage->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfImage, 16).toUpper());
            ope_SizeOfHeaders->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfHeaders, 16).toUpper());
            ope_CheckSum->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.CheckSum, 16).toUpper());
            ope_SizeOfStackReserve->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfStackReserve, 16).toUpper());
            ope_SizeOfStackCommit->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfStackCommit, 16).toUpper());
            ope_SizeOfHeapReserve->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfHeapReserve, 16).toUpper());
            ope_SizeOfHeapCommit->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.SizeOfHeapCommit, 16).toUpper());
            ope_NumberOfRvaAndSizes->setText(QString::number(pe->m_lpNtHeader32->OptionalHeader.NumberOfRvaAndSizes, 16).toUpper());

            // 3.数据目录
            tb_export_size->setText(QString::number(pe->m_lpExportTable->Size, 16).toUpper());
            tb_import_size->setText(QString::number(pe->m_lpImportTable->Size, 16).toUpper());
            tb_resource_size->setText(QString::number(pe->m_lpResourceTable->Size, 16).toUpper());
            tb_base_relocation_size->setText(QString::number(pe->m_lpRelocationTable->Size, 16).toUpper());
            tb_export_rva->setText(QString::number(pe->m_lpExportTable->VirtualAddress, 16).toUpper());
            tb_import_rva->setText(QString::number(pe->m_lpImportTable->VirtualAddress, 16).toUpper());
            tb_resource_rva->setText(QString::number(pe->m_lpResourceTable->VirtualAddress, 16).toUpper());
            tb_base_relocation_rva->setText(QString::number(pe->m_lpRelocationTable->VirtualAddress, 16).toUpper());

            tb_export_foa->setText(QString::number(pe->m_foaExportTable, 16).toUpper());
            tb_import_foa->setText(QString::number(pe->m_foaImportTable, 16).toUpper());
            tb_resource_foa->setText(QString::number(pe->m_foaResourceTable, 16).toUpper());
            tb_base_relocation_foa->setText(QString::number(pe->m_foaRelocationTable, 16).toUpper());


            // 4.节表
            for (int i = 0; i < pe->m_lpNtHeader32->FileHeader.NumberOfSections; i++)
            {
                tableGridModel->setItem(i, 0, new QStandardItem(tr((char *)(pe->m_lpSecHeader[i].Name))));
                tableGridModel->setItem(i, 1, new QStandardItem(QString::number(pe->m_lpSecHeader[i].Misc.VirtualSize, 16).toUpper()));
                tableGridModel->setItem(i, 2, new QStandardItem(QString::number(pe->m_lpSecHeader[i].SizeOfRawData, 16).toUpper()));
                tableGridModel->setItem(i, 3, new QStandardItem(QString::number(pe->m_lpSecHeader[i].VirtualAddress, 16).toUpper()));
                tableGridModel->setItem(i, 4, new QStandardItem(QString::number(pe->m_lpSecHeader[i].PointerToRawData, 16).toUpper()));
                tableGridModel->setItem(i, 5, new QStandardItem(QString::number(pe->m_lpSecHeader[i].Characteristics, 16).toUpper()));
            }

            auto removeCount = tableGridModel->rowCount() - pe->m_lpNtHeader32->FileHeader.NumberOfSections;

            if (removeCount > 0) {
                tableGridModel->removeRows(pe->m_lpNtHeader32->FileHeader.NumberOfSections, removeCount);
            }
        } else {
            pe_Machine->setText(QString::number(pe->m_lpNtHeader64->FileHeader.Machine, 16).toUpper());                         // 程序运行的CPU型号
            pe_NumberOfSections->setText(QString::number(pe->m_lpNtHeader64->FileHeader.NumberOfSections, 16).toUpper());       // 节的数量
            pe_TimeDateStamp->setText(QString::number(pe->m_lpNtHeader64->FileHeader.TimeDateStamp, 16).toUpper());             // 时间戳，编译器填写
            pe_SizeOfOptionHeader->setText(QString::number(pe->m_lpNtHeader64->FileHeader.SizeOfOptionalHeader, 16).toUpper()); // 可选PE头的大小 32位文件默认E0h，64位PE文件默认为F0h，可自定义
            pe_Characteristics->setText(QString::number(pe->m_lpNtHeader64->FileHeader.Characteristics, 16).toUpper());


            ope_Magic->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.Magic, 16).toUpper());
            ope_SizeOfCode->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfCode, 16).toUpper());
            ope_SizeOfInitializedData->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfInitializedData, 16).toUpper());
            ope_SizeOfUninitializedData->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfUninitializedData, 16).toUpper());
            ope_AddressOfEntryPoint_RVA->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.AddressOfEntryPoint, 16).toUpper());
            ope_BaseOfCode->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.BaseOfCode, 16).toUpper());

            //            ope_BaseOfData->setText(QString::number(pNTHeader64->OptionalHeader.BaseOfData, 16).toUpper());
            ope_ImageBase->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.ImageBase, 16).toUpper());
            ope_SectionAlignment->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SectionAlignment, 16).toUpper());
            ope_FileAlignment->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.FileAlignment, 16).toUpper());
            ope_SizeOfImage->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfImage, 16).toUpper());
            ope_SizeOfHeaders->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfHeaders, 16).toUpper());
            ope_CheckSum->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.CheckSum, 16).toUpper());
            ope_SizeOfStackReserve->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfStackReserve, 16).toUpper());
            ope_SizeOfStackCommit->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfStackCommit, 16).toUpper());
            ope_SizeOfHeapReserve->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfHeapReserve, 16).toUpper());
            ope_SizeOfHeapCommit->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.SizeOfHeapCommit, 16).toUpper());
            ope_NumberOfRvaAndSizes->setText(QString::number(pe->m_lpNtHeader64->OptionalHeader.NumberOfRvaAndSizes, 16).toUpper());


            // 3.数据目录
            tb_export_size->setText(QString::number(pe->m_lpExportTable->Size, 16).toUpper());
            tb_import_size->setText(QString::number(pe->m_lpImportTable->Size, 16).toUpper());
            tb_resource_size->setText(QString::number(pe->m_lpResourceTable->Size, 16).toUpper());
            tb_base_relocation_size->setText(QString::number(pe->m_lpRelocationTable->Size, 16).toUpper());
            tb_export_rva->setText(QString::number(pe->m_lpExportTable->VirtualAddress, 16).toUpper());
            tb_import_rva->setText(QString::number(pe->m_lpImportTable->VirtualAddress, 16).toUpper());
            tb_resource_rva->setText(QString::number(pe->m_lpResourceTable->VirtualAddress, 16).toUpper());
            tb_base_relocation_rva->setText(QString::number(pe->m_lpRelocationTable->VirtualAddress, 16).toUpper());

            tb_export_foa->setText(QString::number(pe->m_foaExportTable, 16).toUpper());
            tb_import_foa->setText(QString::number(pe->m_foaImportTable, 16).toUpper());
            tb_resource_foa->setText(QString::number(pe->m_foaResourceTable, 16).toUpper());
            tb_base_relocation_foa->setText(QString::number(pe->m_foaRelocationTable, 16).toUpper());


            // 4.节表
            for (int i = 0; i < pe->m_lpNtHeader64->FileHeader.NumberOfSections; i++)
            {
                tableGridModel->setItem(i, 0, new QStandardItem(tr((char *)(pe->m_lpSecHeader[i].Name))));
                tableGridModel->setItem(i, 1, new QStandardItem(QString::number(pe->m_lpSecHeader[i].Misc.VirtualSize, 16).toUpper()));
                tableGridModel->setItem(i, 2, new QStandardItem(QString::number(pe->m_lpSecHeader[i].SizeOfRawData, 16).toUpper()));
                tableGridModel->setItem(i, 3, new QStandardItem(QString::number(pe->m_lpSecHeader[i].VirtualAddress, 16).toUpper()));
                tableGridModel->setItem(i, 4, new QStandardItem(QString::number(pe->m_lpSecHeader[i].PointerToRawData, 16).toUpper()));
                tableGridModel->setItem(i, 5, new QStandardItem(QString::number(pe->m_lpSecHeader[i].Characteristics, 16).toUpper()));
            }

            auto removeCount = tableGridModel->rowCount() -  pe->m_lpNtHeader64->FileHeader.NumberOfSections;

            if (removeCount > 0) {
                tableGridModel->removeRows(pe->m_lpNtHeader64->FileHeader.NumberOfSections, removeCount);
            }
        }


        // 5.导出表
        if (pe->m_lpExportDirectory) {
            export_Name->setText(QString::number(pe->m_lpExportDirectory->Name, 16).toUpper());
            export_Base->setText(QString::number(pe->m_lpExportDirectory->Base, 16).toUpper());
            export_NumberOfFunctions->setText(QString::number(pe->m_lpExportDirectory->NumberOfFunctions, 16).toUpper());
            export_NumberOfNames->setText(QString::number(pe->m_lpExportDirectory->NumberOfNames, 16).toUpper());
            export_AddressOfFunctions->setText(QString::number(pe->m_lpExportDirectory->AddressOfFunctions, 16).toUpper());
            export_AddressOfNames->setText(QString::number(pe->m_lpExportDirectory->AddressOfNames, 16).toUpper());
            export_AddressOfNameOrdinals->setText(QString::number(pe->m_lpExportDirectory->AddressOfNameOrdinals, 16).toUpper());
            export_AddressOfFunctions_foa->setText(QString::number(pe->m_lpExportDirectory->AddressOfFunctions, 16).toUpper());
            export_AddressOfNames_foa->setText(QString::number(pe->m_lpExportDirectory->AddressOfNames, 16).toUpper());
            export_AddressOfNameOrdinals_foa->setText(QString::number(pe->m_lpExportDirectory->AddressOfNameOrdinals, 16).toUpper());

            // 宽度注意
            PDWORD arr_fun = NULL;
            PDWORD arr_name = NULL;
            PWORD arr_ord = NULL;
            arr_fun = (PDWORD)((size_t)pe->m_lpFileData + pe->m_foaAddressOfFunctions);   // 得到指向AddressOfFunction的指针
            arr_name = (PDWORD)((size_t)pe->m_lpFileData + pe->m_foaAddressOfNames);      // 得到指向AddressOfNames的指针
            arr_ord = (PWORD)((size_t)pe->m_lpFileData + pe->m_foaAddressOfNameOrdinals); // 得到指向AddressOfNameOrdinal的指针

            // 注意遍历顺序(具体我忘了)
            for (DWORD i = 0; i < pe->m_lpExportDirectory->NumberOfFunctions; i++)        // 遍历函数表
            {
                exportGridModel->setItem(i, 0, new QStandardItem(QString::number(i)));
                exportGridModel->setItem(i, 1, new QStandardItem(QString::number(arr_fun[i], 16).toUpper()));
                DWORD foa = 0;
                pe->rvaToFoa(arr_fun[i], &foa);
                exportGridModel->setItem(i, 2, new QStandardItem(QString::number(foa, 16).toUpper()));
            }

            auto removeCount = exportGridModel->rowCount() -  pe->m_lpExportDirectory->NumberOfFunctions;

            if (removeCount > 0) {
                exportGridModel->removeRows(pe->m_lpExportDirectory->NumberOfFunctions, removeCount);
            }


            for (DWORD i = 0; i <  pe->m_lpExportDirectory->NumberOfNames; i++) // 遍历names
            {
                DWORD  name_rva = *(arr_name + i);

                if (name_rva) {
                    DWORD  name_foa = 0;
                    pe->rvaToFoa(name_rva, &name_foa);

                    if (name_foa) {
                        auto ordinal = *(arr_ord + i);
                        exportGridModel->setItem(ordinal, 3, new QStandardItem(QString::number(ordinal, 16).toUpper()));
                        exportGridModel->setItem(ordinal, 4, new QStandardItem(tr((char *)((size_t)pe->m_lpFileData + name_foa))));
                    }
                }
            }
        }


        // 6.重定位表
        if (pe->m_lpRelocationBase) {
            PIMAGE_BASE_RELOCATION lpRelocationTable = pe->m_lpRelocationBase;
            int i = 0;

            while (1)
            {
                if ((lpRelocationTable->SizeOfBlock == 0) && (lpRelocationTable->VirtualAddress == 0)) {
                    break;
                }
                int num_of_addr = (lpRelocationTable->SizeOfBlock - 8) / 2;
                relocationGridModel->setItem(i, 1, new QStandardItem(QString::number(lpRelocationTable->VirtualAddress, 16).toUpper()));
                relocationGridModel->setItem(i, 2, new QStandardItem(QString::number(num_of_addr).toUpper()));
                relocationGridModel->setItem(i, 3, new QStandardItem(QString::number(lpRelocationTable->SizeOfBlock).toUpper()));

// 明细
//                    PDWORD t_pAddr = NULL;
//                    t_pAddr = (PDWORD)((size_t)pRelocationTable + 8);

//                    for (int i = 0; i < num_of_addr; i++)
//                    {
//                        if ((t_pAddr[i] & 0x3000) == 0x3000) // 判断高三位是否为0011
//                        {
//                            (t_pAddr[i] & 0xfff) + pRelocationTable->VirtualAddress;
//                        } else {}

//                        else cout << "the first 4 bits are not 0011!" << endl;
//                    }

                lpRelocationTable = (PIMAGE_BASE_RELOCATION)((size_t)lpRelocationTable + lpRelocationTable->SizeOfBlock);

                i++;
            }
        }


        // 导入表
        if (pe->m_lpImportDescriptor) {
            PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = pe->m_lpImportDescriptor;

            int i = 0;

            while (1)
            {
                if (!pImportDescriptor->TimeDateStamp && !pImportDescriptor->FirstThunk) break;
                DWORD name_FOA = 0;
                pe->rvaToFoa(pImportDescriptor->Name, &name_FOA);


                importGridModel->setItem(i, 0, new QStandardItem(tr((PCHAR)(size_t)pe->m_lpFileData + name_FOA)));

                DWORD INTAddr_FOA = 0;
                DWORD IATAddr_FOA = 0;
                pe->rvaToFoa(pImportDescriptor->OriginalFirstThunk, &INTAddr_FOA);
                pe->rvaToFoa(pImportDescriptor->FirstThunk,         &IATAddr_FOA);

                importGridModel->setItem(i, 1, new QStandardItem(QString::number(pImportDescriptor->OriginalFirstThunk, 16).toUpper()));
                importGridModel->setItem(i, 2, new QStandardItem(QString::number(INTAddr_FOA, 16).toUpper()));
                importGridModel->setItem(i, 3, new QStandardItem(QString::number(pImportDescriptor->FirstThunk, 16).toUpper()));
                importGridModel->setItem(i, 4, new QStandardItem(QString::number(IATAddr_FOA, 16).toUpper()));

//                    assert(INTAddr_FOA & IATAddr_FOA);
                PDWORD pThunkData_INT = NULL;
                PDWORD pThunkData_IAT = NULL;
                pThunkData_INT = (PDWORD)((size_t)pe->m_lpFileData + INTAddr_FOA);
                pThunkData_IAT = (PDWORD)((size_t)pe->m_lpFileData + IATAddr_FOA);

//                    while (*pThunkData_INT && *pThunkData_IAT)
//                    {
//                        if (*pThunkData_INT >> 31)         // 是序号
//                        {
//                            *pThunkData_INT &= 0x7fffffff; // 把最高位去掉
//                        }
//                        else
//                        {
//                            DWORD import_by_name_FOA = 0;
//                            Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, *pThunkData_INT, &import_by_name_FOA);
//                            PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((size_t)pFileBuffer + import_by_name_FOA);
//                            pImportByName->Hint; // 导出序号，可能为0
//                            printf("INT:%s", pImportByName->Name);
//                        }

//                        if (*pThunkData_IAT >> 31)         // 是序号
//                        {
//                            *pThunkData_IAT &= 0x7fffffff; // 把最高位去掉
//                            cout << "      IAT:ordinal:" << *pThunkData_IAT << endl;
//                        }
//                        else
//                        {
//                            DWORD import_by_name_FOA = 0;
//                            Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader,   *pThunkData_IAT, &import_by_name_FOA);
//                            PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)( (size_t)pFileBuffer + import_by_name_FOA);
//                            printf("      IAT:%s\n", pImportByName->Name);
//                        }
//                        pThunkData_INT++, pThunkData_IAT++;
//                    }
                pImportDescriptor += 1;
                i++;
            }
        }
    });


    connect(relocationTableView, &QTableView::doubleClicked, [this](const QModelIndex& current) {
        auto rowIndex = current.row();

        int i = 0;


        PIMAGE_BASE_RELOCATION pRelocationTable = pe->m_lpRelocationBase;

        while (1)
        {
            if (i == rowIndex) {
                int num_of_addr = (pRelocationTable->SizeOfBlock - 8) / 2;
                PWORD t_pAddr = NULL;
                t_pAddr = (PWORD)((size_t)pRelocationTable + 8);
                int removeAdd = 0;

                for (int j = 0; j < num_of_addr; j++)
                {
                    if (((t_pAddr[j] & 0x3000) == 0x3000) || ((t_pAddr[j] & 0x2000) == 0x2000)) // 判断高三位是否为0011
                    {
                        auto rva = (t_pAddr[j] & 0xfff) + pRelocationTable->VirtualAddress;
                        relocation2GridModel->setItem(j, 0, new QStandardItem(QString::number(rva, 16).toUpper()));
                        DWORD foa = 0;
                        pe->rvaToFoa(rva, &foa);
                        relocation2GridModel->setItem(j, 1, new QStandardItem(QString::number(foa, 16).toUpper()));
                    } else {
//                        cout << "the first 4 bits are not 0011!" << endl;
                        removeAdd++;
                        continue;
                    }
                }

                auto removeCount = relocation2GridModel->rowCount() -  (num_of_addr  + removeAdd);

                if (removeCount > 0) {
                    relocation2GridModel->removeRows(num_of_addr - removeAdd, removeCount + removeAdd);
                }

                break;
            } else {
                pRelocationTable = (PIMAGE_BASE_RELOCATION)((size_t)pRelocationTable + pRelocationTable->SizeOfBlock);
                i++;
            }
        }
    });


    connect(importTableView, &QTableView::doubleClicked, [this](const QModelIndex& current) {
        auto rowIndex = current.row();

        int i = 0;


        PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = pe->m_lpImportDescriptor;

        while (1)
        {
            if (i == rowIndex) {
                DWORD INTAddr_FOA = 0;
                DWORD IATAddr_FOA = 0;
                pe->rvaToFoa(pImportDescriptor->OriginalFirstThunk, &INTAddr_FOA);
                pe->rvaToFoa(pImportDescriptor->FirstThunk,         &IATAddr_FOA);
                int count = 0;

// 32位时
                if (pe->isX86()) {
                    PDWORD pThunkData_INT = NULL;
                    PDWORD pThunkData_IAT = NULL;

                    // PIMAGE_THUNK_DATA32 这里是导入查找表
                    pThunkData_INT = (PDWORD)((size_t)pe->m_lpFileData + INTAddr_FOA);
                    pThunkData_IAT = (PDWORD)((size_t)pe->m_lpFileData + IATAddr_FOA);


                    while (*pThunkData_INT && *pThunkData_IAT)
                    {
                        import2GridModel->setItem(count, 0, new QStandardItem(QString::number((size_t)INTAddr_FOA, 16).toUpper()));

                        if (*pThunkData_INT >> 31)         // 是序号
                        {
                            *pThunkData_INT &= 0x7fffffff; // 把最高位去掉
                            import2GridModel->setItem(i, 3, new QStandardItem(QString::number(*pThunkData_INT, 16).toUpper()));
                        }
                        else
                        {
                            DWORD import_by_name_FOA = 0;
                            pe->rvaToFoa(*pThunkData_INT, &import_by_name_FOA);
                            PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((size_t)pe->m_lpFileData + import_by_name_FOA);

                            import2GridModel->setItem(count, 1, new QStandardItem(QString::number(*pThunkData_INT, 16).toUpper()));
                            import2GridModel->setItem(count, 2, new QStandardItem(QString::number(import_by_name_FOA, 16).toUpper()));
                            import2GridModel->setItem(count, 3, new QStandardItem(tr(pImportByName->Name)));
                            import2GridModel->setItem(count, 4, new QStandardItem(QString::number(pImportByName->Hint, 16).toUpper()));
                        }

                        pThunkData_INT++, pThunkData_IAT++;
                        INTAddr_FOA += 4;
                        count++;
                    }
                } else {
                    ULONGLONG *pThunkData_INT = NULL;
                    ULONGLONG *pThunkData_IAT = NULL;

// PIMAGE_THUNK_DATA64 这里是导入查找表
                    pThunkData_INT = (ULONGLONG *)((size_t)pe->m_lpFileData + INTAddr_FOA);
                    pThunkData_IAT = (ULONGLONG *)((size_t)pe->m_lpFileData + IATAddr_FOA);


                    while (*pThunkData_INT && *pThunkData_IAT)
                    {
                        import2GridModel->setItem(count, 0, new QStandardItem(QString::number((size_t)INTAddr_FOA, 16).toUpper()));

                        if (*pThunkData_INT >> 63)                 // 是序号
                        {
                            *pThunkData_INT &= 0x7fffffffffffffff; // 把最高位去掉
                            import2GridModel->setItem(i, 3, new QStandardItem(QString::number(*pThunkData_INT, 16).toUpper()));
                        }
                        else
                        {
                            DWORD import_by_name_FOA = 0;
                            pe->rvaToFoa(*pThunkData_INT, &import_by_name_FOA);
                            PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((size_t)pe->m_lpFileData + import_by_name_FOA);

                            import2GridModel->setItem(count, 1, new QStandardItem(QString::number(*pThunkData_INT, 16).toUpper()));
                            import2GridModel->setItem(count, 2, new QStandardItem(QString::number(import_by_name_FOA, 16).toUpper()));
                            import2GridModel->setItem(count, 3, new QStandardItem(tr(pImportByName->Name)));
                            import2GridModel->setItem(count, 4, new QStandardItem(QString::number(pImportByName->Hint, 16).toUpper()));
                        }

                        pThunkData_INT++, pThunkData_IAT++;
                        INTAddr_FOA += 8;
                        count++;
                    }
                }


                auto removeCount = import2GridModel->rowCount() -  count;

                if (removeCount > 0) {
                    import2GridModel->removeRows(count, removeCount);
                }

                break;
            } else {
                pImportDescriptor += 1;
                i++;
            }
        }
    });
}

bool ItemView10PE::RVA_TO_FOA(PIMAGE_NT_HEADERS32 pNTHeader32,
                              PIMAGE_NT_HEADERS64 pNTHeader64,
                              PIMAGE_SECTION_HEADER pSectionHeader, IN DWORD RVA,
                              OUT PDWORD FOA) {
    if (pNTHeader32) {
        return Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, RVA, FOA);
    } else {
        return Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader, RVA, FOA);
    }
}

bool ItemView10PE::FOA_TO_RVA(PIMAGE_NT_HEADERS32 pNTHeader32,
                              PIMAGE_NT_HEADERS64 pNTHeader64,
                              PIMAGE_SECTION_HEADER pSectionHeader, IN DWORD FOA,
                              OUT PDWORD RVA) {
    if (pNTHeader32) {
        return Utils::FOA_TO_RVA(pNTHeader32, pSectionHeader, FOA, RVA);
    } else {
        return Utils::FOA_TO_RVA_64(pNTHeader64, pSectionHeader, FOA, RVA);
    }
}

// 创建新的节

// 1. 算出新节表位置 = 第一个节表(指针)(pFirstSectionHeader) + 节数量 (pNTHeader->FileHeader.NumberOfSections)
// 2. 计算节表空间够不够,一般都够。
// pNTHeader->OptionalHeader.SizeOfHeaders  (总空间)
// - (pDosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS32) + pNTHeader->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER)) (已使用的空间)
// < sizeof(IMAGE_SECTION_HEADER) * 2 (两个节表空间)
// 3.在添加的节表后面填充0 (memset(pNewSectionHeader + 1, 0, sizeof(IMAGE_SECTION_HEADER)); )
// 4.节表数量+1 (pNTHeader->FileHeader.NumberOfSections += 1)
// 5.SizeOfImage + 2000? pNTHeader->OptionalHeader.SizeOfImage += size_of_new_section
// 6.拷贝节表结构
// 7.填节表名称
// 8.节表VirtualSize给了2000  pNewSectionHeader->Misc.VirtualSize
// 9.节表SizeOfRawData给了2000  pNewSectionHeader->SizeOfRawData
// 10.根据倒数第2个节表算出PointerToRawData （t_LastSectionHeader->PointerToRawData + t_LastSectionHeader->SizeOfRawData;）
// 这里有问题，看下面注释 11.根据倒数第2个节表算出VirtualAddress （t_LastSectionHeader->VirtualAddress + t_LastSectionHeader->SizeOfRawData;）


bool ItemView10PE::CreateNewSection(PE    *pe,
                                    size_t size_of_new_section,
                                    LPSTR  NameOfNewSetionHeader)
{
    // NameOfNewSetionHeader 这个注意文件对齐值
    // 先判断最后一个节表末尾是否有能存放两个节表的空闲空间
    PIMAGE_SECTION_HEADER pNewSectionHeader = pe->m_lpSecHeader + pe->m_lpNtHeader32->FileHeader.NumberOfSections;

    if (pe->m_lpNtHeader32->OptionalHeader.SizeOfHeaders - (pe->m_lpDosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS32) +  pe->m_lpNtHeader32->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER)) < sizeof(IMAGE_SECTION_HEADER) * 2)
    {
        QMessageBox::warning(this, "警告", "not enough space to add  section header!");
        return false;
    }


    void *newLpFileData = realloc(pe->m_lpFileData, pe->m_fileSize + size_of_new_section); // 在末尾增加需要增加的大小

    if (newLpFileData == NULL) {
        QMessageBox::warning(this, "警告", "扩容失败!");

        // 应该不用free
    } else {
        pe->m_lpFileData = newLpFileData;
        pe->m_fileSize = pe->m_fileSize + size_of_new_section;

        if (pe->build()) {
            pNewSectionHeader = pe->m_lpSecHeader + pe->m_lpNtHeader32->FileHeader.NumberOfSections;
        } else {
            QMessageBox::warning(this, "警告", "扩容构建失败!");
            return false;
        }
    }

    // memset((PBYTE)pFileBuffer + size_of_new_section, 0,
    // pNTHeader->OptionalHeader.FileAlignment);
    // 再增加一个文件对齐大小的0

    memset(pNewSectionHeader + 1, 0, sizeof(IMAGE_SECTION_HEADER));        // 在添加的节表后面填充0

    pe->m_lpNtHeader32->FileHeader.NumberOfSections += 1;
    pe->m_lpNtHeader32->OptionalHeader.SizeOfImage += size_of_new_section; // 修正PE头


    // 拷贝一个节表结构
    // 注意要有读写权限，一般第一个是.text都有读写权限
    memcpy(pNewSectionHeader, pe->m_lpSecHeader,     sizeof(IMAGE_SECTION_HEADER));

    // 拷贝节表名称
    memcpy(pNewSectionHeader, NameOfNewSetionHeader, sizeof(NameOfNewSetionHeader));

    // 修改节表地址相关
    pNewSectionHeader->Misc.VirtualSize = size_of_new_section;
    pNewSectionHeader->SizeOfRawData = size_of_new_section;

    PIMAGE_SECTION_HEADER t_LastSectionHeader = pNewSectionHeader - 1;
    pNewSectionHeader->PointerToRawData = t_LastSectionHeader->PointerToRawData + t_LastSectionHeader->SizeOfRawData;

    // 注意内存对齐，注意VirtualSize可能大于SizeOfRawData
    // 如果VirtualAddress算错会导致无法运行
    // pNewSectionHeader->VirtualAddress = t_LastSectionHeader->VirtualAddress + t_LastSectionHeader->SizeOfRawData; 这么写不对，除非文件对齐和内存对齐一致。
    int maxSize = t_LastSectionHeader->SizeOfRawData;

    if (t_LastSectionHeader->Misc.VirtualSize > maxSize) maxSize = t_LastSectionHeader->Misc.VirtualSize;
    int size = (maxSize / pe->m_lpNtHeader32->OptionalHeader.SectionAlignment + 1) * pe->m_lpNtHeader32->OptionalHeader.SectionAlignment;
    pNewSectionHeader->VirtualAddress = t_LastSectionHeader->VirtualAddress + size;
}

// 注意坑，必须包含 C000 0000 也就是必须有代码写入的权限，因为有IAT要初始化写入
void ItemView10PE::InjectImportTable() {
    DWORD oldFileSize = pe->m_fileSize;

    // 节表名称
    char t_NameOfNewSectionHeader[6] = { 'Y', 'U', 'C', 'H', 'U', 'A' };

    CreateNewSection(pe, 0x2000, t_NameOfNewSectionHeader);


    // 原导入表
    // m_lpImportDescriptor

    // 新导入表
    PIMAGE_IMPORT_DESCRIPTOR pNewImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((size_t)pe->m_lpFileData + oldFileSize + 0x10);
    memset((LPVOID)((size_t)pe->m_lpFileData + oldFileSize), 0, 0x1000);

    PIMAGE_IMPORT_DESCRIPTOR oldImport = pe->m_lpImportDescriptor;
    PIMAGE_IMPORT_DESCRIPTOR newImport = pNewImportDescriptor;

    while (oldImport->FirstThunk != 0 /*|| pImportDescriptor->OriginalFirstThunk != 0*/) // 导入表迁移
    {
        memcpy(newImport, oldImport, sizeof(IMAGE_IMPORT_DESCRIPTOR));
        oldImport++, newImport++;
    }
    memset(newImport, 0, sizeof(IMAGE_IMPORT_DESCRIPTOR) * 2);

    // INT 表 IAT 表
    PDWORD pNewINT = (PDWORD)((size_t)pe->m_lpFileData + oldFileSize + 0x200);
    PDWORD pNewIAT = (PDWORD)((size_t)pe->m_lpFileData + oldFileSize + 0x300);
    PIMAGE_IMPORT_BY_NAME pNewName = (PIMAGE_IMPORT_BY_NAME)((size_t)pe->m_lpFileData + oldFileSize + 0x400);

    DWORD t_rva;
    pe->foaToRva(oldFileSize + 0x200, &t_rva);
    newImport->OriginalFirstThunk = t_rva; // 修正导入表中INT指针
    pe->foaToRva(oldFileSize + 0x300, &t_rva);
    newImport->FirstThunk = t_rva;         // 修正导入表中IAT指针
    pNewName->Hint = 0;
    char funname[] = "Add2";

    // if (memcpy(pNewName->Name, funname, strlen(funname)))
    //	printf("memcpy error!\n");
    for (int i = 0; i < strlen(funname); i++) pNewName->Name[i] = funname[i];  // 修正Name表的函数名称

    // Name表的rva 迁移
    pe->foaToRva(oldFileSize + 0x400, &t_rva);
    *pNewIAT = t_rva; // 修正INT表指针
    *pNewINT = t_rva; // 修正IAT表指针
    *(pNewINT + 1) = 0;
    *(pNewIAT + 1) = 0;


    char dllname[] = "InjectTestDll.dll";

    // if(memcpy((LPVOID)((DWORD)pFileBuffer+ file_size + 0x500), dllname, strlen(dllname)))
    //	printf("memcpy error!\n");
    PCHAR t_pchar = (PCHAR)((size_t)pe->m_lpFileData + oldFileSize + 0x500);

    for (int i = 0; i < strlen(dllname); i++) t_pchar[i] = dllname[i];

    pe->foaToRva(oldFileSize + 0x500, &t_rva);
    newImport->Name = t_rva;                                                    // 修正导入表DLL名称

    pe->foaToRva(oldFileSize + 0x10,  &t_rva);
    pe->m_lpNtHeader32->OptionalHeader.DataDirectory[1].VirtualAddress = t_rva; // 修正导入表指针
    pe->m_lpNtHeader32->OptionalHeader.DataDirectory[1].Size += sizeof(IMAGE_IMPORT_DESCRIPTOR);

    newImport->TimeDateStamp = 0;

    // 写入文件
    FILE *fp = _tfopen(FilePath, TEXT("wb"));
    fwrite(pe->m_lpFileData, pe->m_fileSize, 1, fp);
    fclose(fp);

    // memset((LPVOID)((DWORD)pFileBuffer + file_size + 0x2000), 7, 0x2000);
}

void ItemView10PE::appendConsole(const QString& msg)
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

void ItemView10PE::writeConsole(const QString& msg)
{
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

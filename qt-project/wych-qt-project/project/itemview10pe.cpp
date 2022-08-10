#include "itemview10pe.h"

// #include "mainwindow.h"
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

    // 第一层
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);

    leftQWidget->setFixedWidth(300);

    // 第二层
    auto leftQWidgetGroupBox1 = new QGroupBox("DOS头", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout = new QGridLayout(leftQWidgetGroupBox1);
    leftQWidgetGroupBox1->setFixedHeight(100);


    auto leftQWidgetGroupBox2 = new QGroupBox("PE头", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout2 = new QGridLayout(leftQWidgetGroupBox2);
    leftQWidgetGroupBox2->setFixedHeight(130);


    auto leftQWidgetGroupBox3 = new QGroupBox("可选PE头", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox3);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout3 = new QGridLayout(leftQWidgetGroupBox3);
    leftQWidgetGroupBox3->setFixedHeight(400);


    // 第一层
    auto centerQWidget = new QWidget(this);
    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);


    // 第二层

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


    // 第一层
    auto rightQWidget = new QWidget(this);
    auto rightQWidgetLayout = new QVBoxLayout(rightQWidget);


    // 第二层
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
    tabTabWidget->addTab( tab, tr("导出表"));
    tabTabWidget->addTab(tab2, tr("导入表"));
    tabTabWidget->addTab(tab3, tr("重定位表"));
    rightQWidgetGroupBox1Layout->addWidget(tabTabWidget);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    // 第四层
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
    auto importTabTabWidgetGroupBox = new QGroupBox("DLL", tab2);
    importTabTabWidgetLayout->addWidget(importTabTabWidgetGroupBox);
    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto importTabTabWidgetGroupBoxLayout = new QGridLayout(importTabTabWidgetGroupBox);
    importTabTabWidgetGroupBox->setFixedHeight(250);
    importTableView = new QTableView(this);
    importGridModel = new QStandardItemModel();
    importGridModel->setHorizontalHeaderLabels({  "DLL名称",  "OriginFirsthunk(指向INT)", "Firsthunk(指向IAT)" });
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
    import2GridModel->setHorizontalHeaderLabels({  "RVA",  "FOA", "函数名称" });
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
    leftQWidgetGroup1Layout->addWidget(  lb_e_magic, 0, 0);
    leftQWidgetGroup1Layout->addWidget( lb_e_lfanew, 1, 0);
    leftQWidgetGroup1Layout->addWidget(    btnStart, 2, 0);
    leftQWidgetGroup1Layout->addWidget( dos_e_magic, 0, 1);
    leftQWidgetGroup1Layout->addWidget(dos_e_lfanew, 1, 1);


    auto lb_Machine = new QLabel("Machine:");
    auto lb_NumberOfSections = new QLabel("NumberOfSections:");
    auto lb_SizeOfOptionHeader = new QLabel("SizeOfOptionHeader:");
    lb_Machine->setPalette(pe);
    lb_NumberOfSections->setPalette(pe);
    lb_SizeOfOptionHeader->setPalette(pe);
    leftQWidgetGroup1Layout2->addWidget(           lb_Machine,                            0, 0);
    leftQWidgetGroup1Layout2->addWidget(  lb_NumberOfSections,                            1, 0);
    leftQWidgetGroup1Layout2->addWidget(                  new QLabel("TimeDateStamp:"),   2, 0);
    leftQWidgetGroup1Layout2->addWidget(lb_SizeOfOptionHeader,                            3, 0);
    leftQWidgetGroup1Layout2->addWidget(                  new QLabel("Characteristics:"), 4, 0);
    leftQWidgetGroup1Layout2->addWidget(           pe_Machine,                            0, 1);
    leftQWidgetGroup1Layout2->addWidget(  pe_NumberOfSections,                            1, 1);
    leftQWidgetGroup1Layout2->addWidget(     pe_TimeDateStamp,                            2, 1);
    leftQWidgetGroup1Layout2->addWidget(pe_SizeOfOptionHeader,                            3, 1);
    leftQWidgetGroup1Layout2->addWidget(   pe_Characteristics,                            4, 1);


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
    leftQWidgetGroup1Layout3->addWidget(                   lb_Magic,                                    0,  0); // 10B 32位，20B 64位文件
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("SizeOfCode:"),              1,  0); // 没用 FileAlignment 整数倍
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("SizeOfInitializedData:"),   2,  0); // 没用 FileAlignment 整数倍
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("SizeOfUninitializedData:"), 3,  0); // 没用 FileAlignment 整数倍
    leftQWidgetGroup1Layout3->addWidget( lb_AddressOfEntryPoint_RVA,                                    4,  0); // 入口
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("BaseOfCode:"),              5,  0); // 没用 代码开始的基址
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("BaseOfData:"),              6,  0); // 没用 数据开始的基址
    leftQWidgetGroup1Layout3->addWidget(               lb_ImageBase,                                    7,  0); // 内存镜像基址
    leftQWidgetGroup1Layout3->addWidget(        lb_SectionAlignment,                                    8,  0); // 内存对齐
    leftQWidgetGroup1Layout3->addWidget(           lb_FileAlignment,                                    9,  0); // 文件对齐
    leftQWidgetGroup1Layout3->addWidget(             lb_SizeOfImage,                                    10, 0); // 内存中整个PE文件的映射尺寸，SectionAlignment整数倍
    leftQWidgetGroup1Layout3->addWidget(           lb_SizeOfHeaders,                                    11, 0); // 所有头+节表按照文件对齐后的大小，按照FileAlignment对齐
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("CheckSum:"),                12, 0); // 有些有校验
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("SizeOfStackReserve:"),      13, 0); // 初始化保留的堆栈大小
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("SizeOfStackCommit:"),       14, 0); // 初始化实际提交的大小
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("SizeOfHeapReserve:"),       15, 0); // 初始化保留的堆大小
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("SizeOfHeapCommit:"),        16, 0); // 初始化实际提交的大小
    leftQWidgetGroup1Layout3->addWidget(                        new QLabel("NumberOfRvaAndSizes:"),     17, 0); //

    leftQWidgetGroup1Layout3->addWidget(                  ope_Magic,                                    0,  1);
    leftQWidgetGroup1Layout3->addWidget(             ope_SizeOfCode,                                    1,  1);
    leftQWidgetGroup1Layout3->addWidget(  ope_SizeOfInitializedData,                                    2,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_SizeOfUninitializedData,                                    3,  1);
    leftQWidgetGroup1Layout3->addWidget(ope_AddressOfEntryPoint_RVA,                                    4,  1);
    leftQWidgetGroup1Layout3->addWidget(             ope_BaseOfCode,                                    5,  1);
    leftQWidgetGroup1Layout3->addWidget(             ope_BaseOfData,                                    6,  1);
    leftQWidgetGroup1Layout3->addWidget(              ope_ImageBase,                                    7,  1);
    leftQWidgetGroup1Layout3->addWidget(       ope_SectionAlignment,                                    8,  1);
    leftQWidgetGroup1Layout3->addWidget(          ope_FileAlignment,                                    9,  1);
    leftQWidgetGroup1Layout3->addWidget(            ope_SizeOfImage,                                    10, 1);
    leftQWidgetGroup1Layout3->addWidget(          ope_SizeOfHeaders,                                    11, 1);
    leftQWidgetGroup1Layout3->addWidget(               ope_CheckSum,                                    12, 1);
    leftQWidgetGroup1Layout3->addWidget(     ope_SizeOfStackReserve,                                    13, 1);
    leftQWidgetGroup1Layout3->addWidget(      ope_SizeOfStackCommit,                                    14, 1);
    leftQWidgetGroup1Layout3->addWidget(      ope_SizeOfHeapReserve,                                    15, 1);
    leftQWidgetGroup1Layout3->addWidget(       ope_SizeOfHeapCommit,                                    16, 1);
    leftQWidgetGroup1Layout3->addWidget(    ope_NumberOfRvaAndSizes,                                    17, 1);

    centerQWidgetGroupBox2Layout->addWidget(                    new QLabel("导出表:"),  1, 0);
    centerQWidgetGroupBox2Layout->addWidget(                    new QLabel("导入表:"),  2, 0);
    centerQWidgetGroupBox2Layout->addWidget(                    new QLabel("资源表:"),  3, 0);
    centerQWidgetGroupBox2Layout->addWidget(                    new QLabel("重定位表:"), 4, 0);
    centerQWidgetGroupBox2Layout->addWidget(                    new QLabel("大小:"),   0, 1);
    centerQWidgetGroupBox2Layout->addWidget(                    new QLabel("RVA:"),  0, 2);
    centerQWidgetGroupBox2Layout->addWidget(                    new QLabel("FOA:"),  0, 3);
    centerQWidgetGroupBox2Layout->addWidget(         tb_export_size,                 1, 1);
    centerQWidgetGroupBox2Layout->addWidget(         tb_import_size,                 2, 1);
    centerQWidgetGroupBox2Layout->addWidget(       tb_resource_size,                 3, 1);
    centerQWidgetGroupBox2Layout->addWidget(tb_base_relocation_size,                 4, 1);
    centerQWidgetGroupBox2Layout->addWidget(          tb_export_rva,                 1, 2);
    centerQWidgetGroupBox2Layout->addWidget(          tb_import_rva,                 2, 2);
    centerQWidgetGroupBox2Layout->addWidget(        tb_resource_rva,                 3, 2);
    centerQWidgetGroupBox2Layout->addWidget( tb_base_relocation_rva,                 4, 2);
    centerQWidgetGroupBox2Layout->addWidget(          tb_export_foa,                 1, 3);
    centerQWidgetGroupBox2Layout->addWidget(          tb_import_foa,                 2, 3);
    centerQWidgetGroupBox2Layout->addWidget(        tb_resource_foa,                 3, 3);
    centerQWidgetGroupBox2Layout->addWidget( tb_base_relocation_foa,                 4, 3);

    centerQWidgetGroupBox1Layout->addWidget(ckConsoleEnable);
    centerQWidgetGroupBox1Layout->addWidget(btnConsoleClear);

    //    rightQWidgetGroupBox1Layout->addWidget(ckRefreshClients);
    //    rightQWidgetGroupBox1Layout->addWidget(      btnClients);
    //    rightQWidgetLayout->addWidget(infoTableView);

    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("名称:"),                    1, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("Base:"),                  2, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("NumberOfFunctions:"),     3, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("NumberOfNames:"),         4, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("AddressOfFunctions:"),    5, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("AddressOfNameOrdinals:"), 6, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("AddressOfNames:"),        7, 0);
    exportTabTabWidgetGroupBox1Layout->addWidget(                     export_Name,                                  1, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(                     export_Base,                                  2, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(        export_NumberOfFunctions,                                  3, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(            export_NumberOfNames,                                  4, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(       export_AddressOfFunctions,                                  5, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(           export_AddressOfNames,                                  6, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(    export_AddressOfNameOrdinals,                                  7, 1);
    exportTabTabWidgetGroupBox1Layout->addWidget(   export_AddressOfFunctions_foa,                                  5, 3);
    exportTabTabWidgetGroupBox1Layout->addWidget(export_AddressOfNameOrdinals_foa,                                  6, 3);
    exportTabTabWidgetGroupBox1Layout->addWidget(       export_AddressOfNames_foa,                                  7, 3);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("FOA:"),                   5, 2);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("FOA:"),                   6, 2);
    exportTabTabWidgetGroupBox1Layout->addWidget(                             new QLabel("FOA:"),                   7, 2);


    layout->addWidget(  leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget( rightQWidget);
}

void ItemView10PE::initConnect()
{
    connect(btnStart, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("文件对话框！"), "F:", tr("动态链接库(*dll *jpg);;" "执行文件(*exe)"));
        auto path = (wchar_t *)fileName.utf16();

        // 转char*
        //        auto str = fileName.toStdString();
        //        auto path = str.c_str();
        if (pFileBuffer) {
            free(pFileBuffer);
        }

        auto size = Utils::ReadFile(path, &pFileBuffer);

        if ((size == NULL) || (pFileBuffer == NULL)) {
            return;
        }

        // 重定位表指针
        pRelocationTableBase = NULL;


        // 获取DosHeader
        if (*((PWORD)pFileBuffer) != IMAGE_DOS_SIGNATURE) {
            QMessageBox::warning(this, "警告", "the first word is not MZ!");
            return;
        }
        PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;

        dos_e_magic->setText(QString::number(pDosHeader->e_magic, 16));
        dos_e_lfanew->setText(QString::number(pDosHeader->e_lfanew, 16));


        pSectionHeader = NULL;
        pNTHeader32 = NULL;
        pNTHeader64 = NULL;

        // 判断32位
        if (*(PWORD)((size_t)pFileBuffer + pDosHeader->e_lfanew + 0x14) == 0x00E0) {
            // 获取NTHeader
            pNTHeader32 = (PIMAGE_NT_HEADERS32)((size_t)pFileBuffer + pDosHeader->e_lfanew);

            if (pNTHeader32->Signature != IMAGE_NT_SIGNATURE) {
                QMessageBox::warning(this, "警告", "nt header error!signature not match!");
                return;
            }
            pe_Machine->setText(QString::number(pNTHeader32->FileHeader.Machine, 16).toUpper());                         // 程序运行的CPU型号
            pe_NumberOfSections->setText(QString::number(pNTHeader32->FileHeader.NumberOfSections, 16).toUpper());       // 节的数量
            pe_TimeDateStamp->setText(QString::number(pNTHeader32->FileHeader.TimeDateStamp, 16).toUpper());             // 时间戳，编译器填写
            pe_SizeOfOptionHeader->setText(QString::number(pNTHeader32->FileHeader.SizeOfOptionalHeader, 16).toUpper()); // 可选PE头的大小 32位文件默认E0h，64位PE文件默认为F0h，可自定义
            pe_Characteristics->setText(QString::number(pNTHeader32->FileHeader.Characteristics, 16).toUpper());


            ope_Magic->setText(QString::number(pNTHeader32->OptionalHeader.Magic, 16).toUpper());
            ope_SizeOfCode->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfCode, 16).toUpper());
            ope_SizeOfInitializedData->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfInitializedData, 16).toUpper());
            ope_SizeOfUninitializedData->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfUninitializedData, 16).toUpper());
            ope_AddressOfEntryPoint_RVA->setText(QString::number(pNTHeader32->OptionalHeader.AddressOfEntryPoint, 16).toUpper());
            ope_BaseOfCode->setText(QString::number(pNTHeader32->OptionalHeader.BaseOfCode, 16).toUpper());
            ope_BaseOfData->setText(QString::number(pNTHeader32->OptionalHeader.BaseOfData, 16).toUpper());
            ope_ImageBase->setText(QString::number(pNTHeader32->OptionalHeader.ImageBase, 16).toUpper());
            ope_SectionAlignment->setText(QString::number(pNTHeader32->OptionalHeader.SectionAlignment, 16).toUpper());
            ope_FileAlignment->setText(QString::number(pNTHeader32->OptionalHeader.FileAlignment, 16).toUpper());
            ope_SizeOfImage->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfImage, 16).toUpper());
            ope_SizeOfHeaders->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfHeaders, 16).toUpper());
            ope_CheckSum->setText(QString::number(pNTHeader32->OptionalHeader.CheckSum, 16).toUpper());
            ope_SizeOfStackReserve->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfStackReserve, 16).toUpper());
            ope_SizeOfStackCommit->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfStackCommit, 16).toUpper());
            ope_SizeOfHeapReserve->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfHeapReserve, 16).toUpper());
            ope_SizeOfHeapCommit->setText(QString::number(pNTHeader32->OptionalHeader.SizeOfHeapCommit, 16).toUpper());
            ope_NumberOfRvaAndSizes->setText(QString::number(pNTHeader32->OptionalHeader.NumberOfRvaAndSizes, 16).toUpper());


            pSectionHeader = (PIMAGE_SECTION_HEADER)(
                (size_t)pNTHeader32 + sizeof(IMAGE_NT_SIGNATURE) +
                sizeof(IMAGE_FILE_HEADER) + pNTHeader32->FileHeader.SizeOfOptionalHeader);

            PIMAGE_DATA_DIRECTORY ExportTable =  &(pNTHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
            PIMAGE_DATA_DIRECTORY ImportTable =  &(pNTHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
            PIMAGE_DATA_DIRECTORY ResourceTable =  &(pNTHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]);
            PIMAGE_DATA_DIRECTORY RelocationTable =  &(pNTHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);

            tb_export_size->setText(QString::number(ExportTable->Size, 16).toUpper());
            tb_import_size->setText(QString::number(ImportTable->Size, 16).toUpper());
            tb_resource_size->setText(QString::number(ResourceTable->Size, 16).toUpper());
            tb_base_relocation_size->setText(QString::number(RelocationTable->Size, 16).toUpper());
            tb_export_rva->setText(QString::number(ExportTable->VirtualAddress, 16).toUpper());
            tb_import_rva->setText(QString::number(ImportTable->VirtualAddress, 16).toUpper());
            tb_resource_rva->setText(QString::number(ResourceTable->VirtualAddress, 16).toUpper());
            tb_base_relocation_rva->setText(QString::number(RelocationTable->VirtualAddress, 16).toUpper());

            DWORD ExportTableFoa = 0;
            DWORD ImportTableFoa = 0;
            DWORD ResourceTableFoa = 0;
            DWORD RelocationTableFoa = 0;

            if (ExportTable->VirtualAddress) {
                Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, ExportTable->VirtualAddress, &ExportTableFoa);
            }

            if (ImportTable->VirtualAddress) {
                Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, ImportTable->VirtualAddress, &ImportTableFoa);
            }

            if (ResourceTable->VirtualAddress) {
                Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, ResourceTable->VirtualAddress, &ResourceTableFoa);
            }

            if (RelocationTable->VirtualAddress) {
                Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, RelocationTable->VirtualAddress, &RelocationTableFoa);
            }
            tb_export_foa->setText(QString::number(ExportTableFoa, 16).toUpper());
            tb_import_foa->setText(QString::number(ImportTableFoa, 16).toUpper());
            tb_resource_foa->setText(QString::number(ResourceTableFoa, 16).toUpper());
            tb_base_relocation_foa->setText(QString::number(RelocationTableFoa, 16).toUpper());

            // 节表
            for (int i = 0; i < pNTHeader32->FileHeader.NumberOfSections; i++)
            {
                tableGridModel->setItem(i, 0, new QStandardItem(tr((char *)(pSectionHeader[i].Name))));
                tableGridModel->setItem(i, 1, new QStandardItem(QString::number(pSectionHeader[i].Misc.VirtualSize, 16).toUpper()));
                tableGridModel->setItem(i, 2, new QStandardItem(QString::number(pSectionHeader[i].SizeOfRawData, 16).toUpper()));
                tableGridModel->setItem(i, 3, new QStandardItem(QString::number(pSectionHeader[i].VirtualAddress, 16).toUpper()));
                tableGridModel->setItem(i, 4, new QStandardItem(QString::number(pSectionHeader[i].PointerToRawData, 16).toUpper()));
                tableGridModel->setItem(i, 5, new QStandardItem(QString::number(pSectionHeader[i].Characteristics, 16).toUpper()));
            }

            auto removeCount = tableGridModel->rowCount() - pNTHeader32->FileHeader.NumberOfSections;

            if (removeCount > 0) {
                tableGridModel->removeRows(pNTHeader32->FileHeader.NumberOfSections, removeCount);
            }

            // 导出表
            if (ExportTableFoa) {
                PIMAGE_EXPORT_DIRECTORY pExportDirectory = NULL;
                pExportDirectory =
                    (PIMAGE_EXPORT_DIRECTORY)((size_t)pFileBuffer + ExportTableFoa);


                export_Name->setText(QString::number(pExportDirectory->Name, 16).toUpper());
                export_Base->setText(QString::number(pExportDirectory->Base, 16).toUpper());
                export_NumberOfFunctions->setText(QString::number(pExportDirectory->NumberOfFunctions, 16).toUpper());
                export_NumberOfNames->setText(QString::number(pExportDirectory->NumberOfNames, 16).toUpper());
                export_AddressOfFunctions->setText(QString::number(pExportDirectory->AddressOfFunctions, 16).toUpper());
                export_AddressOfNames->setText(QString::number(pExportDirectory->AddressOfNames, 16).toUpper());
                export_AddressOfNameOrdinals->setText(QString::number(pExportDirectory->AddressOfNameOrdinals, 16).toUpper());


                DWORD FunctionTableAddr = NULL;
                DWORD NameTableAddr = NULL;
                DWORD OrdinalTableAddr = NULL;

                Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader,
                                  pExportDirectory->AddressOfFunctions, &FunctionTableAddr);
                Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader,
                                  pExportDirectory->AddressOfNames, &NameTableAddr);
                Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader,
                                  pExportDirectory->AddressOfNameOrdinals, &OrdinalTableAddr);


                export_AddressOfFunctions_foa->setText(QString::number(FunctionTableAddr, 16).toUpper());
                export_AddressOfNames_foa->setText(QString::number(NameTableAddr, 16).toUpper());
                export_AddressOfNameOrdinals_foa->setText(QString::number(OrdinalTableAddr, 16).toUpper());

                PDWORD arr_fun = NULL;
                PDWORD arr_name = NULL;
                PWORD arr_ord = NULL;
                arr_fun = (PDWORD)((size_t)pFileBuffer + FunctionTableAddr);    // 得到指向AddressOfFunction的指针
                arr_name = (PDWORD)((size_t)pFileBuffer + NameTableAddr);       // 得到指向AddressOfNames的指针
                arr_ord = (PWORD)((size_t)pFileBuffer + OrdinalTableAddr);      // 得到指向AddressOfNameOrdinal的指针


                for (DWORD i = 0; i < pExportDirectory->NumberOfFunctions; i++) // 遍历函数表
                {
                    exportGridModel->setItem(i, 0, new QStandardItem(QString::number(i)));

                    exportGridModel->setItem(i, 1, new QStandardItem(QString::number(arr_fun[i], 16).toUpper()));
                    DWORD foa = 0;
                    Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, arr_fun[i], &foa);
                    exportGridModel->setItem(i, 2, new QStandardItem(QString::number(foa, 16).toUpper()));
                }

                auto removeCount = exportGridModel->rowCount() -  pExportDirectory->NumberOfFunctions;

                if (removeCount > 0) {
                    exportGridModel->removeRows(pExportDirectory->NumberOfFunctions, removeCount);
                }


                for (DWORD i = 0; i < pExportDirectory->NumberOfNames; i++) // 遍历name
                {
                    DWORD  name_rva = *(arr_name + i);

                    if (name_rva) {
                        DWORD  name_foa = 0;
                        Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, name_rva, &name_foa);

                        if (name_foa) {
                            auto ordinal = *(arr_ord + i);
                            exportGridModel->setItem(ordinal, 3, new QStandardItem(QString::number(ordinal, 16).toUpper()));
                            exportGridModel->setItem(ordinal, 4, new QStandardItem(tr((char *)((size_t)pFileBuffer + name_foa))));
                        }
                    }
                }
            }

            // 重定位表
            if (RelocationTableFoa) {
                pRelocationTableBase = (PIMAGE_BASE_RELOCATION)((size_t)pFileBuffer + RelocationTableFoa);
                PIMAGE_BASE_RELOCATION pRelocationTable = pRelocationTableBase;
                int i = 0;

                while (1)
                {
                    if ((pRelocationTable->SizeOfBlock == 0) && (pRelocationTable->VirtualAddress == 0)) {
                        break;
                    }
                    int num_of_addr = (pRelocationTable->SizeOfBlock - 8) / 2;
                    relocationGridModel->setItem(i, 1, new QStandardItem(QString::number(pRelocationTable->VirtualAddress, 16).toUpper()));
                    relocationGridModel->setItem(i, 2, new QStandardItem(QString::number(num_of_addr).toUpper()));
                    relocationGridModel->setItem(i, 3, new QStandardItem(QString::number(pRelocationTable->SizeOfBlock).toUpper()));

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

                    pRelocationTable = (PIMAGE_BASE_RELOCATION)((size_t)pRelocationTable + pRelocationTable->SizeOfBlock);

                    i++;
                }
            }
        } else {
            // 获取NTHeader
            pNTHeader64 = (PIMAGE_NT_HEADERS64)((size_t)pFileBuffer + pDosHeader->e_lfanew);

            if (pNTHeader64->Signature != IMAGE_NT_SIGNATURE) {
                QMessageBox::warning(this, "警告", "nt header error!signature not match!");
                return;
            }
            pe_Machine->setText(QString::number(pNTHeader64->FileHeader.Machine, 16).toUpper());                         // 程序运行的CPU型号
            pe_NumberOfSections->setText(QString::number(pNTHeader64->FileHeader.NumberOfSections, 16).toUpper());       // 节的数量
            pe_TimeDateStamp->setText(QString::number(pNTHeader64->FileHeader.TimeDateStamp, 16).toUpper());             // 时间戳，编译器填写
            pe_SizeOfOptionHeader->setText(QString::number(pNTHeader64->FileHeader.SizeOfOptionalHeader, 16).toUpper()); // 可选PE头的大小 32位文件默认E0h，64位PE文件默认为F0h，可自定义
            pe_Characteristics->setText(QString::number(pNTHeader64->FileHeader.Characteristics, 16).toUpper());


            ope_Magic->setText(QString::number(pNTHeader64->OptionalHeader.Magic, 16).toUpper());
            ope_SizeOfCode->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfCode, 16).toUpper());
            ope_SizeOfInitializedData->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfInitializedData, 16).toUpper());
            ope_SizeOfUninitializedData->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfUninitializedData, 16).toUpper());
            ope_AddressOfEntryPoint_RVA->setText(QString::number(pNTHeader64->OptionalHeader.AddressOfEntryPoint, 16).toUpper());
            ope_BaseOfCode->setText(QString::number(pNTHeader64->OptionalHeader.BaseOfCode, 16).toUpper());

            //            ope_BaseOfData->setText(QString::number(pNTHeader64->OptionalHeader.BaseOfData, 16).toUpper());
            ope_ImageBase->setText(QString::number(pNTHeader64->OptionalHeader.ImageBase, 16).toUpper());
            ope_SectionAlignment->setText(QString::number(pNTHeader64->OptionalHeader.SectionAlignment, 16).toUpper());
            ope_FileAlignment->setText(QString::number(pNTHeader64->OptionalHeader.FileAlignment, 16).toUpper());
            ope_SizeOfImage->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfImage, 16).toUpper());
            ope_SizeOfHeaders->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfHeaders, 16).toUpper());
            ope_CheckSum->setText(QString::number(pNTHeader64->OptionalHeader.CheckSum, 16).toUpper());
            ope_SizeOfStackReserve->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfStackReserve, 16).toUpper());
            ope_SizeOfStackCommit->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfStackCommit, 16).toUpper());
            ope_SizeOfHeapReserve->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfHeapReserve, 16).toUpper());
            ope_SizeOfHeapCommit->setText(QString::number(pNTHeader64->OptionalHeader.SizeOfHeapCommit, 16).toUpper());
            ope_NumberOfRvaAndSizes->setText(QString::number(pNTHeader64->OptionalHeader.NumberOfRvaAndSizes, 16).toUpper());


            pSectionHeader = (PIMAGE_SECTION_HEADER)(
                (size_t)pNTHeader64 + sizeof(IMAGE_NT_SIGNATURE) +
                sizeof(IMAGE_FILE_HEADER) + pNTHeader64->FileHeader.SizeOfOptionalHeader);


            PIMAGE_DATA_DIRECTORY ExportTable =  &(pNTHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
            PIMAGE_DATA_DIRECTORY ImportTable =  &(pNTHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
            PIMAGE_DATA_DIRECTORY ResourceTable =  &(pNTHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]);
            PIMAGE_DATA_DIRECTORY RelocationTable =  &(pNTHeader64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);

            tb_export_size->setText(QString::number(ExportTable->Size, 16).toUpper());
            tb_import_size->setText(QString::number(ImportTable->Size, 16).toUpper());
            tb_resource_size->setText(QString::number(ResourceTable->Size, 16).toUpper());
            tb_base_relocation_size->setText(QString::number(RelocationTable->Size, 16).toUpper());
            tb_export_rva->setText(QString::number(ExportTable->VirtualAddress, 16).toUpper());
            tb_import_rva->setText(QString::number(ImportTable->VirtualAddress, 16).toUpper());
            tb_resource_rva->setText(QString::number(ResourceTable->VirtualAddress, 16).toUpper());
            tb_base_relocation_rva->setText(QString::number(RelocationTable->VirtualAddress, 16).toUpper());

            DWORD ExportTableFoa = 0;
            DWORD ImportTableFoa = 0;
            DWORD ResourceTableFoa = 0;
            DWORD RelocationTableFoa = 0;

            if (ExportTable->VirtualAddress) {
                Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader, ExportTable->VirtualAddress, &ExportTableFoa);
            }

            if (ImportTable->VirtualAddress) {
                Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader, ImportTable->VirtualAddress, &ImportTableFoa);
            }

            if (ResourceTable->VirtualAddress) {
                Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader, ResourceTable->VirtualAddress, &ResourceTableFoa);
            }

            if (RelocationTable->VirtualAddress) {
                Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader, RelocationTable->VirtualAddress, &RelocationTableFoa);
            }
            tb_export_foa->setText(QString::number(ExportTableFoa, 16).toUpper());
            tb_import_foa->setText(QString::number(ImportTableFoa, 16).toUpper());
            tb_resource_foa->setText(QString::number(ResourceTableFoa, 16).toUpper());
            tb_base_relocation_foa->setText(QString::number(RelocationTableFoa, 16).toUpper());

            // 节表
            for (int i = 0; i < pNTHeader64->FileHeader.NumberOfSections; i++)
            {
                tableGridModel->setItem(i, 0, new QStandardItem(tr((char *)(pSectionHeader[i].Name))));
                tableGridModel->setItem(i, 1, new QStandardItem(QString::number(pSectionHeader[i].Misc.VirtualSize, 16).toUpper()));
                tableGridModel->setItem(i, 2, new QStandardItem(QString::number(pSectionHeader[i].SizeOfRawData, 16).toUpper()));
                tableGridModel->setItem(i, 3, new QStandardItem(QString::number(pSectionHeader[i].VirtualAddress, 16).toUpper()));
                tableGridModel->setItem(i, 4, new QStandardItem(QString::number(pSectionHeader[i].PointerToRawData, 16).toUpper()));
                tableGridModel->setItem(i, 5, new QStandardItem(QString::number(pSectionHeader[i].Characteristics, 16).toUpper()));
            }

            auto removeCount = tableGridModel->rowCount() - pNTHeader64->FileHeader.NumberOfSections;

            if (removeCount > 0) {
                tableGridModel->removeRows(pNTHeader64->FileHeader.NumberOfSections, removeCount);
            }

            // 导出表
            if (ExportTableFoa) {
                PIMAGE_EXPORT_DIRECTORY pExportDirectory = NULL;
                pExportDirectory =
                    (PIMAGE_EXPORT_DIRECTORY)((size_t)pFileBuffer + ExportTableFoa);


                export_Name->setText(QString::number(pExportDirectory->Name, 16).toUpper());
                export_Base->setText(QString::number(pExportDirectory->Base, 16).toUpper());
                export_NumberOfFunctions->setText(QString::number(pExportDirectory->NumberOfFunctions, 16).toUpper());
                export_NumberOfNames->setText(QString::number(pExportDirectory->NumberOfNames, 16).toUpper());
                export_AddressOfFunctions->setText(QString::number(pExportDirectory->AddressOfFunctions, 16).toUpper());
                export_AddressOfNames->setText(QString::number(pExportDirectory->AddressOfNames, 16).toUpper());
                export_AddressOfNameOrdinals->setText(QString::number(pExportDirectory->AddressOfNameOrdinals, 16).toUpper());


                DWORD FunctionTableAddr = NULL;
                DWORD NameTableAddr = NULL;
                DWORD OrdinalTableAddr = NULL;

                Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader,
                                     pExportDirectory->AddressOfFunctions, &FunctionTableAddr);
                Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader,
                                     pExportDirectory->AddressOfNames, &NameTableAddr);
                Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader,
                                     pExportDirectory->AddressOfNameOrdinals, &OrdinalTableAddr);


                export_AddressOfFunctions_foa->setText(QString::number(FunctionTableAddr, 16).toUpper());
                export_AddressOfNames_foa->setText(QString::number(NameTableAddr, 16).toUpper());
                export_AddressOfNameOrdinals_foa->setText(QString::number(OrdinalTableAddr, 16).toUpper());

                PDWORD arr_fun = NULL;
                PDWORD arr_name = NULL;
                PWORD arr_ord = NULL;
                arr_fun = (PDWORD)((size_t)pFileBuffer + FunctionTableAddr);    // 得到指向AddressOfFunction的指针
                arr_name = (PDWORD)((size_t)pFileBuffer + NameTableAddr);       // 得到指向AddressOfNames的指针
                arr_ord = (PWORD)((size_t)pFileBuffer + OrdinalTableAddr);      // 得到指向AddressOfNameOrdinal的指针


                for (DWORD i = 0; i < pExportDirectory->NumberOfFunctions; i++) // 遍历函数表
                {
                    exportGridModel->setItem(i, 0, new QStandardItem(QString::number(i)));

                    exportGridModel->setItem(i, 1, new QStandardItem(QString::number(arr_fun[i], 16).toUpper()));
                    DWORD foa = 0;
                    Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader, arr_fun[i], &foa);
                    exportGridModel->setItem(i, 2, new QStandardItem(QString::number(foa, 16).toUpper()));
                }

                auto removeCount = exportGridModel->rowCount() -  pExportDirectory->NumberOfFunctions;

                if (removeCount > 0) {
                    exportGridModel->removeRows(pExportDirectory->NumberOfFunctions, removeCount);
                }


                for (DWORD i = 0; i < pExportDirectory->NumberOfNames; i++) // 遍历name
                {
                    DWORD  name_rva = *(arr_name + i);

                    if (name_rva) {
                        DWORD  name_foa = 0;
                        Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader, name_rva, &name_foa);

                        if (name_foa) {
                            auto ordinal = *(arr_ord + i);
                            exportGridModel->setItem(ordinal, 3, new QStandardItem(QString::number(ordinal, 16).toUpper()));
                            exportGridModel->setItem(ordinal, 4, new QStandardItem(tr((char *)((size_t)pFileBuffer + name_foa))));
                        }
                    }
                }
            }

            // 重定位表
            if (RelocationTableFoa) {
                pRelocationTableBase = (PIMAGE_BASE_RELOCATION)((size_t)pFileBuffer + RelocationTableFoa);
                PIMAGE_BASE_RELOCATION pRelocationTable = pRelocationTableBase;
                int i = 0;

                while (1)
                {
                    if ((pRelocationTable->SizeOfBlock == 0) && (pRelocationTable->VirtualAddress == 0)) {
                        break;
                    }
                    int num_of_addr = (pRelocationTable->SizeOfBlock - 8) / 2;
                    relocationGridModel->setItem(i, 1, new QStandardItem(QString::number(pRelocationTable->VirtualAddress, 16).toUpper()));
                    relocationGridModel->setItem(i, 2, new QStandardItem(QString::number(num_of_addr).toUpper()));
                    relocationGridModel->setItem(i, 3, new QStandardItem(QString::number(pRelocationTable->SizeOfBlock).toUpper()));

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

                    pRelocationTable = (PIMAGE_BASE_RELOCATION)((size_t)pRelocationTable + pRelocationTable->SizeOfBlock);

                    i++;
                }
            }
        }
    });


    connect(relocationTableView, &QTableView::doubleClicked, [this](const QModelIndex& current) {
        auto rowIndex = current.row();

        int i = 0;


        PIMAGE_BASE_RELOCATION pRelocationTable = pRelocationTableBase;

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

                        if (pNTHeader32) {
                            Utils::RVA_TO_FOA(pNTHeader32, pSectionHeader, rva, &foa);
                        } else {
                            Utils::RVA_TO_FOA_64(pNTHeader64, pSectionHeader, rva, &foa);
                        }
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

    //        SLOT(leftTabFavorTableRowDoubleClicked(const QModelIndex&)));
    //    connect(relocationTableView,  SIGNAL(clicked(const QModelIndex&)),       this, SLOT(fileTableRowClicked(const QModelIndex&)));
}

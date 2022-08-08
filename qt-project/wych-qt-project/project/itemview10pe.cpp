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

    auto centerQWidgetGroupBox2 = new QGroupBox("节表", centerQWidget);
    auto centerQWidgetGroupBox2Layout = new QGridLayout(centerQWidgetGroupBox2);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox2);


    auto centerQWidgetGroupBox1 = new QGroupBox("控制台设置", centerQWidget);
    auto centerQWidgetGroupBox1Layout = new QHBoxLayout(centerQWidgetGroupBox1);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox1);

    edtMsg = new QPlainTextEdit();
    edtMsg->setReadOnly(true);
    centerQWidgetLayout->addWidget(edtMsg);


    auto rightQWidget = new QWidget(this);
    auto rightQWidgetLayout = new QVBoxLayout(rightQWidget);
    auto rightQWidgetGroupBox1 = new QGroupBox("客户端控制", rightQWidget);
    auto rightQWidgetGroupBox1Layout = new QHBoxLayout(rightQWidgetGroupBox1);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    // 控件
    ckConsoleEnable = new QCheckBox("启动控制台");
    ckRefreshClients = new QCheckBox("刷新客户端");
    btnStart = new QPushButton("选择文件");
    btnConsoleClear = new QPushButton("清空控制台");
    btnClients = new QPushButton("刷新客户端");


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


    infoTableView = new QTableView(this);
    infoGridModel = new QStandardItemModel();

    /* 设置表格标题行(输入数据为QStringList类型) */
    infoGridModel->setHorizontalHeaderLabels({  "类型", "ID", "地址", "状态" });
    infoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    infoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    infoTableView->setModel(infoGridModel);


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

    rightQWidgetGroupBox1Layout->addWidget(ckRefreshClients);
    rightQWidgetGroupBox1Layout->addWidget(      btnClients);
    rightQWidgetLayout->addWidget(infoTableView);


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
        LPVOID pFileBuffer = NULL;

        auto size = Utils::ReadFile(path, &pFileBuffer);

        if ((size == NULL) || (pFileBuffer == NULL)) {
            return;
        }


        // 获取DosHeader
        if (*((PWORD)pFileBuffer) != IMAGE_DOS_SIGNATURE) {
            QMessageBox::warning(this, "警告", "the first word is not MZ!");
            return;
        }
        PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;

        dos_e_magic->setText(QString::number(pDosHeader->e_magic, 16));
        dos_e_lfanew->setText(QString::number(pDosHeader->e_lfanew, 16));


        PIMAGE_SECTION_HEADER pSectionHeader = NULL;

        // 判断32位
        if (*(PWORD)((size_t)pFileBuffer + pDosHeader->e_lfanew + 0x14) == 0x00E0) {
            // 获取NTHeader
            PIMAGE_NT_HEADERS32 pNTHeader32 = (PIMAGE_NT_HEADERS32)((size_t)pFileBuffer + pDosHeader->e_lfanew);

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
        } else {
            // 获取NTHeader
            PIMAGE_NT_HEADERS64 pNTHeader64 = (PIMAGE_NT_HEADERS64)((size_t)pFileBuffer + pDosHeader->e_lfanew);

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
        }
    });
}

#include "Itemview10Inject.h"

#include "utils.h"

#include <QTextCodec>
#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>

Itemview10Inject::Itemview10Inject(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnect();
}

Itemview10Inject::~Itemview10Inject()
{}

void Itemview10Inject::initUI()
{
    auto layout = new QHBoxLayout(this);

    // 第一层
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);

    //    leftQWidget->setFixedWidth(300);

    // 第二层
    auto leftQWidgetGroupBox1 = new QGroupBox("按钮", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout = new QGridLayout(leftQWidgetGroupBox1);
    leftQWidgetGroupBox1->setFixedHeight(100);


    auto leftQWidgetGroupBox2 = new QGroupBox("进程", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout2 = new QGridLayout(leftQWidgetGroupBox2);

    //    leftQWidgetGroupBox2->setFixedHeight(130);

    processTableView = new QTableView(this);
    processGridModel = new QStandardItemModel();
    processGridModel->setHorizontalHeaderLabels({  "进程", "PID",  "镜像基址", "镜像大小" });
    processTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    processTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    processTableView->setModel(processGridModel);
    leftQWidgetGroup1Layout2->addWidget(processTableView);


    //    auto leftQWidgetGroupBox3 = new QGroupBox("可选PE头", this);
    //    leftQWidgetLayout->addWidget(leftQWidgetGroupBox3);
    //    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    //    auto leftQWidgetGroup1Layout3 = new QGridLayout(leftQWidgetGroupBox3);
    //    leftQWidgetGroupBox3->setFixedHeight(400);


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

    //    tableTableView = new QTableView(this);
    //    tableGridModel = new QStandardItemModel();
    //    tableGridModel->setHorizontalHeaderLabels({  "Name", "VirtualSize",  "SizeOfRawData", "VirtualAddress", "PointerToRawData", "Characteristics" });
    //    tableTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    tableTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    tableTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    tableTableView->setModel(tableGridModel);
    //    centerQWidgetGroupBox3Layout->addWidget(tableTableView);

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
    importTabTabWidgetGroupBox3->setFixedHeight(60);
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
    btnStart = new QPushButton("刷新进程");
    btnRemoteInject = new QPushButton("远程线程注入(32位测试通过)");

    btnConsoleClear = new QPushButton("清空控制台");


    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    auto lb_e_magic = new QLabel("e_magic:");
    auto lb_e_lfanew = new QLabel("e_lfanew:");
    lb_e_magic->setPalette(pe);
    lb_e_lfanew->setPalette(pe);
    leftQWidgetGroup1Layout->addWidget(btnStart,        0, 0);
    leftQWidgetGroup1Layout->addWidget(btnRemoteInject, 0, 1);


    layout->addWidget(leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget(rightQWidget);
}

void Itemview10Inject::initConnect()
{
    // 远程线程加载DLL
    connect(btnRemoteInject, &QPushButton::clicked, [this]() {
        auto rowIndex = processTableView->currentIndex().row();

        if (rowIndex < 0) {
            return;
        }

        auto pidStr =  processGridModel->item(rowIndex, 1)->text();

        int pid = pidStr.toInt();

        DWORD dwProcessID = pid;
        const char *szDllpathName = "C:\\WorkMe\\wych-csharp-project\\bin\\Win32\\Release\\InjectTestDll.dll";


        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

        // 准备参数(1.参数要放在A进程中)
        //        char string_inject[] = "InjectTestDll.dll";
        //                char string_inject[] =  "C:\\WorkMe\\wych-csharp-project\\bin\\Win32\\Release\\InjectTestDll.dll";
        //                const char *string_inject2 =  "C:\\WorkMe\\wych-csharp-project\\bin\\Win32\\Release\\InjectTestDll.dll";

        // 在进程中分配内存
        LPVOID baseAddr = ::VirtualAllocEx(hProcess, NULL, strlen(szDllpathName) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

        if (baseAddr == NULL) {
            QMessageBox::warning(this, "警告", "VirtualAllocEx failure");
            return;
        }

        // 写入内存
        SIZE_T NumberOfBytesWritten = 0;

        if (!WriteProcessMemory(hProcess, baseAddr, szDllpathName, strlen(szDllpathName) + 1, &NumberOfBytesWritten)) {
            QMessageBox::warning(this, "警告", "WriteProcessMemory failure");
            return;
        }

        // 创建远程线程(2.直接调用LoadLibrary)
        //        HMODULE hModule = GetModuleHandle(L"kernel32.dll");
        //        DWORD dwLoadAddr = (DWORD)GetProcAddress(hModule, "LoadLibraryA");
        HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, /*TODO: */ baseAddr, 0, NULL);

        // 3、等待线程函数结束， 获取线程退出码,即LoadLibrary的返回值，即dll的首地址
        WaitForSingleObject(hRemoteThread, -1);
        DWORD exitCode = 0;

        if (!GetExitCodeThread(hRemoteThread, &exitCode)) {
            QMessageBox::warning(this, "警告", "GetExitCodeThread error!");
            return;
        }

        QMessageBox::warning(this, "警告", QString::number(GetLastError()));
    });


    connect(btnStart, &QPushButton::clicked, [this]() {
        PROCESSENTRY32 pe32;        // 进程结构
        pe32.dwSize = sizeof(pe32); // 在使用这个结构前，先设置它的大小
        // 给系统内所有的进程拍个快照
        HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hProcessSnap == INVALID_HANDLE_VALUE)
        {
            // 输出错误信息
            return;
        }
        BOOL bMore = Process32First(hProcessSnap, &pe32);

        // HANDLE hProcess = 0;
        // WCHAR procPath[_MAX_PATH] = {0};
        MODULEENTRY32 lpme;                  // DLL结构
        lpme.dwSize = sizeof(MODULEENTRY32); // 在使用这个结构前，先设置它的大小
        DWORD row = 0;                       // 初始化行号，要在循环中递增
        BOOL bRet = FALSE;
        char bufferPID[20] = { 0 };          // 用于将PID，镜像基址，镜像大小unsigned long类型的值转化为char*类型
        char bufferModBaseAddr[20] = { 0 };
        char bufferImageSize[20] = { 0 };

        HANDLE hModuleSnap = NULL;
        int i = 0;

        while (bMore)
        {
            processGridModel->setItem(i, 0, new QStandardItem(QString::fromWCharArray(pe32.szExeFile)));
            processGridModel->setItem(i, 1, new QStandardItem(QString::number(pe32.th32ProcessID)));

            // 给一个已存在的进程内所有的DLL拍个快照
            hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
            bRet = Module32First(hModuleSnap, &lpme);

            if (bRet)
            {
// 获取镜像基址
// 设置第row行第2列，镜像基址
                _ultoa((unsigned long)(lpme.modBaseAddr), bufferModBaseAddr, 16);

                processGridModel->setItem(i, 2, new QStandardItem(tr(bufferModBaseAddr)));
            }

            // 遍历DLL快照，找到进程的最后一个模块从而计算出镜像大小
            do
            {
                bRet = Module32Next(hModuleSnap, &lpme);
            } while (bRet);

            // 设置第row行第3列,镜像大小
            _ultoa((unsigned long)(lpme.modBaseAddr) + (unsigned long)lpme.modBaseSize, bufferImageSize, 16);
            processGridModel->setItem(i, 3, new QStandardItem(tr(bufferImageSize)));

            bMore = Process32Next(hProcessSnap, &pe32);
            i++;
        }
    });
}
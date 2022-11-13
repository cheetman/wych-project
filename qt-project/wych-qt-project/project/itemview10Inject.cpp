

#include <Windows.h>
#include <tchar.h>
#include <QTextCodec>
#include <TlHelp32.h>
#include <QDateTime>
#include <vector>
#include <QMenu>
#include "utils.h"
#include "itemview10Inject.h"
#include "itemview10Script.h"
#include "itemview10.h"


std::vector<HWND> childWindows;


Itemview10Inject::Itemview10Inject(QWidget *parent)
    : QWidget{parent}
{
    this->parent = (ItemView10 *)parent;
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


    //    auto leftQWidgetGroupBox3 = new QGroupBox("模块", this);
    //    leftQWidgetLayout->addWidget(leftQWidgetGroupBox3);
    //    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    //    auto leftQWidgetGroup1Layout3 = new QGridLayout(leftQWidgetGroupBox3);


    processTableView = new QTableView(this);
    processGridModel = new QStandardItemModel();
    processGridModel->setHorizontalHeaderLabels({ "窗口", "进程", "PID", "平台", "镜像基址", "镜像大小", "用户名", "用户所属" });
    processTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    processTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    processTableView->setModel(processGridModel);
    processTableView->setContextMenuPolicy(Qt::CustomContextMenu); // 右键功能
    leftQWidgetGroup1Layout2->addWidget(processTableView);


    // 第一层 (中间列)
    auto centerQWidget = new QWidget(this);
    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);


    // 第二层(中间列)

    auto centerQWidgetGroupBox2 = new QGroupBox("进程状态", centerQWidget);
    auto centerQWidgetGroupBox2Layout = new QGridLayout(centerQWidgetGroupBox2);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox2);


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

    // 第三层(右边列)
    auto tabTabWidget = new QTabWidget(rightQWidgetGroupBox1);
    auto tab = new QWidget(tabTabWidget);
    auto exportTabTabWidgetLayout = new QVBoxLayout(tab);
    auto tab2 = new QWidget(tabTabWidget);
    auto importTabTabWidgetLayout = new QVBoxLayout(tab2);
    auto tab3 = new QWidget(tabTabWidget);
    auto relocationTabTabWidgetLayout = new QVBoxLayout(tab3);
    tabTabWidget->addTab(tab,  tr("模块"));
    tabTabWidget->addTab(tab2, tr("窗口"));
    tabTabWidget->addTab(tab3, tr("重定位表"));
    rightQWidgetGroupBox1Layout->addWidget(tabTabWidget);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    // 第四层(导出)(右边列)
    auto exportTabTabWidgetGroupBox1 = new QGroupBox("头部", tab);
    exportTabTabWidgetLayout->addWidget(exportTabTabWidgetGroupBox1);

    exportTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto exportTabTabWidgetGroupBox1Layout = new QGridLayout(exportTabTabWidgetGroupBox1);

    //    exportTabTabWidgetGroupBox1->setFixedHeight(100);


    auto exportTabTabWidgetGroupBox2 = new QGroupBox("明细", tab);
    exportTabTabWidgetLayout->addWidget(exportTabTabWidgetGroupBox2);
    exportTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto exportTabTabWidgetGroupBox2Layout = new QGridLayout(exportTabTabWidgetGroupBox2);

    moduleTableView = new QTableView(this);
    moduleGridModel = new QStandardItemModel();
    moduleGridModel->setHorizontalHeaderLabels({  "模块名称", "模块基址",  "模块大小" });
    moduleTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    moduleTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    moduleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    moduleTableView->setModel(moduleGridModel);

    //    exportTableView->setColumnHidden(0, true);
    exportTabTabWidgetGroupBox2Layout->addWidget(moduleTableView);


    //    exportTableView = new QTableView(this);
    //    exportGridModel = new QStandardItemModel();
    //    exportGridModel->setHorizontalHeaderLabels({  "索引",  "地址_RVA", "地址_FOA", "序号表",  "函数名" });
    //    exportTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    exportTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    exportTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    exportTableView->setModel(exportGridModel);

    //    exportTabTabWidgetGroupBox2Layout->addWidget(exportTableView);


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


    // 第四层(窗口)

    auto importTabTabWidgetGroupBox3 = new QGroupBox("操作", tab2);
    importTabTabWidgetLayout->addWidget(importTabTabWidgetGroupBox3);
    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto importTabTabWidgetGroupBoxLayout3 = new QHBoxLayout(importTabTabWidgetGroupBox3);

    //    importTabTabWidgetGroupBox3->setFixedHeight(60);
    //    importTabTabWidgetGroupBoxLayout3->addWidget(btnImportAdd);


    auto importTabTabWidgetGroupBox = new QGroupBox("窗口明细", tab2);
    importTabTabWidgetLayout->addWidget(importTabTabWidgetGroupBox);
    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    auto importTabTabWidgetGroupBoxLayout = new QGridLayout(importTabTabWidgetGroupBox);

    //    importTabTabWidgetGroupBox->setFixedHeight(250);
    WindowsTableView = new QTableView(this);
    WindowsGridModel = new QStandardItemModel();
    WindowsGridModel->setHorizontalHeaderLabels({  "句柄",  "父句柄", "标题", "类", "窗口大小" });
    WindowsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    WindowsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    WindowsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    WindowsTableView->setModel(WindowsGridModel);
    importTabTabWidgetGroupBoxLayout->addWidget(WindowsTableView);

    //    auto importTabWidgetGroupBox2 = new QGroupBox("函数", tab2);
    //    importTabTabWidgetLayout->addWidget(importTabWidgetGroupBox2);
    //    importTabTabWidgetLayout->setAlignment(Qt::AlignTop);
    //    auto importTabWidgetGroupBoxLayout2 = new QGridLayout(importTabWidgetGroupBox2);
    //    import2TableView = new QTableView(this);
    //    import2GridModel = new QStandardItemModel();
    //    import2GridModel->setHorizontalHeaderLabels({ "查找表FOA(+4/+8)",  "提示/名称表RVA",  "提示/名称表FOA", "函数名称", "HINT[导出表索引]" });
    //    import2TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    import2TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    import2TableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    import2TableView->setModel(import2GridModel);
    //    importTabWidgetGroupBoxLayout2->addWidget(import2TableView);


    // 控件
    ckConsoleEnable = new QCheckBox("启动控制台");
    ckConsoleEnable->setChecked(true);
    btnStart = new QPushButton("刷新进程");
    btnDebugPrivilege = new QPushButton("Debug权限");
    btnRemoteInject = new QPushButton("远程线程注入(32位测试通过)");
    btnReflectiveInject = new QPushButton("反射注入");
    btnConsoleClear = new QPushButton("清空控制台");
    btnWindowPrint = new QPushButton("截图");

    tb_export_rva = new QLineEdit("");
    tb_import_rva = new QLineEdit("");
    tb_resource_rva = new QLineEdit("");
    tb_base_relocation_rva = new QLineEdit("");
    tb_window_size = new QLineEdit("");
    tb_window2_size = new QLineEdit("");
    tb_window_position = new QLineEdit("");
    tb_window2_position = new QLineEdit("");
    tb_export_foa = new QLineEdit("");
    tb_import_foa = new QLineEdit("");
    tb_resource_foa = new QLineEdit("");
    tb_base_relocation_foa = new QLineEdit("");


    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    auto lb_e_magic = new QLabel("e_magic:");
    auto lb_e_lfanew = new QLabel("e_lfanew:");
    lb_e_magic->setPalette(pe);
    lb_e_lfanew->setPalette(pe);
    leftQWidgetGroup1Layout->addWidget(btnStart,            0, 0);
    leftQWidgetGroup1Layout->addWidget(btnDebugPrivilege,   0, 1);
    leftQWidgetGroup1Layout->addWidget(btnRemoteInject,     1, 0);
    leftQWidgetGroup1Layout->addWidget(btnReflectiveInject, 1, 1);


    centerQWidgetGroupBox2Layout->addWidget(new QLabel("窗口大小:"), 1, 0);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("内部大小:"), 2, 0);

    //    centerQWidgetGroupBox2Layout->addWidget(new QLabel("资源表:"),      3, 0);
    //    centerQWidgetGroupBox2Layout->addWidget(new QLabel("重定位表:"),     4, 0);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("大小:"),   0, 1);
    centerQWidgetGroupBox2Layout->addWidget(new QLabel("坐标:"),   0, 2);

    //    centerQWidgetGroupBox2Layout->addWidget(new QLabel("FOA:"),      0, 3);
    centerQWidgetGroupBox2Layout->addWidget(tb_window_size,      1, 1);
    centerQWidgetGroupBox2Layout->addWidget(tb_window2_size,     2, 1);
    centerQWidgetGroupBox2Layout->addWidget(tb_window_position,  1, 2);
    centerQWidgetGroupBox2Layout->addWidget(tb_window2_position, 2, 2);


    centerQWidgetGroupBox1Layout->addWidget(ckConsoleEnable);
    centerQWidgetGroupBox1Layout->addWidget(btnConsoleClear);


    importTabTabWidgetGroupBoxLayout3->addWidget(btnWindowPrint);

    //    centerQWidgetGroupBox2Layout->addWidget(tb_resource_rva,         3, 2);
    //    centerQWidgetGroupBox2Layout->addWidget(tb_base_relocation_rva,  4, 2);
    //    centerQWidgetGroupBox2Layout->addWidget(tb_export_foa,           1, 3);
    //    centerQWidgetGroupBox2Layout->addWidget(tb_import_foa,           2, 3);
    //    centerQWidgetGroupBox2Layout->addWidget(tb_resource_foa,         3, 3);
    //    centerQWidgetGroupBox2Layout->addWidget(tb_base_relocation_foa,  4, 3);

    layout->addWidget(leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget(rightQWidget);

    action_toScript = new QAction(tr("跳转到脚本"), this);
    menu_rightClick = new QMenu(this);
    menu_rightClick->addAction(action_toScript);
}

BOOL  CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
    handle_data & data = *(handle_data *)lParam;
    unsigned long process_id = 0;

    GetWindowThreadProcessId(handle, &process_id);


    BOOL is_main_window = GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);

    if ((data.process_id != process_id) || !is_main_window) return TRUE;

    data.window_handle = handle;
    return FALSE;
}

BOOL  CALLBACK enum_child_windows_callback(HWND hwndChild, LPARAM lParam)
{
    childWindows.push_back(hwndChild);

    return TRUE;
}

void Itemview10Inject::initConnect()
{
    // 截图
    connect(btnWindowPrint, &QPushButton::clicked, [this]() {
        auto rowIndex = processTableView->currentIndex().row();

        if (rowIndex < 0) {
            return;
        }

        auto isWindow =  processGridModel->item(rowIndex, 0)->text();

        if (isWindow.isEmpty()) {
            return;
        }

        auto pid =  processGridModel->item(rowIndex, 2)->text().toInt();


        const auto clientRectWidth = 200;
        const auto clientRectHeight = 200;

        // 位图信息
        BITMAPINFO bitmapInfo;
        bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo);
        bitmapInfo.bmiHeader.biWidth = clientRectWidth;
        bitmapInfo.bmiHeader.biHeight = clientRectHeight;
        bitmapInfo.bmiHeader.biPlanes = 1;
        bitmapInfo.bmiHeader.biBitCount = 32;
        bitmapInfo.bmiHeader.biSizeImage = clientRectWidth * clientRectHeight;
        bitmapInfo.bmiHeader.biCompression = BI_RGB;

        HWND hwnd_ = (HWND)0x2084A;
        HDC scrDc_;
        HDC memDc_;
        HBITMAP bitmap_;
        HBITMAP oldBitmap_;
        void *bitsPtr_;


        scrDc_ = ::GetWindowDC(hwnd_);         // 获取窗口DC
        memDc_ = ::CreateCompatibleDC(scrDc_); // 缓冲内存DC
        bitmap_ = ::CreateDIBSection(memDc_, &bitmapInfo, DIB_RGB_COLORS, &bitsPtr_, nullptr, 0);

        if (bitmap_ == nullptr)
        {
            ::DeleteDC(memDc_);
            ::ReleaseDC(hwnd_, scrDc_);
            return;
        }
        oldBitmap_ = static_cast<HBITMAP>(::SelectObject(memDc_, bitmap_));

        POINT bitbltStartPoint_ { 0, 0 };

        const auto ret = ::BitBlt(
            memDc_, 0, 0, clientRectWidth, clientRectHeight,
            scrDc_, bitbltStartPoint_.x, bitbltStartPoint_.y,
            SRCCOPY);

        if (ret == 0) {
            return;
        }

        //删除用过的对象
        ::SelectObject(memDc_, oldBitmap_);
        ::DeleteObject(bitmap_);
        ::DeleteDC(memDc_);
        ::ReleaseDC(hwnd_, scrDc_);
    });


    connect(btnConsoleClear, &QPushButton::clicked, [this]() {
        clearMessage();
    });

    connect(btnDebugPrivilege, &QPushButton::clicked, [this]() {
        HANDLE hToken;
        BOOL fOk = FALSE;

        if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        {
            TOKEN_PRIVILEGES tp;
            tp.PrivilegeCount = 1;
            LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

            fOk = (GetLastError() == ERROR_SUCCESS);
            CloseHandle(hToken);
        }
        appendMessage(tr("提升权限：%1!").arg(fOk));
    });


    // 反射注入
    connect(btnReflectiveInject, &QPushButton::clicked, [this]() {
        auto rowIndex = processTableView->currentIndex().row();

        if (rowIndex < 0) {
            return;
        }

        auto pidStr =  processGridModel->item(rowIndex, 2)->text();

        int pid = pidStr.toInt();


        QString fileName = QFileDialog::getOpenFileName(this, tr("文件对话框！"), "F:", tr("动态链接库(*dll *exe);"));

        if (fileName.isEmpty()) {
            return;
        }
        auto path = (wchar_t *)fileName.utf16();

        HANDLE hFile = NULL;
        HANDLE hModule = NULL;
        HANDLE hProcess = NULL;
        LPVOID lpBuffer = NULL;
        DWORD dwLength = 0;
        DWORD dwBytesRead = 0;

        do
        {
            hFile = CreateFileW(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile == INVALID_HANDLE_VALUE) {
                QMessageBox::warning(this, "警告", "Failed to open the DLL file!");
                return;
            }

            dwLength = GetFileSize(hFile, NULL);

            if ((dwLength == INVALID_FILE_SIZE) || (dwLength == 0)) {
                QMessageBox::warning(this, "警告", "Failed to get the DLL file size!");
                return;
            }

        #ifdef _DEBUG
            wprintf(TEXT("[+] File Size: %d\n"), dwLength);
        #endif // ifdef _DEBUG

            lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);

            if (!lpBuffer) {
                QMessageBox::warning(this, "警告", "Failed to get the DLL file size!");
                return;
            }

            if (ReadFile(hFile, lpBuffer, dwLength, &dwBytesRead, NULL) == FALSE)  {
                QMessageBox::warning(this, "警告", "Failed to alloc a buffer!");
                return;
            }


            hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);

            if (!hProcess) {
                QMessageBox::warning(this, "警告", "Failed to open the target process!");
                return;
            }

            hModule = Utils::LoadRemoteLibraryR(hProcess, lpBuffer, dwLength, NULL);

            if (!hModule) {
                QMessageBox::warning(this, "警告", "Failed to inject the DLL!");
                return;
            }

            //                wprintf(TEXT("[+] Injected '%s' into process ID %d!"), cpDllFile, dwProcessId);

            WaitForSingleObject(hModule, -1);
        } while (0);

        if (lpBuffer) HeapFree(GetProcessHeap(), 0, lpBuffer);

        if (hProcess) CloseHandle(hProcess);
    });


    // 远程线程加载DLL
    connect(btnRemoteInject, &QPushButton::clicked, [this]() {
        auto rowIndex = processTableView->currentIndex().row();

        if (rowIndex < 0) {
            return;
        }

        auto pidStr =  processGridModel->item(rowIndex, 2)->text();

        int pid = pidStr.toInt();

        QString fileName = QFileDialog::getOpenFileName(this, tr("文件对话框！"), "F:", tr("动态链接库(*dll *exe);"));

        if (fileName.isEmpty()) {
            return;
        }
        auto path = (wchar_t *)fileName.utf16();


        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

        // 准备参数(1.参数要放在A进程中)
        //        const char *szDllpathName = "C:\\WorkMe\\wych-csharp-project\\bin\\Win32\\Release\\InjectTestDll.dll";


        DWORD dwSize = (lstrlenW(path) + 1) * sizeof(wchar_t);

        //         DWORD dwSize = strlen(szDllpathName) + 1;
        // 在进程中分配内存
        LPVOID baseAddr = ::VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

        if (baseAddr == NULL) {
            QMessageBox::warning(this, "警告", "VirtualAllocEx failure");
            return;
        }

        // 写入内存
        SIZE_T NumberOfBytesWritten = 0;

        if (!WriteProcessMemory(hProcess, baseAddr, path, dwSize, &NumberOfBytesWritten)) {
            QMessageBox::warning(this, "警告", "WriteProcessMemory failure");
            return;
        }

        // 创建远程线程(2.直接调用LoadLibrary)
        //        HMODULE hModule = GetModuleHandle(L"kernel32.dll");
        //        DWORD dwLoadAddr = (DWORD)GetProcAddress(hModule, "LoadLibraryA");
        HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, /*TODO: */ baseAddr, 0, NULL);

        // 3、等待线程函数结束， 获取线程退出码,即LoadLibrary的返回值，即dll的首地址
        WaitForSingleObject(hRemoteThread, -1);
        DWORD exitCode = 0;

        if (!GetExitCodeThread(hRemoteThread, &exitCode)) {
            QMessageBox::warning(this, "警告", "GetExitCodeThread error!");
            return;
        }

        //        QMessageBox::warning(this, "警告", QString::number(GetLastError()));
    });


    // 刷新进程
    connect(btnStart, &QPushButton::clicked, [this]() {
        // 清空
        //        processGridModel->clear();

        PROCESSENTRY32 pe32;        // 进程结构
        pe32.dwSize = sizeof(pe32); // 在使用这个结构前，先设置它的大小
        // 给系统内所有的进程拍个快照
        HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hProcessSnap == INVALID_HANDLE_VALUE)
        {
            appendMessage(tr("CreateToolhelp32Snapshot 报错 errorcode:[%1]").arg(GetLastError()));
            return;
        }
        BOOL bMore = Process32First(hProcessSnap, &pe32);

        // HANDLE hProcess = 0;
        // WCHAR procPath[_MAX_PATH] = {0};
        MODULEENTRY32 lpme;                  // DLL结构
        lpme.dwSize = sizeof(MODULEENTRY32); // 在使用这个结构前，先设置它的大小
        BOOL bRet = FALSE;
        TCHAR bufferImageSize[20] = { 0 };
        TCHAR bufferModBaseAddr[20] = { 0 };

        HANDLE hModuleSnap = NULL;
        int i = 0;

        while (bMore)
        {
            BOOL is32;

            QString platform;
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

            if (hProcess == 0)
            {
                appendMessage(tr("OpenProcess[%1] 报错 errorcode:[%2] ").arg(QString::fromWCharArray(pe32.szExeFile)).arg(GetLastError()));
                platform = tr("-");
            }
            else
            {
                IsWow64Process(hProcess, &is32);
                platform = is32 ? tr("x86") : tr("x64");

// 64-bit process on 64-bit Windows : FALSE
// 32-bit process on 64-bit Windows : TRUE
// 32-bit process on 32-bit Windows : FALSE
// 必须相同系统
                if (is32 != (sizeof(intptr_t) == 4)) {
                    bMore = Process32Next(hProcessSnap, &pe32);
                    CloseHandle(hProcess);
                    continue;
                }


// 判断窗口
                handle_data data;
                data.process_id = pe32.th32ProcessID;
                data.window_handle = 0;
                EnumWindows(enum_windows_callback, (LPARAM)&data);

                if (data.window_handle) {
                    processGridModel->setItem(i, 0, new QStandardItem(tr("✔")));
                }

// 获取用户信息
                while (true) {
                    HANDLE hToken;

                    if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                        appendMessage(tr("OpenProcessToken[%1] 报错 errorcode:[%2] ").arg(QString::fromWCharArray(pe32.szExeFile)).arg(GetLastError()));
                        break;
                    }

                    DWORD dwError;
                    DWORD len = 0;

                    if (!GetTokenInformation(hToken, TokenOwner, NULL, 0, &len))
                    {
                        dwError = GetLastError();

                        if (dwError != ERROR_INSUFFICIENT_BUFFER)
                        {
                            appendMessage(tr("GetTokenInformation[%1] 报错 errorcode:[%2] ").arg(QString::fromWCharArray(pe32.szExeFile)).arg(GetLastError()));
                            CloseHandle(hToken);
                            break;
                        }
                    }


                    PTOKEN_OWNER to = (PTOKEN_OWNER)LocalAlloc(LPTR, len);

                    if (!to)
                    {
                        appendMessage(tr("LocalAlloc[%1] 报错 errorcode:[%2] ").arg(QString::fromWCharArray(pe32.szExeFile)).arg(GetLastError()));
                        CloseHandle(hToken);
                        break;
                    }

                    if (!GetTokenInformation(hToken, TokenOwner, to, len, &len))
                    {
                        appendMessage(tr("GetTokenInformation[%1] 报错 errorcode:[%2] ").arg(QString::fromWCharArray(pe32.szExeFile)).arg(GetLastError()));
                        LocalFree(to);
                        CloseHandle(hToken);
                        break;
                    }

                    char nameUser[256] = { 0 };
                    char domainName[256] = { 0 };
                    DWORD nameUserLen = 256;
                    DWORD domainNameLen = 256;
                    SID_NAME_USE snu;

                    if (!LookupAccountSidA(NULL, to->Owner, nameUser, &nameUserLen, domainName, &domainNameLen, &snu))
                    {
                        dwError = GetLastError();
                        appendMessage(tr("LookupAccountSid[%1] 报错 errorcode:[%2] ").arg(QString::fromWCharArray(pe32.szExeFile)).arg(GetLastError()));
                        LocalFree(to);
                        CloseHandle(hToken);
                        break;
                    }


                    LocalFree(to);
                    CloseHandle(hToken);
                    processGridModel->setItem(i, 6, new QStandardItem(QString::fromLocal8Bit(nameUser)));

//                    processGridModel->setItem(i, 6, new QStandardItem(QString::fromLocal8Bit(nameUser)));
                    processGridModel->setItem(i, 7, new QStandardItem(tr(domainName)));
                    break;
                }


                CloseHandle(hProcess);
            }


            processGridModel->setItem(i, 3, new QStandardItem(platform));
            processGridModel->setItem(i, 1, new QStandardItem(QString::fromWCharArray(pe32.szExeFile)));
            processGridModel->setItem(i, 2, new QStandardItem(QString::number(pe32.th32ProcessID)));

            // 给一个已存在的进程内所有的DLL拍个快照
            hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
            bRet = Module32First(hModuleSnap, &lpme);

            if (bRet)
            {
// 获取镜像基址
// 设置第row行第2列，镜像基址
                _i64tot_s((size_t)(lpme.modBaseAddr), bufferModBaseAddr, 20, 16);
                processGridModel->setItem(i, 4, new QStandardItem(QString::fromWCharArray(bufferModBaseAddr)));
            }

            // 遍历DLL快照，找到进程的最后一个模块从而计算出镜像大小
            if (bRet) {
                do
                {
                    bRet = Module32Next(hModuleSnap, &lpme);
                } while (bRet);

// 设置第row行第3列,镜像大小
                _i64tot_s((size_t)(lpme.modBaseAddr) + (size_t)lpme.modBaseSize, bufferImageSize, 20, 16);
                processGridModel->setItem(i, 5, new QStandardItem(QString::fromWCharArray(bufferImageSize)));

                CloseHandle(hModuleSnap);
            }

            i++;


            bMore = Process32Next(hProcessSnap, &pe32);
        }
        auto removeCount = processGridModel->rowCount() - i;

        if (removeCount > 0) {
            processGridModel->removeRows(i, removeCount);
        }

        CloseHandle(hProcessSnap);
        appendMessage(tr("刷新进程成功!"));
    });


    // 事件 - 表格右键
    connect(processTableView, &QTableView::customContextMenuRequested, [this](const QPoint& pos) {
        QModelIndex index = processTableView->indexAt(pos); // 找到tableview当前位置信息

        if (index.isValid())                                // 如果行数有效，则显示菜单
        {
            menu_rightClick->exec(QCursor::pos());
        }
    });

    // 事件 - 表格双击
    connect(processTableView, &QTableView::doubleClicked, [this](const QModelIndex& current) {
        auto rowIndex = current.row();
        auto pid =  processGridModel->item(rowIndex, 2)->text().toInt();

        // 刷新窗口
        handle_data data;
        data.process_id = pid;
        data.window_handle = 0;
        EnumWindows(enum_windows_callback, (LPARAM)&data);


        if (data.window_handle) {
            // 窗口大小

            RECT rect{ 0, 0, 0, 0 };
            GetWindowRect(data.window_handle, &rect);


            tb_window_size->setText(tr("[ %1 x %2 ]").arg(rect.right - rect.left).arg(rect.bottom - rect.top));
            tb_window_position->setText(tr("(%1,%2),(%3,%4)").arg(rect.left).arg(rect.top).arg(rect.right).arg(rect.bottom));

            // 内部窗口，注意没有位置坐标
            RECT rect2{ 0, 0, 0, 0 };
            POINT point{ 0, 0 };
            GetClientRect(data.window_handle, &rect2);


            // 算内部窗口的坐标
            ClientToScreen(data.window_handle, &point);
            tb_window2_size->setText(tr("[ %1 x %2 ]").arg(rect2.right - rect2.left).arg(rect2.bottom - rect2.top));
            tb_window2_position->setText(tr("(%1,%2),(%3,%4)").arg(point.x).arg(point.y).arg(point.x + rect2.right).arg(point.y + rect2.bottom));

            //            GetClientRect(obj->gameHwnd, &obj->gameRect);
            //            obj->gamePoint.x = 0;
            //            obj->gamePoint.y = 0;
            //            ClientToScreen(obj->gameHwnd, &obj->gamePoint);
            //            MoveWindow(obj->newHwnd, obj->gamePoint.x, obj->gamePoint.y, obj->gameRect.right, obj->gameRect.bottom, true);

            childWindows.clear();

            EnumChildWindows(data.window_handle, enum_child_windows_callback, (LPARAM)NULL);

            TCHAR m_Name[MAXBYTE];
            TCHAR m_Title[MAXBYTE];
            GetClassName(data.window_handle, m_Name, MAXBYTE);   // 获得指定窗⼝所属的类的类名
            GetWindowText(data.window_handle, m_Title, MAXBYTE); // 查找标题

            WindowsGridModel->setItem(0, 0, new QStandardItem(QString::number((intptr_t)(data.window_handle), 16)));
            WindowsGridModel->setItem(0, 2, new QStandardItem(QString::fromWCharArray(m_Name)));
            WindowsGridModel->setItem(0, 3, new QStandardItem(QString::fromWCharArray(m_Title)));


            for (std::vector<HWND>::iterator iter = childWindows.begin(); iter != childWindows.end(); ++iter) {
                int index = std::distance(childWindows.begin(), iter) + 1;


                GetClassName((*iter), m_Name, MAXBYTE);   // 获得指定窗⼝所属的类的类名
                GetWindowText((*iter), m_Title, MAXBYTE); // 查找标题
                HWND p = GetParent((*iter));

                WindowsGridModel->setItem(index, 0, new QStandardItem(QString::number((intptr_t)(*iter), 16)));
                WindowsGridModel->setItem(index, 1, new QStandardItem(QString::number((intptr_t)p, 16)));
                WindowsGridModel->setItem(index, 2, new QStandardItem(QString::fromWCharArray(m_Name)));
                WindowsGridModel->setItem(index, 3, new QStandardItem(QString::fromWCharArray(m_Title)));
            }


            auto removeCount = WindowsGridModel->rowCount() - (childWindows.size() + 1);

            if (removeCount > 0) {
                WindowsGridModel->removeRows(childWindows.size() + 1, removeCount);
            }
        } else {
            tb_window_size->setText("无窗口");
            tb_window_position->setText("");
            tb_window2_size->setText("无窗口");
            tb_window2_position->setText("");
            WindowsGridModel->clear();
        }


        // 刷新模块
        HANDLE      hModuleSnap = NULL;
        MODULEENTRY32 lpme; // DLL结构
        lpme.dwSize = sizeof(MODULEENTRY32);
        hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

        if (hModuleSnap == INVALID_HANDLE_VALUE) {
            appendMessage(tr("CreateToolhelp32Snapshot 报错 errorcode:[%1]").arg(GetLastError()));
            return;
        }
        BOOL bRet = Module32First(hModuleSnap, &lpme);
        TCHAR bufferModBaseAddr[20] = { 0 };
        TCHAR buffer[0x20];
        int i = 0;

        while (bRet)
        {
            // 设置模块名称
            moduleGridModel->setItem(i, 0, new QStandardItem(QString::fromWCharArray(lpme.szModule)));


            // 设置模块基址
            _i64tot_s((size_t)(lpme.modBaseAddr), bufferModBaseAddr, 20, 16);
            moduleGridModel->setItem(i, 1, new QStandardItem(QString::fromWCharArray(bufferModBaseAddr)));


            // 设置模块大小
            _i64tot_s(lpme.modBaseSize, buffer, 20, 16);
            moduleGridModel->setItem(i, 2, new QStandardItem(QString::fromWCharArray(buffer)));

            bRet = Module32Next(hModuleSnap, &lpme);
            i++;
        }

        auto removeCount = moduleGridModel->rowCount() - i;

        if (removeCount > 0) {
            moduleGridModel->removeRows(i, removeCount);
        }

        CloseHandle(hModuleSnap);
        appendMessage(tr("刷新进程模块成功"));
    });


    connect(action_toScript, &QAction::triggered, [this]() {
        auto rowIndex = processTableView->currentIndex().row();

        if (rowIndex < 0) {
            return;
        }

        auto isWindow =  processGridModel->item(rowIndex, 0)->text();

        if (isWindow.isEmpty()) {
            appendMessage(tr("该进程没有窗口！"));
            return;
        }


        auto pid =  processGridModel->item(rowIndex, 2)->text().toInt();

        auto success = parent->itemview10Script->buildProcess((DWORD)pid);

        if (success) {
            parent->SetTabIndex(2);
        }
    });
}

void Itemview10Inject::appendMessage(const QString& msg)
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

void Itemview10Inject::showMessage(const QString& msg)
{
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void Itemview10Inject::clearMessage()
{
    edtMsg->clear();
}

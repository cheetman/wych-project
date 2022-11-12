
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

    //    leftQWidget->setFixedWidth(300);

    // 第二层
    auto leftQWidgetGroupBox1 = new QGroupBox("状态", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    leftQWidgetGroupBox1->setFixedHeight(250);
    auto layout_l_1_m = new QVBoxLayout(this);
    auto leftQWidgetGroup1Layout = new QGridLayout(this);
    auto leftQWidgetGroup1bLayout = new QHBoxLayout(this);
    auto leftQWidgetGroup1cLayout = new QHBoxLayout(this);
    auto leftQWidgetGroup1dLayout = new QHBoxLayout(this);
    leftQWidgetGroup1bLayout->setAlignment(Qt::AlignLeft);
    leftQWidgetGroup1cLayout->setAlignment(Qt::AlignLeft);
    leftQWidgetGroup1dLayout->setAlignment(Qt::AlignLeft);
    leftQWidgetGroupBox1->setLayout(layout_l_1_m);
    layout_l_1_m->addLayout(leftQWidgetGroup1Layout);
    layout_l_1_m->addLayout(leftQWidgetGroup1bLayout);
    layout_l_1_m->addLayout(leftQWidgetGroup1cLayout);
    layout_l_1_m->addLayout(leftQWidgetGroup1dLayout);


    auto leftQWidgetGroupBox2 = new QGroupBox("截图", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout2 = new QGridLayout(leftQWidgetGroupBox2);


    auto scrollArea = new QScrollArea(this);

    leftQWidgetGroup1Layout2->addWidget(scrollArea);

    pixmapWidget = new PixmapWidget(scrollArea);
    scrollArea->setWidget(pixmapWidget);

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

    auto centerQWidgetGroupBox1 = new QGroupBox("控制台设置", rightQWidget);
    auto centerQWidgetGroupBox1Layout = new QHBoxLayout(centerQWidgetGroupBox1);
    rightQWidgetLayout->addWidget(centerQWidgetGroupBox1);

    edtMsg = new QPlainTextEdit();
    edtMsg->setReadOnly(true);
    rightQWidgetLayout->addWidget(edtMsg);


    // 第三层(右边列)
    auto tabTabWidget = new QTabWidget(rightQWidgetGroupBox1);
    auto tab2 = new QWidget(tabTabWidget);
    auto importTabTabWidgetLayout = new QVBoxLayout(tab2);
    auto tab3 = new QWidget(tabTabWidget);
    auto relocationTabTabWidgetLayout = new QVBoxLayout(tab3);
    tabTabWidget->addTab(tab2, tr("窗口"));
    tabTabWidget->addTab(tab3, tr("重定位表"));
    rightQWidgetGroupBox1Layout->addWidget(tabTabWidget);


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
    tb_process_name = new QLineEdit("");
    tb_process_handle = new QLineEdit("");
    tb_window_name = new QLineEdit("");
    tb_window_handle = new QLineEdit("");
    tb_mouse_position = new QLineEdit("");


    rb_printWindow = new QRadioButton("窗口截图");
    rb_printClient = new QRadioButton("内容截图");
    rb_printClient->setChecked(true);

    bg_printConfig = new QButtonGroup(this);
    bg_printConfig->addButton(rb_printWindow, 0);
    bg_printConfig->addButton(rb_printClient, 1);


    rb_printWin32 = new QRadioButton("Win截图");
    rb_printWin32->setChecked(true);
    rb_printAdb = new QRadioButton("Android截图");
    bg_printType = new QButtonGroup(this);
    bg_printType->addButton(rb_printWin32, 0);
    bg_printType->addButton(rb_printAdb,   1);


    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    auto lb_e_magic = new QLabel("e_magic:");
    auto lb_e_lfanew = new QLabel("e_lfanew:");
    lb_e_magic->setPalette(pe);
    lb_e_lfanew->setPalette(pe);


    leftQWidgetGroup1Layout->addWidget(new QLabel("进程名称:"),       0, 0);
    leftQWidgetGroup1Layout->addWidget(new QLabel("窗口名称:"),       1, 0);

    leftQWidgetGroup1Layout->addWidget(tb_process_name,           0, 1);

    //    leftQWidgetGroup1Layout->addWidget(tb_process_handle,        0, 2);
    leftQWidgetGroup1Layout->addWidget(tb_window_name,            1, 1);
    leftQWidgetGroup1Layout->addWidget(tb_window_handle,          1, 2);


    leftQWidgetGroup1Layout->addWidget(new QLabel("窗口大小:", this), 5, 0);
    leftQWidgetGroup1Layout->addWidget(new QLabel("内部大小:", this), 6, 0);

    leftQWidgetGroup1Layout->addWidget(new QLabel("大小:", this),   4, 1);
    leftQWidgetGroup1Layout->addWidget(new QLabel("坐标:", this),   4, 2);

    leftQWidgetGroup1Layout->addWidget(tb_window_size,            5, 1);
    leftQWidgetGroup1Layout->addWidget(tb_window2_size,           6, 1);
    leftQWidgetGroup1Layout->addWidget(tb_window_position,        5, 2);
    leftQWidgetGroup1Layout->addWidget(tb_window2_position,       6, 2);

    leftQWidgetGroup1bLayout->addWidget(new QLabel("截图方式:", this));
    leftQWidgetGroup1bLayout->addWidget(rb_printWin32);
    leftQWidgetGroup1bLayout->addWidget(rb_printAdb);
    leftQWidgetGroup1bLayout->addWidget(btnWindowPrint);


    leftQWidgetGroup1cLayout->addWidget(new QLabel("截图区域:", this));
    leftQWidgetGroup1cLayout->addWidget(rb_printWindow);
    leftQWidgetGroup1cLayout->addWidget(rb_printClient);


    leftQWidgetGroup1dLayout->addWidget(new QLabel("鼠标坐标:", this));
    leftQWidgetGroup1dLayout->addWidget(tb_mouse_position);


    centerQWidgetGroupBox1Layout->addWidget(ckConsoleEnable);
    centerQWidgetGroupBox1Layout->addWidget(btnConsoleClear);


    // importTabTabWidgetGroupBoxLayout3->addWidget(btnWindowPrint);


    layout->addWidget(leftQWidget);

    //    layout->addWidget(centerQWidget);
    layout->addWidget(rightQWidget);
}

void Itemview10Script::initConnect()
{
    // 事件 - 图片框的鼠标事件
    connect(pixmapWidget, &PixmapWidget::mousePositionEvent, [this](int mouseX, int mouseY) {
        qDebug() << mouseX;
        tb_mouse_position->setText(tr("[x:%1, y:%2]").arg(mouseX).arg(mouseY));
    });


    // 截图
    connect(btnWindowPrint, &QPushButton::clicked, [this]() {
        //        QPixmap  m_pixmap = QPixmap("C:\\Users\\汪意超\\Pictures\\Saved Pictures\\001OdAkagy1gyybblipk3j60h00fvajh02.jpg");

        //        pixmapWidget->setPixmap(m_pixmap);

        //        return;


        HWND hwnd_ = windowInfo.HandleWindow;

        if (!hwnd_) {
            return;
        }
        HDC displayDC;
        HDC bitmapDC;

        if (rb_printWindow->isChecked()) {}

        displayDC = ::GetWindowDC(hwnd_);           // 获取窗口DC
        bitmapDC = ::CreateCompatibleDC(displayDC); // 缓冲内存DC

        HBITMAP bitmap = nullptr;
        int w = -1, h = -1, x = 0, y = 0;
        RECT r = { 0, 0, 0, 0 };
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
        return;

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


            return true;
        }
    }

    return false;
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

#include "winapi.h"

#define DEBUG_STRING
#ifdef DEBUG_STRING
# include <stdio.h>
# include <vector>
#endif // ifdef DEBUG_STRING


namespace WychUtils_WinAPI {
std::int32_t MessageBox(
    void        *a_wnd,
    const char  *a_text,
    const char  *a_caption,
    unsigned int a_type) noexcept
{
    return static_cast<std::int32_t>(
        ::MessageBoxA(
            static_cast<::HWND>(a_wnd),
            static_cast<::LPCSTR>(a_text),
            static_cast<::LPCSTR>(a_caption),
            static_cast<::UINT>(a_type)));
}

std::int32_t MessageBox(
    void          *a_wnd,
    const wchar_t *a_text,
    const wchar_t *a_caption,
    unsigned int   a_type) noexcept
{
    return static_cast<std::int32_t>(
        ::MessageBoxW(
            static_cast<::HWND>(a_wnd),
            static_cast<::LPCWSTR>(a_text),
            static_cast<::LPCWSTR>(a_caption),
            static_cast<::UINT>(a_type)));
}

void OutputDebugString(
    const char *a_outputString) noexcept
{
    ::OutputDebugStringA(
        static_cast<::LPCSTR>(a_outputString));
}

void OutputDebugString(
    const wchar_t *a_outputString) noexcept
{
    ::OutputDebugStringW(
        static_cast<::LPCWSTR>(a_outputString));
}

void error(LPCTSTR text)
{
    char buffer[4000];

    ZeroMemory(buffer, 4000);
    wsprintfA(buffer, "发生错误 : %s", text);

#ifdef  DEBUG_STRING
    printf("%s\n", buffer);
    OutputDebugString(buffer);
#endif // ifdef  DEBUG_STRING

    MessageBox(nullptr, buffer, nullptr, MB_OK);
    exit(-1);
}

void warning(LPCTSTR text)
{
    char buffer[5000];

    ZeroMemory(buffer, 5000);
    wsprintfA(buffer, "警告 : %s", text);

#ifdef DEBUG_STRING
    printf("%s \n", buffer);
    OutputDebugString(buffer);
#endif // ifdef DEBUG_STRING

    MessageBox(nullptr, buffer, nullptr, MB_OK);
}

dword read_memory(handle process, int address, void *recv, int size)
{
    SIZE_T read_size;

    ReadProcessMemory(process, (LPCVOID)address, recv, size, &read_size);
    return read_size;
}

dword read_memory(handle process, unsigned int address, void *recv, int size)
{
    SIZE_T read_size;

    ReadProcessMemory(process, (LPCVOID)address, recv, size, &read_size);
    return read_size;
}

dword write_memory(handle process, int address, void *data, int size)
{
    SIZE_T write_size;

    WriteProcessMemory(process, (LPVOID)address, data, size, &write_size);
    return write_size;
}

dword write_memory(handle process, unsigned int address, void *data, int size)
{
    SIZE_T write_size;

    WriteProcessMemory(process, (LPVOID)address, data, size, &write_size);
    return write_size;
}

void* alloc_memory(int size)
{
    return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void free_memory(void *ptr)
{
    if (ptr) VirtualFree(ptr, 0, MEM_RELEASE);
    ptr = nullptr;
}

handle get_process_handle(dword process_id, dword access)
{
    handle process_handle = OpenProcess(access, FALSE, process_id);

    if (!process_handle) {
        warning(L"打开进程句柄失败");
        return 0;
    }

#ifdef DEBUG_STRING
    printf("进程号码 : %8x \n", (unsigned int)process_id);
    printf("进程句柄 : %8x \n", (unsigned int)process_handle);
    printf("\n");
#endif // ifdef DEBUG_STRING

    return process_handle;
}

dword get_process_id(LPCTSTR process_name)
{
    handle snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snap == INVALID_HANDLE_VALUE) {
        error(L"CreateToolhelp32Snapshot失败");
    }

    PROCESSENTRY32 process_info;
    ZeroMemory(&process_info, sizeof(process_info));
    process_info.dwSize = sizeof(process_info);

    wchar_t target[1024];
    ZeroMemory(target, 1024);
    wcsncpy(target, process_name, wcslen(process_name));
    _wcsupr(target);

    BOOL state = Process32First(snap, &process_info);

    while (state)
    {
        if (wcsncmp(_wcsupr(process_info.szExeFile), target, wcslen(target)) == 0)
        {
            if (process_info.cntThreads > 0) {
#ifdef DEBUG_STRING
                _tprintf(_T("进程名称 : %s \n"), process_info.szExeFile);
                _tprintf(_T("进程ID : %d \n"), process_info.th32ProcessID);
                _tprintf(_T("\n"));
#endif // DEBUG_STRING

                CloseHandle(snap);
                return process_info.th32ProcessID;
            }
        }
        state = Process32Next(snap, &process_info);
    }

    CloseHandle(snap);
    warning(L"进程查找失败!");
    return 0;
}

void get_module_info(handle process_handle, dword process_id, LPCTSTR  module_name, struct module_information& info)
{
    // 支持64位获取32位模块
    handle snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, process_id);

    if (snap == INVALID_HANDLE_VALUE) {
        error(L"CreateToolhelp32Snapshot失败");
    }
    MODULEENTRY32 module_info;
    ZeroMemory(&module_info, sizeof(module_info));
    module_info.dwSize = sizeof(module_info);

    wchar_t target[1024];
    ZeroMemory(target, 1024);
    wcsncpy(target, module_name, wcslen(module_name));
    _wcsupr(target);

    BOOL state = Module32First(snap, &module_info);

    while (state)
    {
        if (wcsncmp(_wcsupr(module_info.szModule), target, wcslen(target)) == 0)
        {
            info.module_address = (int)module_info.modBaseAddr;
            info.module_handle = module_info.hModule;
            info.alloc(module_info.modBaseSize);
            ZeroMemory(info.module_name, sizeof(info.module_name));
            wcsncpy(info.module_name, module_info.szModule, wcslen(module_info.szModule));

            dword size = read_memory(process_handle, info.module_address, info.module_data, info.module_size);

            if (size == 0) {
                error(L"读取内存错误");
            }

#ifdef DEBUG_STRING
            _tprintf(_T("模块名称 : %s \n"),      module_info.szModule);
            _tprintf(_T("模块基址 : %8x \n"),               (unsigned int)module_info.modBaseAddr);
            _tprintf(_T("模块大小 : %d Byte \n"), module_info.modBaseSize);
            _tprintf(_T("实际读取 : %d Byte \n"),        size);
            _tprintf(_T("\n"));
#endif // ifdef DEBUG_STRING

            CloseHandle(snap);
            return;
        }
        state = Module32Next(snap, &module_info);
    }

    CloseHandle(snap);
    warning(L"!!!查找不到指定模块!!!");
}

int find_pattern(handle process, struct module_information& module, const char *pattern, int index, int offset)
{
    const char *start = module.module_data + offset;
    const int   length = strlen(pattern);

    int count = 0;

    for (int i = 0; i < module.module_size - offset; i++)
    {
        if ((start[i] == pattern[0]) || (pattern[0] == '?'))
        {
            int j = 1;

            for (; j < length; j++) if ((start[i + j] != pattern[j]) && (pattern[j] != '?')) break;

            if ((j == length) && (count++ == index)) return module.module_address + i + offset;
        }
    }

    warning(L"!!!无法匹配到该内存特征!!!");
    return -1;
}

void show_all_pattern(handle process, struct module_information& module, const char *pattern, int offset)
{
    const char *start = module.module_data + offset;
    const int   length = strlen(pattern);

    int count = 0;

    for (int i = 0; i < module.module_size - offset; i++)
    {
        if ((start[i] == pattern[0]) || (pattern[0] == '?'))
        {
            int j = 1;

            for (; j < length; j++) if ((start[i + j] != pattern[j]) && (pattern[j] != '?')) break;

            if (j == length)
            {
                printf("address : %8x \n", module.module_address + i + offset);
            }
        }
    }
}
}

namespace WinAPI {
//// 从NTDLL里定义原型
typedef DWORD (WINAPI *PNtQuerySystemInformation)(UINT systemInformation, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

#ifdef _M_IX86
typedef struct _CLIENT_ID
{
    DWORD UniqueProcess;
    DWORD UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
#endif // x86模式下
#ifdef _M_X64
typedef struct _CLIENT_ID
{
    ULONG64 UniqueProcess;
    ULONG64 UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
#endif // x64模式下

#define SystemProcessInformation    5 // 功能号

typedef LONG KPRIORITY;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

// 进程结构体，从官网copy
typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG          NextEntryOffset;
    ULONG          NumberOfThreads;
    BYTE           Reserved1[48];
    UNICODE_STRING ImageName;
    KPRIORITY      BasePriority;
    HANDLE         UniqueProcessId;
    PVOID          Reserved2;
    ULONG          HandleCount;
    ULONG          SessionId;
    PVOID          Reserved3;
    SIZE_T         PeakVirtualSize;
    SIZE_T         VirtualSize;
    ULONG          Reserved4;
    SIZE_T         PeakWorkingSetSize;
    SIZE_T         WorkingSetSize;
    PVOID          Reserved5;
    SIZE_T         QuotaPagedPoolUsage;
    PVOID          Reserved6;
    SIZE_T         QuotaNonPagedPoolUsage;
    SIZE_T         PagefileUsage;
    SIZE_T         PeakPagefileUsage;
    SIZE_T         PrivatePageCount;
    LARGE_INTEGER  Reserved7[6];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

// 线程结构体，从官网copy
typedef struct _SYSTEM_THREAD_INFORMATION {
    LARGE_INTEGER Reserved1[3];
    ULONG         Reserved2;
    PVOID         StartAddress;
    CLIENT_ID     ClientId;
    KPRIORITY     Priority;
    LONG          BasePriority;
    ULONG         Reserved3;
    ULONG         ThreadState;
    ULONG         WaitReason;
} SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;


BOOL get_process_info(DWORD pid, PWIN32_PROCESS_INFO result) {
    BOOL success = FALSE;
    PNtQuerySystemInformation NtQuerySystemInformation = NULL;

    NtQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(LoadLibrary(_T("ntdll.dll")), "NtQuerySystemInformation");

    if (NtQuerySystemInformation == NULL) {
        return success;
    }
    PSYSTEM_PROCESS_INFORMATION sysProInfo = NULL, old = NULL;
    ULONG cbSize = sizeof(SYSTEM_PROCESS_INFORMATION);
    LONG  status = 0;

    do {
        old = sysProInfo = (PSYSTEM_PROCESS_INFORMATION)malloc(cbSize);
        status = NtQuerySystemInformation(SystemProcessInformation, sysProInfo, cbSize, &cbSize);

        if (status) free(sysProInfo);
    } while (status);

    // 遍历进程
    do {
        if (sysProInfo->ImageName.Buffer != NULL)
        {
            _tprintf(L"进程名:\t%s \t进程ID:%u \t句柄总数:%u \t线程总数:%u \n", sysProInfo->ImageName.Buffer, sysProInfo->UniqueProcessId,
                     sysProInfo->HandleCount, sysProInfo->NumberOfThreads);

            // 找到进程
            if (pid == (DWORD)sysProInfo->UniqueProcessId) {
                wcsncpy_s(result->PName, ARRAYSIZE(result->PName), sysProInfo->ImageName.Buffer, ARRAYSIZE(result->PName) - 1);
                result->PID = pid;
                result->NumberOfThreads = sysProInfo->NumberOfThreads;
                result->HandleCount = sysProInfo->HandleCount;
                success = TRUE;
                break;
            }

            // 打印线程信息

            /*PSYSTEM_THREAD_INFORMATION threadInfo = NULL;
               threadInfo = (PSYSTEM_THREAD_INFORMATION)((ULONG64)sysProInfo + sizeof(SYSTEM_PROCESS_INFORMATION));
               DWORD curThreadIndex = 1;

               do {
                    _tprintf(L"\t线程ID:%u\t起始地址:%x \t线程的状态码:%u\n", threadInfo->ClientId.UniqueThread, threadInfo->StartAddress, threadInfo->ThreadState);
                    threadInfo += 1;
               } while (curThreadIndex++ < sysProInfo->NumberOfThreads);
               _tprintf(L"\n");*/
        }

        // 指针的加减运算的单位是根据所指向数据类型大小的。字节指针就是1，所以加减运算没问题。这里是结构体指针，所以必须转成数字类型再运算。
        sysProInfo = (PSYSTEM_PROCESS_INFORMATION)((ULONG64)sysProInfo + sysProInfo->NextEntryOffset);
    } while (sysProInfo->NextEntryOffset != 0);
    free(old);

    return success;
}

BOOL  CALLBACK callback_enum_window_child(HWND hwndChild, LPARAM lParam)
{
    std::vector<HWND> *data = (std::vector<HWND> *)lParam;

    data->push_back(hwndChild);
    return TRUE;
}

BOOL  CALLBACK callback_enum_window_main(HWND handle, LPARAM lParam)
{
    PWIN32_WINDOW_INFO info = (PWIN32_WINDOW_INFO)lParam;
    unsigned long process_id = 0;

    GetWindowThreadProcessId(handle, &process_id);


    BOOL is_main_window = GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);

    if ((info->PID != process_id) || !is_main_window) return TRUE;

    info->HandleWindow = handle;
    return FALSE;
}

BOOL get_window_info(HWND handle, PWIN32_WINDOW_INFO result) {
    if (!handle) {
        return FALSE;
    }

    result->HandleWindow = handle;
    GetWindowRect(result->HandleWindow, &result->WindowRect);
    GetClientRect(result->HandleWindow, &result->ClientRect);
    result->ClientToScreen = { 0, 0 };
    ClientToScreen(result->HandleWindow, &result->ClientToScreen);
    GetClassName(result->HandleWindow, result->ClassName, MAXBYTE);  // 获得指定窗⼝所属的类的类名
    GetWindowText(result->HandleWindow, result->TitleName, MAXBYTE); // 查找标题
    result->IsMaximized = IsZoomed(result->HandleWindow);
    result->IsMinimized = IsIconic(result->HandleWindow);

    return TRUE;
}

BOOL get_window_main(DWORD pid, PWIN32_WINDOW_INFO result) {
    result->PID = pid;
    EnumWindows(callback_enum_window_main, (LPARAM)result);

    if (!result->HandleWindow) {
        return FALSE;
    }

    GetWindowRect(result->HandleWindow, &result->WindowRect);
    GetClientRect(result->HandleWindow, &result->ClientRect);
    ClientToScreen(result->HandleWindow, &result->ClientToScreen);
    GetClassName(result->HandleWindow, result->ClassName, MAXBYTE);  // 获得指定窗⼝所属的类的类名
    GetWindowText(result->HandleWindow, result->TitleName, MAXBYTE); // 查找标题
    result->IsMaximized = IsZoomed(result->HandleWindow);
    result->IsMinimized = IsIconic(result->HandleWindow);


    return TRUE;
}

BOOL get_window_child(HWND handleWindow, std::vector<WIN32_WINDOW_INFO>& list) {
    std::vector<HWND> childWindows;

    EnumChildWindows(handleWindow, callback_enum_window_child, (LPARAM)&childWindows);

    list.clear();

    for (std::vector<HWND>::iterator iter = childWindows.begin(); iter != childWindows.end(); ++iter) {
        //        int index = std::distance(childWindows.begin(), iter) + 1;


        WIN32_WINDOW_INFO result;
        result.HandleWindow = (*iter);
        result.HandleParentWindow = GetParent((*iter));

        GetWindowRect((*iter), &result.WindowRect);
        GetClientRect((*iter), &result.ClientRect);
        ClientToScreen((*iter), &result.ClientToScreen);
        GetClassName((*iter), result.ClassName, MAXBYTE);  // 获得指定窗⼝所属的类的类名
        GetWindowText((*iter), result.TitleName, MAXBYTE); // 查找标题
        result.IsMaximized = IsZoomed((*iter));
        result.IsMinimized = IsIconic((*iter));

        list.push_back(result);
    }


    return TRUE;
}

// 获取进程信息(ntdll - NtQuerySystemInformation)
void get_process_info(int pid) {
    BOOL ret = FALSE;
    PNtQuerySystemInformation NtQuerySystemInformation = NULL;

    NtQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(LoadLibrary(_T("ntdll.dll")), "NtQuerySystemInformation");

    if (NtQuerySystemInformation != NULL) {
        PSYSTEM_PROCESS_INFORMATION sysProInfo = NULL, old = NULL;
        ULONG cbSize = sizeof(SYSTEM_PROCESS_INFORMATION);

        // 查询
        LONG status = 0;

        do {
            old = sysProInfo = (PSYSTEM_PROCESS_INFORMATION)malloc(cbSize);
            status = NtQuerySystemInformation(SystemProcessInformation, sysProInfo, cbSize, &cbSize);

            if (status) free(sysProInfo);
        } while (status);


        ret = TRUE;

        // 遍历进程
        do {
            if (sysProInfo->ImageName.Buffer != NULL)
            {
                _tprintf(L"进程名:\t%s \t进程ID:%u \t句柄总数:%u \t线程总数:%u \n", sysProInfo->ImageName.Buffer, sysProInfo->UniqueProcessId,
                         sysProInfo->HandleCount, sysProInfo->NumberOfThreads);

                // 打印线程信息
                PSYSTEM_THREAD_INFORMATION threadInfo = NULL;
                threadInfo = (PSYSTEM_THREAD_INFORMATION)((ULONG64)sysProInfo + sizeof(SYSTEM_PROCESS_INFORMATION));
                DWORD curThreadIndex = 1;

                do {
                    _tprintf(L"\t线程ID:%u\t起始地址:%x \t线程的状态码:%u\n", threadInfo->ClientId.UniqueThread, threadInfo->StartAddress, threadInfo->ThreadState);
                    threadInfo += 1;
                } while (curThreadIndex++ < sysProInfo->NumberOfThreads);
                _tprintf(L"\n");
            }

            // 指针的加减运算的单位是根据所指向数据类型大小的。字节指针就是1，所以加减运算没问题。这里是结构体指针，所以必须转成数字类型再运算。
            sysProInfo = (PSYSTEM_PROCESS_INFORMATION)((ULONG64)sysProInfo + sysProInfo->NextEntryOffset);
        } while (sysProInfo->NextEntryOffset != 0);
        free(old);
    }
}
}

#include "winapi.h"

#define DEBUG_STRING
#ifdef DEBUG_STRING
# include <stdio.h>
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
#ifdef DEBUG_STRING
            _tprintf(_T("进程名称 : %s \n"), process_info.szExeFile);
            _tprintf(_T("进程ID : %d \n"), process_info.th32ProcessID);
            _tprintf(_T("\n"));
#endif // DEBUG_STRING

            CloseHandle(snap);
            return process_info.th32ProcessID;
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
            _tprintf(_T("模块基址 : %8x \n"),    (unsigned int)module_info.modBaseAddr);
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

int find_pattern(handle process, struct module_information& module, const char *pattern, int index               , int offset)
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

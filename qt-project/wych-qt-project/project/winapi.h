#ifndef WINAPI_H
#define WINAPI_H


#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <tchar.h>
#include <vector>


using handle = HANDLE;
using hwnd = HWND;
using dword = DWORD;
using hmodule = HMODULE;
using hresult = HRESULT;

typedef struct _WIN32_PROCESS_INFO {
    // PWCHAR Buffer;
    DWORD PID;
    WCHAR PName[30];
    ULONG HandleCount;
    ULONG NumberOfThreads;
    LONG  BasePriority;
} WIN32_PROCESS_INFO, *PWIN32_PROCESS_INFO;


typedef struct _WIN32_WINDOW_INFO {
    // PWCHAR Buffer;
    DWORD PID;
    HWND  HandleWindow;
    HWND  HandleParentWindow;
    TCHAR ClassName[MAXBYTE];
    TCHAR TitleName[MAXBYTE];
    RECT  WindowRect{ 0, 0, 0, 0 };
    RECT  ClientRect{ 0, 0, 0, 0 };
    POINT ClientToScreen{ 0, 0 };
    BOOL  IsMaximized;
    BOOL  IsMinimized;
} WIN32_WINDOW_INFO, *PWIN32_WINDOW_INFO;

namespace WinAPI {
void get_process_info(int pid);
BOOL get_process_info(DWORD               pid,
                      PWIN32_PROCESS_INFO result);

BOOL get_window_main(DWORD              pid,
                     PWIN32_WINDOW_INFO result);

BOOL get_window_child(HWND                            handleWindow,
                      std::vector<WIN32_WINDOW_INFO>& list);

BOOL get_window_info(HWND               handle,
                     PWIN32_WINDOW_INFO result);
}

namespace WychUtils_WinAPI {
/// <summary>
/// 获取进程ID(通过进程名称)
/// </summary>
/// <param name="process_name">进程名称</param>
/// <returns>进程PID</returns>
dword get_process_id(LPCTSTR process_name);


/// <summary>
/// 获取进程句柄
/// </summary>
/// <param name="process_id">进程ID</param>
/// <param name="access">权限</param>
/// <returns>进程句柄</returns>
handle get_process_handle(dword process_id,
                          dword access = PROCESS_ALL_ACCESS);

/// <summary>
/// 获取模块dll信息
/// </summary>
/// <param name="process_handle">进程句柄</param>
/// <param name="process_id">进程ID</param>
/// <param name="module_name">模块名称</param>
/// <param name="info">模块信息</param>
void get_module_info(handle                     process_handle,
                     dword                      process_id,
                     LPCTSTR                    module_name,
                     struct module_information& info);

/// <summary>
/// 读取内存数据
/// </summary>
/// <param name="process">进程句柄</param>
/// <param name="address">内存地址</param>
/// <param name="recv">数据</param>
/// <param name="size">数据大小</param>
/// <returns>实际数据大小</returns>
dword read_memory(handle process,
                  int    address,
                  void  *recv,
                  int    size);

dword read_memory(handle       process,
                  unsigned int address,
                  void        *recv,
                  int          size);
dword write_memory(handle       process,
                   unsigned int address,
                   void        *data,
                   int          size);

/// <summary>
/// 写入内存数据
/// </summary>
/// <param name="process">进程句柄</param>
/// <param name="address">内存地址</param>
/// <param name="data">数据</param>
/// <param name="size">数据大小</param>
/// <returns>实际数据大小</returns>
dword write_memory(handle process,
                   int    address,
                   void  *data,
                   int    size);


// 下面是取得SKSE的共通

std::int32_t MessageBox(
    void        *a_wnd,
    const char  *a_text,
    const char  *a_caption,
    unsigned int a_type) noexcept;

std::int32_t MessageBox(
    void          *a_wnd,
    const wchar_t *a_text,
    const wchar_t *a_caption,
    unsigned int   a_type) noexcept;


void OutputDebugString(
    const char *a_outputString) noexcept;

void OutputDebugString(
    const wchar_t *a_outputString) noexcept;

void error(LPCTSTR text = nullptr);

void warning(LPCTSTR text);

struct module_information
{
    handle  module_handle;
    wchar_t module_name[1024];
    char   *module_data;
    int     module_address;
    int     module_size;
    void alloc(int size)
    {
        module_size = size;
        module_data = (char *)VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (module_data == 0) {
            error(L"申请内存失败");
        }
    }

    void release()
    {
        if (module_data) VirtualFree(module_data, 0, MEM_RELEASE);
        module_data = nullptr;
    }
};

int find_pattern(handle                     process,
                 struct module_information& module,
                 const char                *pattern,
                 int                        index = 0,
                 int                        offset = 0);
void show_all_pattern(handle                     process,
                      struct module_information& module,
                      const char                *pattern,
                      int                        offset = 0);
}

#endif // WINAPI_H

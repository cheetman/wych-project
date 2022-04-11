#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <d3d9types.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>

// #pragma comment(lib, "d3d9.lib")
// #pragma comment(lib, "d3dx9.lib")
// #pragma comment(lib, "dwmapi.lib")

static WNDCLASSEX wClass;
typedef void (*Draw)();

namespace WychUtils {
HWND CreateTopWindow(HWND  SourceHwnd,
                     void *WinProc);
bool InitD3D9(HWND                   hwnd,
              LPDIRECT3D9          & g_pD3D,
              LPDIRECT3DDEVICE9    & g_pd3dDevice,
              D3DPRESENT_PARAMETERS& g_d3dpp,
              LPD3DXLINE            *pLine,
              LPD3DXFONT            *Font);
class Window {};
}

#endif // WINDOW_H

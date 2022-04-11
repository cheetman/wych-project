#include "window.h"


namespace WychUtils {
bool InitD3D9(HWND hwnd, LPDIRECT3D9& g_pD3D, LPDIRECT3DDEVICE9& g_pd3dDevice, D3DPRESENT_PARAMETERS& g_d3dpp, LPD3DXLINE *pLine, LPD3DXFONT *Font) {
    /*
        D3D这玩意比较复杂，如果单纯是想搞点辅助什么的，复制粘贴我的足够了，
        如果想深入学习，可能得另找资料了，下面的这些基本是固定的，想知道是用来干啥的
        可以自行百度，我这个人比较懒。。。
     */

    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) return false;

    // 创建D3D设备
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0) return false;

    if (*pLine == NULL) D3DXCreateLine(g_pd3dDevice, pLine);

    // 创建D3D字体
    D3DXCreateFont(g_pd3dDevice, 16, 0, FW_DONTCARE, D3DX_DEFAULT, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Vernada", Font);
    return true;
}

HWND CreateTopWindow(HWND SourceHwnd, void *WinProc) {
    if ((WinProc == NULL) || (SourceHwnd == 0)) return 0;

    // 初始化窗口类
    wClass.cbClsExtra = NULL;
    wClass.cbSize = sizeof(WNDCLASSEX);
    wClass.cbWndExtra = NULL;
    wClass.hbrBackground = (HBRUSH)CreateSolidBrush(D3DCOLOR_ARGB(0, 0, 0, 0));
    wClass.hCursor = LoadCursor(0, IDC_ARROW);
    wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
    wClass.hInstance = GetModuleHandle(NULL);
    wClass.lpfnWndProc = (WNDPROC)WinProc;
    wClass.lpszClassName = L" ";
    wClass.lpszMenuName = L" ";
    wClass.style = CS_VREDRAW | CS_HREDRAW;

    // 注册窗口
    if (RegisterClassEx(&wClass) == 0)
    {
        MessageBox(NULL, L"创建窗口出错！", L"提示！", 0);
        exit(1);
    }

    RECT Rect;
    GetClientRect(SourceHwnd, &Rect);

    // 创建窗口
    HWND newHwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, L" ", L" ", WS_POPUP, 1, 1, Rect.right, Rect.bottom, 0, 0, 0, 0);

    // 显示窗口
    SetLayeredWindowAttributes(newHwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
    ShowWindow(newHwnd, SW_SHOW);
    return newHwnd;
}
}

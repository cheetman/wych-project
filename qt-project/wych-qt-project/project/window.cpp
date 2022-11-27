#include "window.h"

namespace WychUtils {
DX9::~DX9() {
    release();
}

DX9::DX9() {}

bool DX9::init(HWND hwnd) {
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

    if (pLine == NULL) D3DXCreateLine(g_pd3dDevice, &pLine);


    // 创建D3D字体
    D3DXCreateFont(g_pd3dDevice, 16, 0, FW_DONTCARE, D3DX_DEFAULT, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Vernada", &Font);
    return true;
}

void DX9::drawLine(D3DCOLOR Color, float X1, float Y1, float X2, float Y2, float Width) {
    D3DXVECTOR2 Vertex[2] = { { X1, Y1 }, { X2, Y2 } };

    pLine->SetWidth(Width);
    pLine->Draw(Vertex, 2, Color);
}

void DX9::drawRect(float X, float Y, float W, float H, float Width, D3DCOLOR Color) {
    D3DXVECTOR2 Vertex[5] = { { X, Y }, { X + W, Y }, { X + W, Y + H }, { X, Y + H }, { X, Y } };

    pLine->SetWidth(Width);
    pLine->Draw(Vertex, 5, Color);
}

void DX9::drawText(float X, float Y, const char *Str, D3DCOLOR Color) {
    RECT Rect = { (LONG)X, (LONG)Y };

    Font->DrawTextA(NULL, Str, -1, &Rect, DT_CALCRECT, Color);
    Font->DrawTextA(NULL, Str, -1, &Rect, DT_LEFT,     Color);
}

void DX9::drawStart() {
    g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
    g_pd3dDevice->BeginScene();
}

void DX9::drawEnd() {
    g_pd3dDevice->EndScene();
    g_pd3dDevice->Present(0, 0, 0, 0);
}

void DX9::release() {
    if (pLine) {
        pLine->Release();
        pLine = NULL;
    }

    if (Font) {
        Font->Release();
        Font = NULL;
    }

    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = NULL;
    }

    if (g_pD3D) {
        g_pD3D->Release();
        g_pD3D = NULL;
    }
}

static WNDCLASSEX wClass;

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
    // WS_EX_TOPMOST 该窗口应放置在所有非最顶层窗口上方，并且应保持其上方，即使窗口已停用也是如此。 若要添加或删除此样式，请使用 SetWindowPos 函数。
    // WS_EX_TRANSPARENT 在绘制同一线程) 创建的窗口下方的同级 (之前，不应绘制窗口。 窗口显示为透明，因为已绘制基础同级窗口的位。若要在不使用这些限制的情况下实现透明度，请使用 SetWindowRgn 函数。
    // WS_EX_LAYERED 窗口是分层 窗口。 如果窗口的 类样式 为 CS_OWNDC 或 CS_CLASSDC，则不能使用此样式。
    HWND newHwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, L" ", L" ", WS_POPUP, 1, 1, Rect.right, Rect.bottom, 0, 0, 0, 0);

    // 显示窗口
    SetLayeredWindowAttributes(newHwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
    ShowWindow(newHwnd, SW_SHOW);
    return newHwnd;
}

void CloseTopWindow(HWND newHwnd) {
    CloseWindow(newHwnd);
    DestroyWindow(newHwnd);
    UnregisterClass(wClass.lpszClassName, wClass.hInstance);
}
}

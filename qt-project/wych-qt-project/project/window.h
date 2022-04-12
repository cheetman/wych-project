#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <d3d9types.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>


// typedef void (*Draw)();

namespace WychUtils {
class DX9 {
public:

    explicit DX9();
    ~DX9();
    bool init(HWND hwnd);
    void drawLine(D3DCOLOR Color,
                  float    X1,
                  float    Y1,
                  float    X2,
                  float    Y2,
                  float    Width);
    void drawText(float       X,
                  float       Y,
                  const char *Str,
                  D3DCOLOR    Color);
    void drawRect(float    X,
                  float    Y,
                  float    W,
                  float    H,
                  float    Width,
                  D3DCOLOR Color);
    void drawStart();
    void drawEnd();
    void release();

    LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

private:

    LPDIRECT3D9 g_pD3D = NULL;
    D3DPRESENT_PARAMETERS g_d3dpp = {};
    ID3DXLine *pLine = 0;
    ID3DXFont *Font;
};


HWND CreateTopWindow(HWND  SourceHwnd,
                     void *WinProc);
void CloseTopWindow(HWND newHwnd);
}
#endif // WINDOW_H

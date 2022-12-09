#include "capturebtn.h"
#include <QDebug>
#include <QMouseEvent>
#include <stdio.h>
#include <windows.h>
#include "mainwindow.h"

CaptureBtn::CaptureBtn(QWidget *parent)
    : QPushButton{parent}
{}

CaptureBtn::CaptureBtn(const QString& text, QWidget *parent)
    : QPushButton{text, parent}
{
    //    this->setMouseTracking(true);
}

bool CaptureBtn::eraseWindowFrame(HWND hwnd) {
    if (hwnd) {
        RedrawWindow(hwnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
        HWND hparent = ::GetParent(hwnd);

        if (IsWindow(hparent)) {
            RedrawWindow(hparent, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
        }
    }
    return TRUE;
}

bool CaptureBtn::drawWindowFrame(HWND hwnd) {
    HDC  hdc = GetWindowDC(hwnd);
    RECT rect;

    GetWindowRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HPEN hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    HPEN holdpen = (HPEN)SelectObject(hdc, hpen);

    MoveToEx(hdc, 0, 0, NULL);
    LineTo(hdc, width, 0);
    LineTo(hdc, width, height);
    LineTo(hdc, 0,     height);
    LineTo(hdc, 0,     0);

    SelectObject(hdc, holdpen);
    DeleteObject(hpen);
    ReleaseDC(hwnd, hdc);
    return TRUE;
}

void CaptureBtn::mousePressEvent(QMouseEvent *event) {
    qDebug() << "按下";
    this->setMouseTracking(true);

    setCursor(Qt::CrossCursor);

    //    hide();

    //    ShowWindow((HWND)selfMainHwnd, SW_HIDE);


    //    HCURSOR hp;
    //    hp = (HCURSOR)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CROSS), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
    //    SetCursor(hp);
    //    SetCapture((HWND)selfMainHwnd);
}

void CaptureBtn::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "抬起";
    this->setMouseTracking(false);
    eraseWindowFrame(prehwnd);

    setCursor(Qt::ArrowCursor);

    //    ShowWindow((HWND)selfMainHwnd, SW_SHOW);

    //    HCURSOR hp;
    //    hp = (HCURSOR)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
    //    SetCursor(hp);
    //    ReleaseCapture();
}

void CaptureBtn::mouseMoveEvent(QMouseEvent *event) {
    //    int mouseX = event->x();
    //    int mouseY = event->y();


    //    qDebug() << mouseX << ":" << mouseY;

    POINT pos;

    GetCursorPos(&pos);


    //    qDebug() << pos.x << ":" << pos.y;

    HWND hwnd = ::WindowFromPoint(pos); // 找鼠标对应的窗口


    if (hwnd == prehwnd)                //不一直画相同窗口和自己
        return;
    else {
        //        qDebug() << "hwnd:" << hwnd;
    }

    if (hwnd ==  (HWND)selfMainHwnd) {
        eraseWindowFrame(prehwnd);
        prehwnd = NULL;
        return;
    }

    HWND prehwndtmp = prehwnd;
    prehwnd = hwnd;

    //    qDebug() << "句柄:" <<  g_mainwnd->selfMainHwnd;
    //    qDebug() << "句柄:" << selfMainHwnd;


    eraseWindowFrame(prehwndtmp);
    drawWindowFrame(hwnd);
    emit hwndEvent(hwnd);
}

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
    this->setMouseTracking(true);
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
        qDebug() << "hwnd:" << hwnd;
    }
    prehwnd = hwnd;

    //    qDebug() << "句柄:" <<  g_mainwnd->selfMainHwnd;
    //    qDebug() << "句柄:" << selfMainHwnd;

    if (prehwnd ==  (HWND)selfMainHwnd) {
        return;
    }
    emit hwndEvent(hwnd);
}

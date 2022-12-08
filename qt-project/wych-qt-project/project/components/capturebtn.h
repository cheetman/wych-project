#ifndef CAPTUREBTN_H
#define CAPTUREBTN_H

#include <QObject>
#include <QPushButton>

class CaptureBtn : public QPushButton {
    Q_OBJECT

public:

    explicit CaptureBtn(QWidget *parent = nullptr);
    explicit CaptureBtn(const QString& text,
                        QWidget       *parent = nullptr);


    void mouseMoveEvent(QMouseEvent *event); //鼠标移动事件
    HWND prehwnd;

signals:

    void hwndEvent(HWND hwnd);
};

#endif // CAPTUREBTN_H

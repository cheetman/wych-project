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


    void mouseMoveEvent(QMouseEvent *);    //鼠标移动事件
    void mousePressEvent(QMouseEvent *);   //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *); //鼠标释放事件
    HWND prehwnd;

private:

    bool drawWindowFrame(HWND);
    bool eraseWindowFrame(HWND);

signals:

    void hwndEvent(HWND);
};

#endif // CAPTUREBTN_H

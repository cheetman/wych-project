#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

#include <QWidget>

class PixmapWidget : public QWidget {
    Q_OBJECT

public:

    explicit PixmapWidget(QWidget *parent = nullptr);
    void   setPixmap(QPixmap& newPixmap);
    QSize  getPixmapSize();
    QColor getRgb(int x,
                  int y);

protected:

    void paintEvent(QPaintEvent *event);      // 绘制事件
    void mousePressEvent(QMouseEvent *event); //鼠标点击事件
    void mouseMoveEvent(QMouseEvent *event);  //鼠标移动事件
    void enterEvent(QEvent *event);           //鼠标进入窗口事件
    void leaveEvent(QEvent *event);           //鼠标离开窗口事件

private:

    QPixmap m_pixmap;
    QSize m_pixmapSize;
    QImage m_image;
    QBrush m_brush;    //
    QColor m_penColor; // 坐标，追踪十字的颜色
    int mouseX, mouseY;
    int m_scale = 1;
    typedef enum { Border, Tracking } PaintMode;
    PaintMode m_paintMode;

signals:

    void mouseClicked(int     x,
                      int     y,
                      QColor& rgb,
                      float   xr,
                      float   yr);
    void mousePositionEvent(int     x,
                            int     y,
                            QColor& rgb,
                            float   xr,
                            float   yr);
};

#endif // PIXMAPWIDGET_H

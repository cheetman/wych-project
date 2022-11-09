#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

#include <QWidget>

class PixmapWidget : public QWidget {
    Q_OBJECT

public:

    explicit PixmapWidget(QWidget *parent = nullptr);
    void setPixmap(QPixmap& newPixmap);

protected:

    void paintEvent(QPaintEvent *event); // 绘制事件

private:

    QPixmap m_pixmap;
    QBrush m_brush;    //
    QColor m_penColor; // 坐标，追踪十字的颜色

signals:
};

#endif // PIXMAPWIDGET_H

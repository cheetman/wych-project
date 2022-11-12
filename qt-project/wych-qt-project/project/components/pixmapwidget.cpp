#include "pixmapwidget.h"
#include <QMouseEvent>
#include <QPainter>

PixmapWidget::PixmapWidget(QWidget *parent)
    : QWidget{parent}
{
    //    this->setBackgroundRole(QPalette::Mid);


    m_brush = QBrush(Qt::white);

    m_penColor.setRgb(0, 255, 255, 255);

    m_pixmap = QPixmap(700, 700);
    m_pixmap.fill(Qt::red);

    this->setFixedWidth(800);
    this->setFixedHeight(800);

    // m_pixmap.load("C:\\Users\\汪意超\\Pictures\\Saved Pictures\\001OdAkagy1gyybblipk3j60h00fvajh02.jpg");

    //    setMinimumSize(QSize(800, 800));

    // m_pixmap = QPixmap("C:\\Users\\汪意超\\Pictures\\Saved Pictures\\001OdAkagy1gyybblipk3j60h00fvajh02.jpg");

    //    m_penColor =

    //    this->update();
}

void PixmapWidget::setPixmap(QPixmap& newPixmap) {
    QSize size = newPixmap.size();

    m_pixmap.swap(newPixmap);
    this->setFixedSize(size);

    //    this->resize(size); // 没用，估计是调用了resize事件

    //    m_pixmap = newPixmap;
    //    this->update(); setFixedSize后就不需要了 还是需要
    this->update();
}

void PixmapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);


    QPainter painter(this);

    //    auto     ff =   QColor::red();
    painter.setPen(m_penColor);
    painter.setBackground(m_brush);

    painter.eraseRect(rect());
    painter.drawPixmap(0, 0, m_pixmap);

    switch (m_paintMode) {
    case Border:
    {
        QVector<QLine> lines;
        int xMax = painter.window().width() - 1;
        int yMax = painter.window().height() - 1;
        QPoint p1(0, 0), p2(xMax, 0), p3(xMax, yMax), p4(0, yMax);
        QLine  line1(p1, p2);
        QLine  line2(p2, p3);
        QLine  line3(p3, p4);
        QLine  line4(p4, p1);
        lines.append(line1);
        lines.append(line2);
        lines.append(line3);
        lines.append(line4);
        painter.drawLines(lines);
        break;
    }

    case Tracking:
    {
        QVector<QLine> lines;
        int xMax = painter.window().width() - 1;
        int yMax = painter.window().height() - 1;
        QPoint p1(mouseX, 0), p2(mouseX, yMax), p3(0, mouseY), p4(xMax, mouseY);
        QLine  line1(p1, p2);
        QLine  line2(p3, p4);
        lines.append(line1);
        lines.append(line2);
        painter.drawLines(lines);
        break;
    }
    }
}

void PixmapWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    this->setMouseTracking(true);
    this->setFocus(); // 方向键的监听需要调用此函数
    // this->activateWindow();
}

void PixmapWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    this->setMouseTracking(false);

    if (!m_pixmap.isNull()) {
        m_paintMode = Border;
        this->update();
    }
}

void PixmapWidget::mousePressEvent(QMouseEvent *event) {
    //    if(m_pixmap.isNull()){
    //        return;
    //    }
    //    this->setMouseTracking(false); //关闭鼠标追踪
    //    x = event->x();
    //    y = event->y();
    //    if(event->button() == Qt::LeftButton){ //鼠标左键被点击
    //        //存储当被点击的点
    //        m_points.append(QPoint(x/m_scale,y/m_scale));
    //        //绘制定位十字
    //        m_pixmap = drawLocationCross(m_points,m_scale);
    //        //绘制追踪十字
    //        m_paintMode = Tracking;
    //        this->update();
    //    }
    //    else if(event->button() == Qt::RightButton){ //右键
    //        //删除最后一个点
    //        if(m_points.size()>0){
    //            m_points.removeLast();
    //        }
    //        m_pixmap = drawLocationCross(m_points,m_scale);
    //        m_paintMode = Tracking;
    //        this->update();
    //    }
    //    else if(event->button() ==Qt::MiddleButton){
    //        this->activateWindow(); //中键激活窗口
    //    }
    //    this->setMouseTracking(true);
    //    emit clicked(m_points); //发送被点击的信号
    //    emit clicked(x/m_scale,y/m_scale);
}

void PixmapWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_pixmap.isNull()) {
        return;
    }
    mouseX = event->x();
    mouseY = event->y();

    // 绘制定位十字
    m_paintMode = Tracking;
    this->update();

    emit mousePositionEvent(mouseX / m_scale, mouseY / m_scale); // send position
}

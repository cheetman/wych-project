#include "pixmapwidget.h"
#include "qtimer.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

PixmapWidget::PixmapWidget(QWidget *parent)
    : QWidget{parent}
{
    //    this->setBackgroundRole(QPalette::Mid);


    m_brush = QBrush(Qt::white);
    m_penColor.setRgb(0, 255, 255, 255);

    m_pixmap = QPixmap(700, 700);
    m_pixmap.fill(Qt::gray);


    m_pixmapSize = m_pixmap.size();
    this->setFixedWidth(800);
    this->setFixedHeight(800);

    QImage image = m_pixmap.toImage();
    m_image.swap(image);
}

QSize PixmapWidget::getPixmapSize() {
    return m_pixmapSize;
}

void PixmapWidget::setPixmap(QPixmap& newPixmap) {
    QSize  size = newPixmap.size();
    QImage image = newPixmap.toImage();

    m_image.swap(image);
    m_pixmap.swap(newPixmap);

    // 子线程会报错，用sign也不行。。
    QTimer::singleShot(0, this, [this, size]()
    {
        this->setFixedSize(size);
    });

    //    this->setFixedSize(size);
    m_pixmapSize = size;

    //    this->resize(size); // 没用，估计是调用了resize事件
    this->update(); //   setFixedSize后就不需要了 还是需要,否则可能只有部分刷新。。。
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
    case Rect: {
        QVector<QLine> lines;
        QPoint p1(mouseXBegin, mouseYBegin), p2(mouseXBegin, mouseYEnd), p3(mouseXEnd, mouseYEnd), p4(mouseXEnd, mouseYBegin);
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

void PixmapWidget::mouseReleaseEvent(QMouseEvent *event) {
    mousePressed = false;
}

void PixmapWidget::mousePressEvent(QMouseEvent *event) {
    if (m_pixmap.isNull()) {
        return;
    }

    mousePressed = true;


    this->setMouseTracking(false); // 关闭鼠标追踪
    mouseYBegin =  mouseXBegin = mouseX = event->x();
    mouseYEnd = mouseYBegin = mouseY = event->y();
    QColor color = m_image.pixel(mouseX, mouseY);

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
    this->setMouseTracking(true);

    //    emit clicked(m_points); //发送被点击的信号
    emit mouseClicked(mouseX / m_scale, mouseY / m_scale, color,    (float)mouseX / (float)m_pixmapSize.width(), (float)mouseY / (float)m_pixmapSize.height());
}

void PixmapWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_pixmap.isNull()) {
        return;
    }

    mouseX = event->x();
    mouseY = event->y();

    if (mousePressed) {
        m_paintMode = Rect;
        mouseYEnd = mouseY;
        mouseXEnd = mouseX;
    } else {
        m_paintMode = Tracking;
    }
    this->update();

    if (!m_image.valid(mouseX, mouseY)) {
        qDebug() << "invalid:" << mouseX  << ":" << mouseY;
    }
    QColor color = m_image.pixel(mouseX, mouseY);


    // m_pixmap.copy()

    // m_image.copy()

    // m_pixmap.save()

    // QTransform trans;
    // trans.
    // m_pixmap.transformed()

    emit mousePositionEvent(mouseX / m_scale, mouseY / m_scale, color,    (float)mouseX / (float)m_pixmapSize.width(), (float)mouseY / (float)m_pixmapSize.height());
}

QColor PixmapWidget::getRgb(int x, int y) {
    return m_image.pixel(x, y);
}

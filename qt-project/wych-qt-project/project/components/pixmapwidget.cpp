#include "pixmapwidget.h"
#include <QPainter>

PixmapWidget::PixmapWidget(QWidget *parent)
    : QWidget{parent}
{
    //    this->setBackgroundRole(QPalette::Mid);


    m_brush = QBrush(Qt::white);


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
    //    this->update(); setFixedSize后就不需要了
}

void PixmapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);


    QPainter painter(this);

    //    painter.setPen(m_penColor);
    painter.setBackground(m_brush);

    painter.eraseRect(rect());
    painter.drawPixmap(0, 0, m_pixmap);
}

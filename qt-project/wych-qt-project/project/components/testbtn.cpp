#include "testbtn.h"
#include <QDebug>
#include <QMouseEvent>

testBtn::testBtn(QWidget *parent)
    : QPushButton{parent}
{}

testBtn::testBtn(const QString& text, QWidget *parent)
    : QPushButton{text, parent}
{}


void testBtn::mouseMoveEvent(QMouseEvent *event) {
    int mouseX = event->x();
    int mouseY = event->y();

    qDebug() << mouseX << ":" << mouseY;
}

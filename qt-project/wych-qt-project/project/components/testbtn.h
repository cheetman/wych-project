#ifndef TESTBTN_H
#define TESTBTN_H

#include <QObject>
#include <QPushButton>

class testBtn : public QPushButton {
    Q_OBJECT

public:

    explicit testBtn(QWidget *parent = nullptr);
    explicit testBtn(const QString& text,
                     QWidget       *parent = nullptr);


    void mouseMoveEvent(QMouseEvent *event); //鼠标移动事件

signals:
};

#endif // TESTBTN_H

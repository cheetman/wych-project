#ifndef ITEMVIEW7VLCPLAYER_H
#define ITEMVIEW7VLCPLAYER_H

#include "vlclib.h"

#include <QDialog>
#include <QSlider>
#include <QWidget>
#include <QGroupBox>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTime>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>

class ItemView7VlcPlayer : public QDialog
{
    Q_OBJECT
public:
    explicit ItemView7VlcPlayer(QWidget *parent = nullptr);

private:
    VlcLib *player;
    QWidget *playWidget;
    QSlider *sliderPosition;
    QSlider *sliderVolume;
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;

    void closeEvent(QCloseEvent *event);
protected:
//    bool eventFilter(QObject *watched, QEvent *event);

public slots:
   void getVideoData(QString url);
signals:
   void closeDialog();

};

#endif // ITEMVIEW7VLCPLAYER_H

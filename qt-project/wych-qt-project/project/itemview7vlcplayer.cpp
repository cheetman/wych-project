#include "itemview7vlcplayer.h"
#include "utils.h"

#include <QTextCodec>
#include <QCloseEvent>
#include <QThreadPool>
#include <QFuture>
#include <QtConcurrent>


ItemView7VlcPlayer::ItemView7VlcPlayer(QWidget *parent) : QDialog(parent)
{
    playWidget = new QWidget(this);
    auto layout = new QVBoxLayout(this);
    playWidget->setMinimumSize(QSize(200,200));


    /* 按钮区域 */
    auto btnsGroupBox = new QGroupBox(this);
    btnsGroupBox->setFixedHeight(60);
    auto btnslayout = new QHBoxLayout(this);
    btnsGroupBox->setLayout(btnslayout);
    auto btnTest = new QPushButton(tr("打开测试文件"),this);
    auto btnStop = new QPushButton(tr("停止"),this);
    auto btnPlay = new QPushButton(tr("播放/暂停"),this);
    auto btnTest2 = new QPushButton(tr("显示位置"),this);
    auto btnTest3 = new QPushButton(tr("测试位置"),this);
    sliderVolume = new QSlider(Qt::Horizontal,this);
    sliderVolume->setFixedWidth(80);
    sliderVolume->installEventFilter(this);
    btnslayout->addWidget(btnTest);
    btnslayout->addWidget(btnStop);
    btnslayout->addWidget(btnPlay);
    btnslayout->addWidget(btnTest2);
    btnslayout->addWidget(btnTest3);
    btnslayout->addWidget(sliderVolume);


    /* 进度条区域 */
    auto sliderGroupBox = new QGroupBox(this);
    sliderGroupBox->setFixedHeight(60);
    auto sliderlayout = new QHBoxLayout(this);
    sliderGroupBox->setLayout(sliderlayout);
    sliderPosition = new QSlider(Qt::Horizontal,this);
    sliderPosition->installEventFilter(this);
    lineEdit1 = new QLineEdit(this);
    lineEdit2 = new QLineEdit(this);
    sliderlayout->addWidget(lineEdit1);
    sliderlayout->addWidget(sliderPosition);
    sliderlayout->addWidget(lineEdit2);
    lineEdit1->setFrame(false);
    lineEdit2->setFrame(false);
    lineEdit1->setReadOnly(true);
    lineEdit2->setReadOnly(true);
    lineEdit1-> setStyleSheet("background:transparent;border-width:0;border-style:outset");
    lineEdit2-> setStyleSheet("background:transparent;border-width:0;border-style:outset");

    lineEdit1->setFixedWidth(50);
    lineEdit2->setFixedWidth(50);


      layout->addWidget(playWidget);
    layout->addWidget(btnsGroupBox);
    layout->addWidget(sliderGroupBox);
    setLayout(layout);
    player = new VlcLib(playWidget);



    connect(btnTest,&QPushButton::clicked,[=](){
        player->playFile(QString("http://xxx:xxx@xxx:111/video/中文测试.mkv").toStdString().data(), (HWND)playWidget->winId());
    });

    connect(btnPlay,&QPushButton::clicked,[=](){
        player->play();
    });
    connect(btnStop,&QPushButton::clicked,[=](){

//        QtConcurrent::run([player]()
//            {
//             player->stop();

//            });


//        if (playWidget != nullptr)
//          {
//            delete playWidget;
//            playWidget = nullptr;
//          }
            player->stop();

    });
    connect(btnTest2,&QPushButton::clicked,[=](){
       qDebug() << "播放位置："<< player->getPosition();
    });
    connect(btnTest3,&QPushButton::clicked,[=](){
        auto position = player->getPosition();
        position += 0.01f;
        player->setPosition(position);
    });



    QTimer * timer = new QTimer(this);
    timer->start(100);
    connect(timer, &QTimer::timeout,[=](){
        QTime n(0, 0, 0), cur, tol;
        float position = player->getPosition();
        int volume = player->getVolume();
        libvlc_time_t curtime = player->getTime();
        libvlc_time_t toltime = player->getLength();
        cur = n.fromMSecsSinceStartOfDay(curtime);
        tol = n.fromMSecsSinceStartOfDay(toltime);
        if (toltime > 3600000)
        {
        lineEdit1->setText(cur.toString("hh:mm:ss"));
        lineEdit2->setText(tol.toString("hh:mm:ss"));
        }
        else if (toltime > 60000)
        {
            lineEdit1->setText(cur.toString("mm:ss"));
            lineEdit2->setText(tol.toString("mm:ss"));
        }
        else if (toltime > 1000)
        {
            lineEdit1->setText(cur.toString("ss"));
            lineEdit2->setText(tol.toString("ss"));
        }
        else if (toltime > 0)
        {
            lineEdit1->setText(cur.toString("zzz"));
            lineEdit2->setText(tol.toString("zzz"));
        }
        sliderPosition->setValue(position * sliderPosition->maximum());
        // lineEdit1_3->setText(QString::number(volume) + "%");
        sliderVolume->setValue(sliderVolume->maximum() * volume / 100.0);
        if (player->isEnd())
        {
            player->stop();
            sliderPosition->setValue(0);
            lineEdit1->clear();
        }
    });

}


// 重写closeEvent: 确认退出对话框
void ItemView7VlcPlayer::closeEvent(QCloseEvent *event)
{
     player->stop();
     emit closeDialog();
     event->accept();
//    QMessageBox::StandardButton button;
//    button=QMessageBox::question(this,tr("退出程序"),QString(tr("确认退出程序")),QMessageBox::Yes|QMessageBox::No);
//    if(button==QMessageBox::No)
//    {
//        event->ignore(); // 忽略退出信号，程序继续进行
//    }
//    else if(button==QMessageBox::Yes)
//    {
//        event->accept(); // 接受退出信号，程序退出
//    }
}

void ItemView7VlcPlayer::getVideoData(QString url){

    qDebug() << "准备播放：" << url;
    //player->stop();
    player->playFile(url.toStdString().data(), (HWND)playWidget->winId());
}



bool ItemView7VlcPlayer::eventFilter(QObject *watched, QEvent *event)
{

    if (event->type() == QEvent::MouseButtonPress)
    {
        if (watched == sliderPosition)
        {
            QMouseEvent * mouset = (QMouseEvent *)event;
            if (mouset->button() == Qt::LeftButton)
            {
                double pro = (double)mouset->x() /sliderPosition->width();
                double pos = pro * sliderPosition->maximum();
                sliderPosition->setValue(pos);
                player->setPosition(pro);
                return true;
            }
        }
        else if (watched == sliderVolume)
        {
            QMouseEvent * mouset = (QMouseEvent *)event;
            if (mouset->button() == Qt::LeftButton)
            {
                double pro = (double)mouset->x() /sliderVolume->width();
                double pos = pro * sliderVolume->maximum();
               sliderVolume->setValue(pos);
                player->setVolume(pro * 101);
                return true;
            }
        }
    }
    return false;
}

//ItemView7VlcPlayer::~ItemView7VlcPlayer(){
//    qDebug() << "析构函数";
//}



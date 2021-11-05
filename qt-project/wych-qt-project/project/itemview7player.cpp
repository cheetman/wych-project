#include "itemview7player.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

ItemView7Player::ItemView7Player(QWidget *parent) : QDialog(parent)
{

    auto mainLayout = new QVBoxLayout(this);
    auto videoWidget = new QVideoWidget();
    auto playlist = new QMediaPlaylist();
    videoWidget->resize(600,300);
    /*本地读取*/
//    playlist->addMedia(QMediaContent(QUrl::fromLocalFile("F:/1.mp4")));
//    playlist->addMedia(QUrl("qrc:/static/1.mp4"));
//    playlist->addMedia(QUrl("F:/1.mp4"));
    /*网络读取*/
    playlist->addMedia(QMediaContent(QUrl("https://v-cdn.zjol.com.cn/280443.mp4")));
    playlist->addMedia(QMediaContent(QUrl("https://v-cdn.zjol.com.cn/276982.mp4")));
    playlist->setCurrentIndex(1);

    auto player = new QMediaPlayer(this);
    player->setPlaylist(playlist);
    player->setVideoOutput(videoWidget);
    player->play();


    mainLayout->addWidget(videoWidget);
    setLayout(mainLayout);

}

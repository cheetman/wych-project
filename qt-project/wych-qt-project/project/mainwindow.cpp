
#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // 读取配置
//    auto settings = new QSettings("configs/setting.ini", QSettings::IniFormat);
//    settings->beginGroup("QT");
//    MainWindow::FtpFavorDirList = settings->value("ftpFavorDirList","D:\\ftpFavorDirList.json").toString();
//    settings->endGroup();



    auto tabWidget = new QTabWidget(this);
    tabWidget->addTab(new ItemView8(this), tr("6.视频播放器(WebDav)"));
    tabWidget->addTab(new ItemView7(this), tr("7.视频播放器(FTP)"));
    tabWidget->addTab(new ItemView9(this), tr("9.游戏辅助测试"));


    QWidget* widget = new QWidget(this);
    auto mainLayout = new QVBoxLayout(widget);
    mainLayout->addWidget(tabWidget);
    this->setCentralWidget(widget);
}

MainWindow::~MainWindow()
{



}


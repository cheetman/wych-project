
#include "mainwindow.h"
#include "events/customevent.h"
#include "itemview10.h"

#include <ItemView11.h>

SINGLETON_IMPL(MainWindow)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 读取配置
    //    auto settings = new QSettings("configs/setting.ini", QSettings::IniFormat);
    //    settings->beginGroup("QT");
    //    MainWindow::FtpFavorDirList = settings->value("ftpFavorDirList","D:\\ftpFavorDirList.json").toString();
    //    settings->endGroup();


    auto tabWidget = new QTabWidget(this);


    QMessageBox::StandardButton button = QMessageBox::question(this, tr("提示"), QString(tr("脚本？")), QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
    {
        tabWidget->addTab(new ItemView10(this), tr("10.底层测试"));
    } else {
        tabWidget->addTab(new ItemView8(this),  tr("6.视频播放器(WebDav)"));

        //    tabWidget->addTab(new ItemView7(this), tr("7.视频播放器(FTP)"));
        tabWidget->addTab(new ItemView9(this),  tr("9.游戏辅助测试"));
        tabWidget->addTab(new ItemView10(this), tr("10.底层测试"));
        tabWidget->addTab(new ItemView11(this), tr("11.安卓底层测试"));
    }


    QWidget *widget = new QWidget(this);
    auto     mainLayout = new QVBoxLayout(widget);
    mainLayout->addWidget(tabWidget);
    this->setCentralWidget(widget);
    setWindowTitle(tr("CheatmanTools v0.5"));
}

MainWindow::~MainWindow()
{}

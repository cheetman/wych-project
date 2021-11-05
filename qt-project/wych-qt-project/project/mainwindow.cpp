
#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{



    auto tabWidget = new QTabWidget;
    tabWidget->addTab(new ItemView8, tr("6.视频播放器(WebDav)"));
    tabWidget->addTab(new ItemView7, tr("7.视频播放器(FTP)"));


    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    QWidget* widget = new QWidget(this);
    widget->setLayout(mainLayout);
    this->setCentralWidget(widget);
}

MainWindow::~MainWindow()
{



}


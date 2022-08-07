
#include "mainwindow.h"
#include "customevent.h"
#include "itemview10.h"

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

    tabWidget->addTab(new ItemView8(this),  tr("6.视频播放器(WebDav)"));

    //    tabWidget->addTab(new ItemView7(this), tr("7.视频播放器(FTP)"));
    tabWidget->addTab(new ItemView9(this),  tr("9.游戏辅助测试"));
    tabWidget->addTab(new ItemView10(this), tr("10.底层测试"));


    QWidget *widget = new QWidget(this);
    auto     mainLayout = new QVBoxLayout(widget);
    mainLayout->addWidget(tabWidget);
    this->setCentralWidget(widget);
}

MainWindow::~MainWindow()
{}

void MainWindow::postMessage(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg);

    QApplication::postEvent(this, event);
}

void MainWindow::appendMessage(const QString& msg)
{
    //    QString text = recvmsg->toPlainText();
    //    text += QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz] ");
    //    text += msg;
    //    if (text.back() != '\n') {
    //        text += "\n";
    //    }
    //    showMessage(text);
}

void MainWindow::showMessage(const QString& msg)
{
    //    recvmsg->setText(msg);
    //    QTextCursor cursor = recvmsg->textCursor();
    //    cursor.movePosition(QTextCursor::End);
    //    recvmsg->setTextCursor(cursor);
    //    recvmsg->repaint();
}


#include "mainwindow.h"
#include "events/customevent.h"
#include "qftp.h"
#include "itemview7.h"
#include "itemview8.h"
#include "itemview9.h"
#include "itemview10.h"
#include "itemview11.h"
#include "events/eventwinmessage.h"
#include "itemview10ProcessStatus.h"


int selfMainHwnd = 0;

SINGLETON_IMPL(MainWindow)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    selfMainHwnd = this->winId();

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
        itemView10 = new ItemView10(this);
        tabWidget->addTab(itemView10,           tr("10.底层测试"));
        tabWidget->addTab(new ItemView11(this), tr("11.安卓底层测试"));
    }


    QWidget *widget = new QWidget(this);
    auto     mainLayout = new QVBoxLayout(widget);
    mainLayout->addWidget(tabWidget);
    this->setCentralWidget(widget);
    setWindowTitle(tr("CheatmanTools v0.5"));


    auto g_MainWnd = FindWindow(NULL, L"AssemblyTestWindows");

    if (g_MainWnd) {
        // 鼠标
        if (!ChangeWindowMessageFilterEx((HWND)selfMainHwnd, WM_USER + 0x101, MSGFLT_ALLOW, NULL)) {
            int err = GetLastError();
            qDebug() << "GetLastError：" << err;
        }
    }
}

MainWindow::~MainWindow()
{}


bool MainWindow::nativeEvent(const QByteArray& eventType,
                             void             *message,
                             long             *result) {
    if (eventType == "windows_generic_MSG") // windows平台
    {
        MSG *msg = reinterpret_cast<MSG *>(message);

        if (msg->message > WM_USER + 0x100) // 消息类型
        {
            WPARAM wParam = msg->wParam;
            LPARAM lParam = msg->lParam;


            //            LPMOUSEHOOKSTRUCT lParam2 = (LPMOUSEHOOKSTRUCT)lParam;


            //            qDebug() << "A x" << lParam2->pt.x << "y" <<  lParam2->pt.y;
            //            qDebug() << "B x" << LOWORD(lParam) << "y" <<  HIWORD(lParam);


            //            qDebug() << "message:0x" << QString::number(msg->message, 16) << " wParam:0x" << QString::number(wParam, 16) << " lParam:0x" <<
            // QString::number(lParam, 16);

            switch (msg->message) {
            // 鼠标
            case WM_USER + 0x101: {
                EventWinMessage *event = new EventWinMessage(qEventMouseProc, wParam, lParam);

                // 这么调用居然不行
                // QApplication::postEvent(itemView10->itemview10ProcessStatus, event);

                if (g_itemview10ProcessStatus) {
                    QApplication::postEvent(g_itemview10ProcessStatus, event);
                }

                // 处理完成
                return true;

                break;
            }
            }
        }
    }


    return QWidget::nativeEvent(eventType, message, result); // 交给Qt处理
}

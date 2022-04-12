#ifndef ITEMVIEW9CS16_H
#define ITEMVIEW9CS16_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QTextEdit>
#include <QApplication>
#include <QStandardItemModel>
#include <QTableView>
#include <QPlainTextEdit>
#include <QCheckBox>
#include "hv.h"
#include "evpp/TcpServer.h"
#include <d3d9types.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>
#include "window.h"


class ItemView9CS16 : public QWidget {
    Q_OBJECT

public:

    explicit ItemView9CS16(QWidget *parent = nullptr);
    ~ItemView9CS16();

    bool start(int         port,
               const char *host = "0.0.0.0");
    void stop();

    HWND newHwnd, gameHwnd;
    RECT gameRect;
    POINT gamePoint{ 0, 0 };
    WychUtils::DX9 *dx;
    MARGINS Margin;

    bool isStart = false;

    //    static ItemView9CS16 *instance;

protected:

    void                      initUI();
    void                      initConnect();
    void                      postMessage(const QString& msg);
    void                      clearMessage();

    void                      appendMessage(const QString& msg);
    void                      showMessage(const QString& msg);
    virtual void              customEvent(QEvent *e);

    static unsigned __stdcall Start(void *param);

    static LRESULT            WinProc(HWND   hWnd,
                                      UINT   Message,
                                      WPARAM wParam,
                                      LPARAM lParam);

private:

    QLineEdit *edtHost;
    QLineEdit *edtPort;
    QPushButton *btnStartStop;
    QPushButton *btnClients;
    QPushButton *btnConsoleClear;
    QCheckBox *ckConsoleEnable;
    QCheckBox *ckRefreshClients;

    QPlainTextEdit *edtMsg;
    hv::TcpServer *server;

    QStandardItemModel *infoGridModel;
    QTableView *infoTableView;

signals:
};


// #define g_cs16 ItemView9CS16::instance()


#endif // ITEMVIEW9CS16_H

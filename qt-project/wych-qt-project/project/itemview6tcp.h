#ifndef ITEMVIEW6TCP_H
#define ITEMVIEW6TCP_H

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

class ItemView6Tcp : public QWidget {
    Q_OBJECT

public:

    explicit ItemView6Tcp(QWidget *parent = nullptr);
    ~ItemView6Tcp();

    bool start(int         port,
               const char *host = "0.0.0.0");
    void stop();

protected:

    void         initUI();
    void         initConnect();
    void         postMessage(const QString& msg);
    void         clearConsole();

    void         appendConsole(const QString& msg);
    void         writeConsole(const QString& msg);
    virtual void customEvent(QEvent *e);

private:

    QLineEdit *edtHost;
    QLineEdit *edtPort;
    QPushButton *btnStartStop;
    QPushButton *btnClients;
    QPushButton *btnConsoleClear;
    QCheckBox *ckConsoleEnable;
    QCheckBox *ckRefreshClients;

    QPlainTextEdit *edtMsg;
    BOOL isStart = false;
    hv::TcpServer *server;

    QStandardItemModel *infoGridModel;
    QTableView *infoTableView;

signals:
};

#endif // ITEMVIEW6TCP_H

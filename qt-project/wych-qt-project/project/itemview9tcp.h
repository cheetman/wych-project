#ifndef ITEMVIEW9TCP_H
#define ITEMVIEW9TCP_H

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

class ItemView9Tcp : public QWidget
{
    Q_OBJECT
public:
    explicit ItemView9Tcp(QWidget *parent = nullptr);
    ~ItemView9Tcp();

    bool start(int port, const char* host = "0.0.0.0");
    void stop();

protected:
    void initUI();
    void initConnect();
    void postMessage(const QString& msg);
    void clearMessage();

    void appendMessage(const QString& msg);
    void showMessage(const QString& msg);
    virtual void customEvent(QEvent* e);

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

#endif // ITEMVIEW9TCP_H

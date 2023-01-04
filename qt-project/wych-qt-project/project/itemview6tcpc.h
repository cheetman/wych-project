#ifndef ITEMVIEW6TCPC_H
#define ITEMVIEW6TCPC_H

#include <QWidget>

class ItemView6TcpC : public QWidget {
    Q_OBJECT

public:

    explicit ItemView6TcpC(QWidget *parent = nullptr);
    ~ItemView6TcpC();

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

    class QLineEdit *edtHost;
    class QLineEdit *edtPort;
    class QPushButton *btnStartStop;
    class QPushButton *btnClients;
    class QPushButton *btnConsoleClear;
    class QCheckBox *ckConsoleEnable;
    class QCheckBox *ckRefreshClients;

    class QPlainTextEdit *edtMsg;
    bool isStart = false;

    class QStandardItemModel *infoGridModel;
    class QTableView *infoTableView;

signals:
};

#endif // ITEMVIEW6TCPC_H

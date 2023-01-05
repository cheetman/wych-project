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

    void postAppendConsole(const QString& msg);
    class QLineEdit *edtHost;
    class QLineEdit *edtPort;

protected:

    void         initUI();
    void         initConnect();
    void         clearConsole();

    void         writeConsole(const QString& msg);
    virtual void customEvent(QEvent *e);
    void         appendConsole(const QString& msg);

private:

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

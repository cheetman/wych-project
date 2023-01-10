#ifndef ITEMVIEW6TCPC_H
#define ITEMVIEW6TCPC_H

#include <QWidget>

class ItemView6TcpC : public QWidget {
    Q_OBJECT

public:

    explicit ItemView6TcpC(QWidget *parent = nullptr);
    ~ItemView6TcpC();

    void postAppendConsole(const QString& msg);
    void postAppendRecvMsg(const QString& msg);

    class QLineEdit *edtHost;
    class QLineEdit *edtPort;

    class QComboBox *cbCharEncoding;
    class QComboBox *cbCharEncodingS;

protected:

    void         clientAdd(int);
    void         clientRemove(int);


    void         initUI();
    void         initConnect();
    void         clearConsole();
    void         clearRecvMsg();
    void         clearSendMsg();

    void         writeConsole(const QString&);
    virtual void customEvent(QEvent *);
    void         appendConsole(const QString&);
    void         appendRecvMsg(const QString&);

private:

    int currentSocketId = 0;

    class QPushButton *btnStartStop;

    //    class QPushButton *btnClients;
    class QPushButton *btnConsoleClear;
    class QPushButton *btnRecvClear;
    class QPushButton *btnSend;
    class QCheckBox *ckConsoleEnable;

    //    class QCheckBox *ckRefreshClients;

    class QPlainTextEdit *edtMsg;
    class QPlainTextEdit *edtSend;
    class QPlainTextEdit *edtRecv;


    class QStandardItemModel *infoGridModel;
    class QTableView *infoTableView;
    class QStandardItemModel *optionGridModel;
    class QTableView *optionTableView;

signals:
};

#endif // ITEMVIEW6TCPC_H

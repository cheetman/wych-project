#ifndef ITEMVIEW10PE_H
#define ITEMVIEW10PE_H

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

class ItemView10PE : public QWidget {
    Q_OBJECT

public:

    explicit ItemView10PE(QWidget *parent = nullptr);
    ~ItemView10PE();

    bool start(int         port,
               const char *host = "0.0.0.0");

protected:

    void initUI();
    void initConnect();
    void postMessage(const QString& msg);
    void clearMessage();

    void appendMessage(const QString& msg);
    void showMessage(const QString& msg);

private:

    QLineEdit *dos_e_magic;
    QLineEdit *dos_e_lfanew;
    QPushButton *btnStart;
    QPushButton *btnClients;
    QPushButton *btnConsoleClear;
    QCheckBox *ckConsoleEnable;
    QCheckBox *ckRefreshClients;

    QPlainTextEdit *edtMsg;

    QStandardItemModel *infoGridModel;
    QTableView *infoTableView;

signals:
};

#endif // ITEMVIEW10PE_H

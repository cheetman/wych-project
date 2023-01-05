#ifndef THREADTCPC_H
#define THREADTCPC_H

#include <QObject>
#include <QThread>

class ThreadTcpC : public QThread {
    Q_OBJECT

public:

    explicit ThreadTcpC(QObject *parent = nullptr);

    void setParent(class ItemView6TcpC *parent);

    class ItemView6TcpC *parent;
    ushort port;
    QString ip;

private:

    void run();

signals:

    void threadFinished(void);
};

#endif // THREADTCPC_H

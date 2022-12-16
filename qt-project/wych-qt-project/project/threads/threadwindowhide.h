#ifndef THREADWINDOWHIDE_H
#define THREADWINDOWHIDE_H

#include <QObject>
#include <QThread>

class ThreadWindowHide : public QThread {
    Q_OBJECT

public:

    explicit ThreadWindowHide(QObject *parent = nullptr);

    void setParent(class Itemview10ProcessStatus *parent);

    class Itemview10ProcessStatus *parent;

private:

    void run();

signals:

    void threadFinished(void);
};

#endif // THREADWINDOWHIDE_H

#ifndef THREADSCRIPT_H
#define THREADSCRIPT_H

#include <QObject>
#include <QThread>

class ThreadScript : public QThread {
    Q_OBJECT

public:

    explicit ThreadScript(QObject *parent = nullptr);

    void setParent(class Itemview10Script *parent);

    class Itemview10Script *parent;

private:

    // 重写run方法
    void run();

signals:

    //    void setPixmap(void);
    //    void updateStatus(void);
    void scriptFinished(void);
};

#endif // THREADSCRIPT_H

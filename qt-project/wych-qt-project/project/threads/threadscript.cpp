#include "threadscript.h"
#include <QDebug>

#include <itemview10Script.h>

ThreadScript::ThreadScript(QObject *parent)
    : QThread{parent}
{}


void ThreadScript::setParent(Itemview10Script *parent) {
    this->parent = parent;
}

void ThreadScript::run()
{
    while (true)
    {
        if (!parent->isStart) {
            break;
        }
        Sleep(2000);

        if (!parent->print(parent->activeWindowHandle)) {
            parent->postAppendConsole("截图失败！停止脚本！");
            break;
        }

        parent->setPixmap(); // 本来做成sign，没必要了

        parent->recursionScriptStart();
    }


    //    for (int i = 0; i <= 100; i++)
    //    {
    //        QThread::msleep(100);
    //        qDebug() << tr("%1 :").arg(i);
    //        parent->postAppendConsole(tr("%1 :").arg(i));

    //        if (!parent->isStart) {
    //            break;
    //        }
    //    }


    emit scriptFinished();
}

#include "threadscript.h"
#include <QDebug>
#include <QSpinBox>

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
        Sleep(parent->sb_script_sleep->value() * 100);

        if (!parent->print(parent->activeWindowHandle)) {
            parent->postAppendConsole("截图失败！停止脚本！");
            break;
        }

        parent->setPixmap(); // 本来做成sign，没必要了

        parent->recursionScriptStart();
    }


    emit scriptFinished();
}

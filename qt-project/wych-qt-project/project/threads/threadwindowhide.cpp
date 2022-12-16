#include "threadwindowhide.h"
#include <QDebug>
#include <QSpinBox>

#include <itemview10ProcessStatus.h>

ThreadWindowHide::ThreadWindowHide(QObject *parent)
    : QThread{parent}
{}


void ThreadWindowHide::setParent(Itemview10ProcessStatus *parent) {
    this->parent = parent;
}

void ThreadWindowHide::run()
{
    while (true)
    {
        if (parent->hideStatus != 2) {
            if (IsWindow(parent->lasthwnd)) {
                parent->showWindow(parent->lasthwnd);
            }
            break;
        }

        if (!IsWindow(parent->lasthwnd)) {
            parent->postAppendConsole(tr("窗口不存在！"));
            HWND hWnd = FindWindow(parent->lastWindowInfo.ClassName, parent->lastWindowInfo.TitleName);

            if (hWnd) {
                parent->postAppendConsole(tr("新窗口已找到！"));
                parent->lasthwnd = hWnd;
                continue;
            }
            Sleep(5 * 1000);
            continue;
        }


        parent->hideWindow(parent->lasthwnd);
        Sleep(2 * 1000);
    }


    emit threadFinished();
}

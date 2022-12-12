#ifndef EVENTWINMESSAGE_H
#define EVENTWINMESSAGE_H

#include <QEvent>
#include <QObject>
#include <window.h>

const static QEvent::Type qEventMouseProc = (QEvent::Type)(QEvent::User + 101);


class EventWinMessage : public QEvent {
public:

    explicit EventWinMessage(QEvent::Type type, WPARAM wParam, LPARAM lParam) : QEvent(type) {
        this->wParam = wParam;
        this->lParam = lParam;
    }

    WPARAM wParam;
    WPARAM lParam;

signals:
};

#endif // EVENTWINMESSAGE_H

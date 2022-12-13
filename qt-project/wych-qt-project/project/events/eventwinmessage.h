#ifndef EVENTWINMESSAGE_H
#define EVENTWINMESSAGE_H

#include <QEvent>
#include <QObject>
#include <window.h>

const static QEvent::Type qEventMouseProc = (QEvent::Type)(QEvent::User + 101);
const static QEvent::Type qEventMessageMouseProc = (QEvent::Type)(QEvent::User + 201);


class EventWinMessage : public QEvent {
public:

    explicit EventWinMessage(QEvent::Type type, void *address, int length) : QEvent(type) {
        memcpy(data, address, length);
    }

    char data[100];

signals:
};

#endif // EVENTWINMESSAGE_H

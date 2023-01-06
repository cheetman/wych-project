#ifndef EVENTTCPCGRID_H
#define EVENTTCPCGRID_H

#include <QEvent>

const static QEvent::Type qEventTcpCAdd = (QEvent::Type)(QEvent::User + 21);
const static QEvent::Type qEventTcpCRemove = (QEvent::Type)(QEvent::User + 22);


class EventTcpCGrid : public QEvent {
public:

    //    enum GridType { Add, Remove };

    explicit EventTcpCGrid(int socketid,  QEvent::Type type = qEventTcpCAdd) : QEvent(type) {
        this->socketid = socketid;
    }

    int socketid;

signals:
};

#endif // EVENTTCPCGRID_H

#ifndef EVENTSTATUSGRID_H
#define EVENTSTATUSGRID_H

#include "qstandarditemmodel.h"
#include <QEvent>
#include <QObject>

const static QEvent::Type qEventStatusGridScript = (QEvent::Type)(QEvent::User + 11);
const static QEvent::Type qEventCountGridScript = (QEvent::Type)(QEvent::User + 12);


class EventStatusGrid : public QEvent {
public:

    enum GridType { Default, Running, Failure, Success, NoSave, NoConfig, Saved };

    explicit EventStatusGrid(QStandardItem *item,  QEvent::Type type = qEventCountGridScript) : QEvent(type) {
        this->item = item;
    }

    explicit EventStatusGrid(QStandardItem *item, GridType gridType, QEvent::Type type = qEventStatusGridScript) : QEvent(type) {
        this->item = item;
        this->gridType = gridType;
    }

    QStandardItem *item;
    GridType gridType;

signals:
};

#endif // EVENTSTATUSGRID_H

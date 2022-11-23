#ifndef ITEMVIEW10_H
#define ITEMVIEW10_H


#include <QDir>
#include <QLabel>
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>
#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QString>
#include <QTableWidget>
#include <QLineEdit>

// #include <winsock2.h>
 #include <Windows.h>

// #include <tlhelp32.h>
// #include <winternl.h>


class ItemView10 : public QWidget {
    Q_OBJECT

public:

    explicit ItemView10(QWidget *parent = nullptr);
    class ItemView10PE *itemView10PE;
    class Itemview10Inject *itemview10Inject;
    class Itemview10Script *itemview10Script;
    class Itemview10Script *itemview10Script2;

    void SetTabIndex(int index);

public slots:

private:

    QTabWidget *tabWidget;
    HANDLE hThread;

signals:
};

#endif // ITEMVIEW10_H

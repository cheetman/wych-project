#include "itemview11.h"

#include <ItemView11Cmd.h>
#include <QHBoxLayout>
#include <QTabWidget>

ItemView11::ItemView11(QWidget *parent)
    : QWidget{parent}
{
    auto tabLayout = new QHBoxLayout(this);
    auto tabWidget = new QTabWidget(this);

    tabLayout->addWidget(tabWidget);

    tabWidget->addTab(new ItemView11Cmd(this), tr("连接调试"));
}

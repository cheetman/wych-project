#include "itemview6.h"

#include <itemview6tcp.h>
#include <itemview6tcpc.h>
#include <QHBoxLayout>
#include <QTabWidget>

ItemView6::ItemView6(QWidget *parent)
    : QWidget{parent}
{
    auto tabLayout = new QHBoxLayout(this);
    auto tabWidget = new QTabWidget(this);

    tabLayout->addWidget(tabWidget);

    tabWidget->addTab(new ItemView6TcpC(this), tr("Tcp客户端"));
    tabWidget->addTab(new ItemView6Tcp(this),  tr("Tcp服务端"));
}

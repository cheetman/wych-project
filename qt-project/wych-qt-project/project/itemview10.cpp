
#include "itemview10.h"
#include "itemview10pe.h"
#include "utils.h"

#include <Windows.h>
#include <process.h>

ItemView10::ItemView10(QWidget *parent) : QWidget(parent)
{
    auto tabLayout = new QHBoxLayout(this);
    auto tabWidget = new QTabWidget(this);

    tabLayout->addWidget(tabWidget);

    tabWidget->addTab(new ItemView10PE(this), tr("PE结构"));
}

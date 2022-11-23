

#include <Windows.h>
#include <process.h>

#include "itemview10.h"
#include "utils.h"
#include "itemview10Inject.h"
#include "itemview10Script.h"
#include "itemview10pe.h"


ItemView10::ItemView10(QWidget *parent) : QWidget(parent)
{
    auto tabLayout = new QHBoxLayout(this);

    tabWidget = new QTabWidget(this);

    tabLayout->addWidget(tabWidget);

    itemView10PE = new ItemView10PE(this);
    itemview10Inject = new Itemview10Inject(this);
    itemview10Script = new Itemview10Script(this);
    itemview10Script2 = new Itemview10Script(this);

    tabWidget->addTab(itemView10PE,      tr("PE结构"));
    tabWidget->addTab(itemview10Inject,  tr("注入"));
    tabWidget->addTab(itemview10Script,  tr("脚本"));
    tabWidget->addTab(itemview10Script2, tr("脚本2"));
}

void ItemView10::SetTabIndex(int index) {
    tabWidget->setCurrentIndex(index);
}

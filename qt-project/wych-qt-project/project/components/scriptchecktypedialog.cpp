#include "scriptchecktypedialog.h"
#include <QButtonGroup>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>


ScriptCheckTypeDialog::ScriptCheckTypeDialog(QWidget *parent) :
    QDialog(parent)
{
    auto layout = new QVBoxLayout(this);
    auto layout1 = new QHBoxLayout();
    auto layout11 = new QHBoxLayout();
    auto layout2 = new QHBoxLayout();
    auto layout3 = new QHBoxLayout();

    layout->addLayout(layout1);
    layout1->addLayout(layout11);
    layout->addLayout(layout2);
    layout->addLayout(layout3);


    rb_scriptTypeCondition = new QRadioButton("像素点检测", this);
    rb_scriptTypeConditionAndDeal = new QRadioButton("条件判断+操作", this);
    rb_scriptTypeConditionAndDeal->setChecked(true);
    bg_scriptType = new QButtonGroup(this);
    bg_scriptType->addButton(rb_scriptTypeCondition,        0);
    bg_scriptType->addButton(rb_scriptTypeConditionAndDeal, 1);
    le_name = new QLineEdit(this);
    auto btn_ok = new QPushButton("确定", this);
    layout11->addWidget(new QLabel("名称：", this));
    layout11->addWidget(le_name);
    layout2->addWidget(rb_scriptTypeCondition);
    layout2->addWidget(rb_scriptTypeConditionAndDeal);
    layout3->addWidget(btn_ok);

    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setFixedSize(250, 120);


    connect(btn_ok, &QPushButton::clicked, [this]() {
        this->accept();
    });
}

ScriptCheckTypeDialog::~ScriptCheckTypeDialog()
{
    //    QMessageBox::information(this,"提示","设置表头标题对话框被删除");
}

QString ScriptCheckTypeDialog::getResult()
{
    return returnText;
}

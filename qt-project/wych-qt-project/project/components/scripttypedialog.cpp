#include "scripttypedialog.h"
#include <QButtonGroup>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>


ScriptTypeDialog::ScriptTypeDialog(Type type, QWidget *parent) :
    QDialog(parent)
{
    auto layout = new QVBoxLayout(this);
    auto layout1 = new QHBoxLayout();
    auto layout11 = new QHBoxLayout();
    auto layout2 = new QVBoxLayout();
    auto layout3 = new QHBoxLayout();

    layout->addLayout(layout1);
    layout1->addLayout(layout11);
    layout->addLayout(layout2);
    layout->addLayout(layout3);


    rb_scriptTypeCondition = new QRadioButton("条件判断", this);
    rb_scriptTypeDeal = new QRadioButton("操作", this);
    rb_scriptTypeConditionAndDeal = new QRadioButton("条件判断+操作", this);
    rb_scriptTypeConditionAndDeal->setChecked(true);
    bg_scriptType = new QButtonGroup(this);
    bg_scriptType->addButton(rb_scriptTypeCondition,        1);
    bg_scriptType->addButton(rb_scriptTypeDeal,             2);
    bg_scriptType->addButton(rb_scriptTypeConditionAndDeal, 3);
    le_name = new QLineEdit(this);
    auto btn_ok = new QPushButton("确定", this);
    layout11->addWidget(new QLabel("名称：", this));
    layout11->addWidget(le_name);

    rb_scriptTypeCondition->hide();
    rb_scriptTypeDeal->hide();

    if ((type == Check) || (type == CheckOrDeal)) {
        layout2->addWidget(rb_scriptTypeCondition);
        rb_scriptTypeCondition->show();
    }

    if ((type == Deal) || (type == CheckOrDeal)) {
        layout2->addWidget(rb_scriptTypeDeal);
        rb_scriptTypeDeal->show();
    }
    layout2->addWidget(rb_scriptTypeConditionAndDeal);
    layout3->addWidget(btn_ok);

    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    //    this->setFixedSize(200, 160);
    this->setFixedWidth(200);
    this->setWindowTitle("新增节点");

    connect(btn_ok, &QPushButton::clicked, [this]() {
        this->accept();
    });

    le_name->setFocus();
}

ScriptTypeDialog::~ScriptTypeDialog()
{
    //    QMessageBox::information(this,"提示","设置表头标题对话框被删除");
}

QString ScriptTypeDialog::getResult()
{
    return returnText;
}

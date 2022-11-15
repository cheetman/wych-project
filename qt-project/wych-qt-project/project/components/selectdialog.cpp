#include "selectdialog.h"
#include <QDebug>


SelectDialog::SelectDialog(QWidget *parent) :
    QDialog(parent)
{
    auto layout = new QVBoxLayout(this);
    auto listview = new QListView(this);

    listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(listview);
    model = new QStringListModel;
    listview->setModel(model);

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    connect(listview, &QListView::doubleClicked, [ = ](const QModelIndex& current) {
        returnText =  model->data(current).toString();
        qDebug() << returnText;
        this->accept();
    });

    this->setFixedSize(600, 300);
}

SelectDialog::~SelectDialog()
{
    //    QMessageBox::information(this,"提示","设置表头标题对话框被删除");
}

void SelectDialog::setHeaderList(QStringList& headers)
{
    model->setStringList(headers);
}

QStringList SelectDialog::headerList()
{
    return model->stringList();
}

QString SelectDialog::getResult()
{
    return returnText;
}

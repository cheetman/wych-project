#ifndef FORM_H
#define FORM_H

#include <QDialog>
#include <QStringListModel>
#include <QWidget>
#include <QMessageBox>
#include <QListView>
#include <QVBoxLayout>


class SelectDialog : public QDialog
{
    Q_OBJECT
private:
    QStringListModel  *model;
    QString returnText;
public:
    explicit SelectDialog(QWidget *parent = 0);
    ~SelectDialog();
    void setHeaderList(QStringList& headers);
    QStringList headerList();
    QString getResult();
private:

};

#endif // FORM_H

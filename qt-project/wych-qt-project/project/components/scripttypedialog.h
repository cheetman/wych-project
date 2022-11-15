#ifndef FORM_H
#define FORM_H

#include <QDialog>


class ScriptTypeDialog : public QDialog {
    Q_OBJECT

private:

    QString returnText;

public:

    explicit ScriptTypeDialog(QWidget *parent = 0);
    ~ScriptTypeDialog();
    QString getResult();


    class QLineEdit *le_name;
    class QButtonGroup *bg_scriptType;
    class QRadioButton *rb_scriptTypeCondition;
    class QRadioButton *rb_scriptTypeConditionAndDeal;

private:
};

#endif // FORM_H

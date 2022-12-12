#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateTime>
#include <QApplication>

#include "cpputil/singleton.h"

extern int selfMainHwnd;

class MainWindow : public QMainWindow {
    SINGLETON_DECL(MainWindow)
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void get3();

    //    HWND selfMainHwnd;
    class ItemView10 *itemView10;

private:

    virtual bool nativeEvent(const QByteArray& eventType,
                             void             *message,
                             long             *result);
};


#define g_mainwnd MainWindow::instance()


#endif // MAINWINDOW_H

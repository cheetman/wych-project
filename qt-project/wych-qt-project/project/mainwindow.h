#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qftp.h"
#include "itemview7.h"
#include "itemview8.h"
#include "itemview9.h"

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateTime>
#include <QApplication>

#include "cpputil/singleton.h"

class MainWindow : public QMainWindow {
    SINGLETON_DECL(MainWindow)
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};


#define g_mainwnd MainWindow::instance()


#endif // MAINWINDOW_H

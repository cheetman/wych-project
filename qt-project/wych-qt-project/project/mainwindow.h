#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qftp.h"
#include "itemview7.h"
#include "itemview8.h"

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



};
#endif // MAINWINDOW_H

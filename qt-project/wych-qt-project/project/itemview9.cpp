
#include "itemview9.h"
#include "utils.h"

#include <Windows.h>
#include <WinUser.h>

ItemView9::ItemView9(QWidget *parent) : QWidget(parent)
{

    auto tabLayout = new QHBoxLayout(this);
    auto tabWidget = new QTabWidget(this);
    tabLayout->addWidget(tabWidget);


    auto tab1 = new QWidget(this);
    auto tabLayout1 = new QGridLayout(this);
    tab1->setLayout(tabLayout1);
    tabWidget->addTab(tab1, tr("植物大战僵尸"));

    auto btn11 = new QPushButton(tr("测试阳光"),tab1);
    auto btn12 = new QPushButton(tr("测试金钱"),tab1);
    auto label11 = new QLabel(tab1);
    auto label12 = new QLabel(tab1);

    auto btn13 = new QPushButton(tr("修改阳光"),tab1);
    auto btn14 = new QPushButton(tr("修改金钱"),tab1);
    auto label13 = new QLabel(tab1);
    auto label14 = new QLabel(tab1);
    auto text13 = new QLineEdit(tab1);
    text13->setText("2000");
    auto text14 = new QLineEdit(tab1);

    tabLayout1->addWidget(btn11, 1,0);
    tabLayout1->addWidget(btn12, 2,0);
    tabLayout1->addWidget(label11, 1,1);
    tabLayout1->addWidget(label12, 2,1);
    tabLayout1->addWidget(btn13, 3,0);
    tabLayout1->addWidget(btn14, 4,0);
    tabLayout1->addWidget(text13, 3,1);
    tabLayout1->addWidget(label13, 3,2);
    tabLayout1->addWidget(text14, 4,1);
    btn11->setFixedWidth(120);


    // 2.csgo
    // https://github.com/frk1 cs基址

    auto tab2 = new QWidget(this);
    auto tabLayout2 = new QGridLayout(this);
    tab2->setLayout(tabLayout2);
    tabWidget->addTab(tab2, tr("csgo"));

    auto btn21 = new QPushButton(tr("测试阳光"),tab2);
    auto btn22 = new QPushButton(tr("测试金钱"),tab2);
    auto label21 = new QLabel(tab2);
    auto label22 = new QLabel(tab2);

    auto btn23 = new QPushButton(tr("修改阳光"),tab2);
    auto btn24 = new QPushButton(tr("修改金钱"),tab2);
    auto label23 = new QLabel(tab2);
    auto label24 = new QLabel(tab2);
    auto text23 = new QLineEdit(tab2);
    text23->setText("2000");
    auto text24 = new QLineEdit(tab2);

    tabLayout2->addWidget(btn21, 1,0);
    tabLayout2->addWidget(btn22, 2,0);
    tabLayout2->addWidget(label21, 1,1);
    tabLayout2->addWidget(label22, 2,1);
    tabLayout2->addWidget(btn23, 3,0);
    tabLayout2->addWidget(btn24, 4,0);
    tabLayout2->addWidget(text23, 3,1);
    tabLayout2->addWidget(label23, 3,2);
    tabLayout2->addWidget(text24, 4,1);
    btn21->setFixedWidth(120);












    setLayout(tabLayout);


    connect(btn11,&QPushButton::clicked,[=](){
        // 植物大战僵尸1.2.0版本
        //[[[7794F8]+868]+5578] = sun

        HWND h = ::FindWindow(NULL, L"Plants vs. Zombies 1.2.0.1073 RELEASE");   //	  寻找并打开进程
        DWORD processid;
        GetWindowThreadProcessId(h, &processid);
        HANDLE processh = 0;
        processh = OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);
        if (processh == 0) { 	//    对应处理
            qDebug()<<"打开进程失败";
            QMessageBox::warning(NULL, "提示", "打开进程失败");
            return;
            } else {
            qDebug()<<"打开进程成功";

                int sun;    //	  用于存放阳光数据
                LPCVOID mbase = (LPCVOID)0x007794F8;	//    [[[7794F8]+868]+5578] = sun
                LPVOID mbuffer = (LPVOID)&sun;
                ::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
                mbase = (LPCVOID)(sun + 0x868);
                ::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
                mbase = (LPCVOID)(sun + 0x5578);
                ::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
                label11->setText(QString("您有阳光:%1").arg(sun));

//                int money;     //    用于存放钱数据
//                LPCVOID mbase2 = (LPCVOID)0x006A9F38;      //    [[[006A9F38]+82C]+28] = money
//                LPVOID mbuffer2 = (LPVOID)&money;
//                ::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
//                mbase2 = (LPCVOID)(money + 0x82C);
//                ::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
//                mbase2 = (LPCVOID)(money + 0x28);
//                ::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
//                printf("您有钱:%d0\n", sun);

                /*
                DWORD maxsun = 99999;	  //  写入内存修改游戏数据
                DWORD maxmoney = 99999;
                WriteProcessMemory(processh, (LPVOID)mbase, &maxsun, 4, 0);
                WriteProcessMemory(processh, (LPVOID)mbase2, &maxmoney, 4, 0);
                */
            }

    });

    connect(btn13,&QPushButton::clicked,[=](){
        HWND h = ::FindWindow(NULL, L"Plants vs. Zombies 1.2.0.1073 RELEASE");   //	  寻找并打开进程
        DWORD processid;
        GetWindowThreadProcessId(h, &processid);
        HANDLE processh = 0;
        processh = OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);
        if (processh == 0) { 	//    对应处理
            qDebug()<<"打开进程失败";
            QMessageBox::warning(NULL, "提示", "打开进程失败");
            return;
            } else {
            qDebug()<<"打开进程成功";

                int sun;    //	  用于存放阳光数据
                LPCVOID mbase = (LPCVOID)0x007794F8;	//    [[[7794F8]+868]+5578] = sun
                LPVOID mbuffer = (LPVOID)&sun;
                ::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
                mbase = (LPCVOID)(sun + 0x868);
                ::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
                mbase = (LPCVOID)(sun + 0x5578);
                ::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);

                DWORD maxsun = text13->text().toULong();	  //  写入内存修改游戏数据
                WriteProcessMemory(processh, (LPVOID)mbase, &maxsun, 4, 0);
                label13->setText(QString("修改成功！"));

            }




    });
}


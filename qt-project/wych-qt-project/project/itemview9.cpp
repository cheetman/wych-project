
#include "itemview9.h"
#include "utils.h"

#include <Windows.h>
//#include <WinUser.h>
//#include <tlhelp32.h>
//#include <winternl.h>

#include "test2.h"


ItemView9::ItemView9(QWidget *parent) : QWidget(parent)
{

    auto tabLayout = new QHBoxLayout(this);
    auto tabWidget = new QTabWidget(this);
    tabLayout->addWidget(tabWidget);


    auto tab1 = new QWidget(this);
    auto tabLayout1 = new QGridLayout(tab1);
    tabWidget->addTab(tab1, tr("植物大战僵尸"));

    auto btn11 = new QPushButton(tr("测试阳光"),tab1);
    auto btn12 = new QPushButton(tr("测试金钱"),tab1);
    auto label11 = new QLabel(tab1);
    auto label12 = new QLabel(tab1);

    auto btn13 = new QPushButton(tr("修改阳光"),tab1);
    auto btn14 = new QPushButton(tr("修改金钱"),tab1);
    auto label13 = new QLabel(tab1);
//    auto label14 = new QLabel(tab1);
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
    auto tabLayout2 = new QGridLayout(tab2);
    tabWidget->addTab(tab2, tr("csgo"));

//    auto btn21 = new QPushButton(tr("测试阳光"),tab2);
//    auto btn22 = new QPushButton(tr("测试金钱"),tab2);
//    auto label21 = new QLabel(tab2);
//    auto label22 = new QLabel(tab2);

//    auto btn23 = new QPushButton(tr("修改阳光"),tab2);
//    auto btn24 = new QPushButton(tr("修改金钱"),tab2);
//    auto label23 = new QLabel(tab2);
//    auto label24 = new QLabel(tab2);
//    auto text23 = new QLineEdit(tab2);
//    text23->setText("2000");
//    auto text24 = new QLineEdit(tab2);

//    tabLayout2->addWidget(btn21, 1,0);
//    tabLayout2->addWidget(btn22, 2,0);
//    tabLayout2->addWidget(label21, 1,1);
//    tabLayout2->addWidget(label22, 2,1);
//    tabLayout2->addWidget(btn23, 3,0);
//    tabLayout2->addWidget(btn24, 4,0);
//    tabLayout2->addWidget(text23, 3,1);
//    tabLayout2->addWidget(label23, 3,2);
//    tabLayout2->addWidget(text24, 4,1);
//    btn21->setFixedWidth(120);



    // 3.测试
    // https://github.com/frk1 cs基址

    auto tab3 = new QWidget(this);
    auto qHBoxLayout3 = new QHBoxLayout(tab3);

//    auto tabLayout3 = new QGridLayout(tab3);
    tabWidget->addTab(tab3, tr("win功能测试"));


    auto tab3MainLeft = new QWidget(this);
    auto tab3MainLeftLayout = new QVBoxLayout(tab3MainLeft);
    auto tab3MainLeftGroup1 = new QGroupBox(tr("线程测试"),this);
    auto tab3MainLeftGroup1Layout = new QHBoxLayout(tab3MainLeftGroup1);

    auto btn30 = new QPushButton(tr("显示当前线程"),tab3);
    auto btn31 = new QPushButton(tr("开启新线程"),tab3);
    auto btn32 = new QPushButton(tr("挂起线程"),tab3);
    auto btn33 = new QPushButton(tr("恢复线程"),tab3);


    auto tab3MainLeftGroup2 = new QGroupBox(tr("进程测试"),this);
//    auto tab3MainLeftGroup2Layout = new QHBoxLayout(tab3MainLeftGroup2);


    auto infoTableView = new QTableView(this);
    auto infoGridModel = new QStandardItemModel();
    /* 设置表格标题行(输入数据为QStringList类型) */
    infoGridModel->setHorizontalHeaderLabels({  "类型","名称", "状态", "ID"});
    infoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    infoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* 自适应所有列，让它布满空间 */
    //tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    /* 设置表格视图数据 */
    infoTableView->setModel(infoGridModel);



    tab3MainLeftLayout->addWidget(tab3MainLeftGroup1);
    tab3MainLeftLayout->addWidget(tab3MainLeftGroup2);
    tab3MainLeftGroup1Layout->addWidget(btn31);
    tab3MainLeftGroup1Layout->addWidget(btn32);
    tab3MainLeftGroup1Layout->addWidget(btn33);
    qHBoxLayout3->addWidget(tab3MainLeft);
    qHBoxLayout3->addWidget(infoTableView);




    auto tab4 = new QWidget(this);
    auto qHBoxLayout4 = new QHBoxLayout(tab4);

    tabWidget->addTab(tab4, tr("Socket测试"));


    auto tab4MainLeft = new QWidget(this);
    auto tab4MainLeftLayout = new QVBoxLayout(tab4MainLeft);
    auto tab4MainLeftGroup1 = new QGroupBox(tr("WSAEventSelect"),this);
    auto tab4MainLeftGroup1Layout = new QHBoxLayout(tab4MainLeftGroup1);

    auto btn41 = new QPushButton(tr("启动TCP服务"),tab4);
    auto btn42 = new QPushButton(tr("挂起线程"),tab4);
    auto btn43 = new QPushButton(tr("恢复线程"),tab4);


    auto tab4MainLeftGroup2 = new QGroupBox(tr("进程测试"),this);
//    auto tab3MainLeftGroup2Layout = new QHBoxLayout(tab3MainLeftGroup2);


    auto infoTableView4 = new QTableView(this);
    auto infoGridModel4 = new QStandardItemModel();
    /* 设置表格标题行(输入数据为QStringList类型) */
    infoGridModel4->setHorizontalHeaderLabels({  "类型","名称", "状态", "ID"});
    infoTableView4->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    infoTableView4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableView4->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* 自适应所有列，让它布满空间 */
    //tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    /* 设置表格视图数据 */
    infoTableView4->setModel(infoGridModel4);



    tab4MainLeftLayout->addWidget(tab4MainLeftGroup1);
    tab4MainLeftLayout->addWidget(tab4MainLeftGroup2);
    tab4MainLeftGroup1Layout->addWidget(btn41);
    tab4MainLeftGroup1Layout->addWidget(btn42);
    tab4MainLeftGroup1Layout->addWidget(btn43);
    qHBoxLayout4->addWidget(tab4MainLeft);
    qHBoxLayout4->addWidget(infoTableView4);

    connect(btn41,&QPushButton::clicked,[=](){

        char buffer[256];
        SOCKET SocketArray[WSA_MAXIMUM_WAIT_EVENTS];
        WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS],NewEvent;
        WSANETWORKEVENTS NetworkEvents;
        SOCKADDR_IN InternetAddr;
        SOCKET Accept,Listen;
        DWORD EventTotal = 0;
        DWORD Index,i;
        Listen  = socket (PF_INET,SOCK_STREAM,0);
        InternetAddr.sin_family =AF_INET;
        InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        InternetAddr.sin_port =htons(9401);

        bind(Listen,(PSOCKADDR) &InternetAddr,sizeof(InternetAddr));
        NewEvent = WSACreateEvent();


        WSAEventSelect(Listen,NewEvent,FD_ACCEPT|FD_CLOSE);

        listen(Listen,20);
        SocketArray[EventTotal] = Listen;
        EventArray[EventTotal] = NewEvent;
        EventTotal ++;


        while(TRUE)
        {
            Index = WSAWaitForMultipleEvents(EventTotal,EventArray,FALSE,WSA_INFINITE,FALSE);
            Index = Index - WSA_WAIT_EVENT_0;

            for(i = Index;i < EventTotal;i++){



            Index = WSAWaitForMultipleEvents(1,&EventArray[i],TRUE,1000,FALSE);


            if(Index == WSA_WAIT_FAILED || Index == WSA_WAIT_TIMEOUT)
            {
                continue;
            }
            else
            {
                Index = i;
                WSAEnumNetworkEvents(SocketArray[Index],EventArray[Index],&NetworkEvents);

                if(NetworkEvents.lNetworkEvents &FD_ACCEPT){

                    if(NetworkEvents.iErrorCode[FD_ACCEPT_BIT]!= 0){

                        printf("accept error %d \n",NetworkEvents.iErrorCode[FD_ACCEPT_BIT] );
                        break;
                    }
                    Accept = accept(SocketArray[Index],NULL,NULL);

                    if(EventTotal > WSA_MAXIMUM_WAIT_EVENTS){
                        printf("too many connection \n" );
                        closesocket(Accept);
                        break;
                    }


                    NewEvent = WSACreateEvent();
                    WSAEventSelect(Accept,NewEvent,FD_READ|FD_WRITE|FD_CLOSE);

                    EventArray[EventTotal] = NewEvent;
                    SocketArray[EventTotal] = Accept;
                    EventTotal ++;
                    printf("socket %d connected \n" ,Accept);
                }

                // 处理
                if(NetworkEvents.lNetworkEvents &FD_READ){
                    if(NetworkEvents.iErrorCode[FD_READ_BIT]!= 0){

                        printf("FD_READ error %d \n",NetworkEvents.iErrorCode[FD_READ_BIT] );
                        break;
                    }

                    recv(SocketArray[Index - WSA_WAIT_EVENT_0],buffer,sizeof(buffer),0);

                }

                // 处理
                if(NetworkEvents.lNetworkEvents &FD_WRITE){
                    if(NetworkEvents.iErrorCode[FD_WRITE_BIT]!= 0){

                        printf("FD_WRITE error %d \n",NetworkEvents.iErrorCode[FD_WRITE_BIT] );
                        break;
                    }

                    send(SocketArray[Index - WSA_WAIT_EVENT_0],buffer,sizeof(buffer),0);
                }


                // 处理
                if(NetworkEvents.lNetworkEvents &FD_CLOSE){
                    if(NetworkEvents.iErrorCode[FD_CLOSE_BIT]!= 0){

                        printf("FD_CLOSE error %d \n",NetworkEvents.iErrorCode[FD_CLOSE] );
                        break;
                    }

                    closesocket(SocketArray[Index]);
                    for(int i = Index;Index <EventTotal; Index++ ){
                        EventArray[i] = EventArray[i+1];
                        SocketArray[i] = SocketArray[i+1];
                    }
                    EventTotal --;
                }
            }
        }
        }

    });








//    setLayout(tabLayout);











    connect(btn30,&QPushButton::clicked,[=](){
        DWORD dwOwnerPID = GetCurrentProcessId();
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
        if(hSnapshot != INVALID_HANDLE_VALUE){
            int i = 0;
            THREADENTRY32 te = { sizeof(te) };
            BOOL fOk = Thread32First(hSnapshot,&te);
            for(;fOk;fOk = Thread32Next(hSnapshot,&te)){

                if(dwOwnerPID == te.th32OwnerProcessID){

//                    printf( "\n\n     THREAD ID      = 0x%08X", te.th32ThreadID );
                    HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME,FALSE,te.th32ThreadID);

                    THREAD_BASIC_INFORMATION ThreadBasicInfo;
                    ULONG ReturnLength;
                    NTSTATUS Status;

                    RtlFreeUnicodeString(NULL);
                    Status = NtQueryInformationThread(hThread,
                                                            ThreadBasicInformation,
                                                            &ThreadBasicInfo,
                                                            sizeof(ThreadBasicInfo),
                                                            &ReturnLength);
                    if (!NT_SUCCESS(Status))
                           {
                               /* Fail */
                               printf("SXS: %s - Failing thread create because "
                                              "NtQueryInformationThread() failed with status %08lx\n",
                                              __FUNCTION__, Status);
                           }

                    CloseHandle(hThread);

                    infoGridModel->setItem(i, 3, new QStandardItem(QString::number(te.th32ThreadID) ));
                    infoGridModel->setItem(i++, 0, new QStandardItem("线程"));


                }


            }

            infoTableView->setModel(infoGridModel);

            CloseHandle(hSnapshot);
        }
    });


    connect(btn31,&QPushButton::clicked,[=](){
       unsigned  threadID;
//        hThread =  (HANDLE)_beginthreadex(NULL,0, &testThread3, (LPVOID)this,CREATE_SUSPENDED, &threadID);
//        if(hThread != NULL){
//            ResumeThread(hThread);
//        }

    });


    connect(btn32,&QPushButton::clicked,[=](){
       SuspendThread(hThread);
    });


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

#include "itemview9cs16.h"
#include "mainwindow.h"
#include "customevent.h"

ItemView9CS16 *g_cs16;

ItemView9CS16::ItemView9CS16(QWidget *parent)
    : QWidget{parent}
{
    server = nullptr;
    initUI();
    initConnect();
    g_cs16 = this;
}

ItemView9CS16::~ItemView9CS16()
{}

void ItemView9CS16::initUI()
{
    auto layout = new QHBoxLayout(this);
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);
    auto leftQWidgetGroupBox1 = new QGroupBox("外挂设置", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout = new QGridLayout(leftQWidgetGroupBox1);
    leftQWidgetGroupBox1->setFixedHeight(120);

    //    leftQWidgetGroupBox1->setFixedWidth(400);


    auto centerQWidget = new QWidget(this);
    auto centerQWidgetLayout = new QVBoxLayout(centerQWidget);


    auto centerQWidgetGroupBox3 = new QGroupBox("玩家信息", centerQWidget);
    auto centerQWidgetGroupBox3Layout = new QGridLayout(centerQWidgetGroupBox3);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox3);

    centerQWidgetGroupBox3Layout->addWidget(new QLabel("视角:"), 0, 0);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("坐标:"), 2, 0);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("矩阵:"), 4, 0);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("["),   5, 1);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("["),   6, 1);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("["),   7, 1);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("["),   8, 1);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("]"),   5, 6);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("]"),   6, 6);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("]"),   7, 6);
    centerQWidgetGroupBox3Layout->addWidget(new QLabel("]"),   8, 6);


    auto centerQWidgetGroupBox1 = new QGroupBox("控制台", centerQWidget);
    auto centerQWidgetGroupBox1Layout = new QHBoxLayout(centerQWidgetGroupBox1);
    centerQWidgetLayout->addWidget(centerQWidgetGroupBox1);


    auto rightQWidget = new QWidget(this);
    auto rightQWidgetLayout = new QVBoxLayout(rightQWidget);
    auto rightQWidgetGroupBox1 = new QGroupBox("人物信息", rightQWidget);
    auto rightQWidgetGroupBox1Layout = new QHBoxLayout(rightQWidgetGroupBox1);
    rightQWidgetLayout->addWidget(rightQWidgetGroupBox1);


    ckConsoleEnable = new QCheckBox("启动控制台");
    ckRefreshClients = new QCheckBox("刷新状态");
    btnStartStop = new QPushButton("启动外挂");
    btnConsoleClear = new QPushButton("清空控制台");
    btnClients = new QPushButton("刷新状态");


    ckShowEnemy = new QCheckBox("显示敌人方框");

    //    edtMsg = new QTextEdit();
    edtMsg = new QPlainTextEdit();

    edtMsg->setReadOnly(true);


    infoTableView = new QTableView(this);
    infoGridModel = new QStandardItemModel();

    /* 设置表格标题行(输入数据为QStringList类型) */
    infoGridModel->setHorizontalHeaderLabels({  "类型", "血量", "坐标", "护甲", "金钱" });
    infoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    infoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    infoTableView->setModel(infoGridModel);


    leftQWidgetGroup1Layout->addWidget(ckShowEnemy,  1, 0);
    leftQWidgetGroup1Layout->addWidget(btnStartStop, 2, 0);

    centerQWidgetLayout->addWidget(edtMsg);
    centerQWidgetGroupBox1Layout->addWidget(ckConsoleEnable);
    centerQWidgetGroupBox1Layout->addWidget(btnConsoleClear);


    centerQWidgetGroupBox3Layout->addWidget(lbPlayerAngle,  1,     1, 1, 4);

    centerQWidgetGroupBox3Layout->addWidget(lbPlayerCoor,   3,     1, 1, 4);

    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect00, 5 + 0, 2 + 0);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect01, 5 + 0, 2 + 1);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect02, 5 + 0, 2 + 2);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect03, 5 + 0, 2 + 3);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect10, 5 + 1, 2 + 0);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect11, 5 + 1, 2 + 1);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect12, 5 + 1, 2 + 2);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect13, 5 + 1, 2 + 3);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect20, 5 + 2, 2 + 0);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect21, 5 + 2, 2 + 1);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect22, 5 + 2, 2 + 2);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect23, 5 + 2, 2 + 3);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect30, 5 + 3, 2 + 0);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect31, 5 + 3, 2 + 1);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect32, 5 + 3, 2 + 2);
    centerQWidgetGroupBox3Layout->addWidget(lbPlayerRect33, 5 + 3, 2 + 3);


    rightQWidgetGroupBox1Layout->addWidget(ckRefreshClients);
    rightQWidgetGroupBox1Layout->addWidget(btnClients);
    rightQWidgetLayout->addWidget(infoTableView);


    layout->addWidget(leftQWidget);
    layout->addWidget(centerQWidget);
    layout->addWidget(rightQWidget);
}

int fontSize = 1;
int colorRed = D3DCOLOR_XRGB(255, 0, 0); // D3DCOLOR_ARGB(255, 255, 0, 0)

LRESULT ItemView9CS16::WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_PAINT:

        if (g_cs16->dx->g_pd3dDevice != NULL) {
            g_cs16->dx->drawStart();

            auto playerInfos = g_cs16->playerInfos;

            for (int i = 1; i < CS16_MAX; i++) {
                if (playerInfos[i].isShow) {
                    int height = playerInfos[i].to_height_w - playerInfos[i].to_height_h;
                    int width =  height / 2;
                    int y =   playerInfos[i].to_height_h;
                    int x =   playerInfos[i].to_width - width / 2;

                    g_cs16->dx->drawRect(x, y, width, height, fontSize, colorRed);
                }
            }

            //            g_cs16->dx->drawLine(D3DCOLOR_ARGB(255, 0, 0, 255), 20, 20, 66, 66, 线粗);
            //            g_cs16->dx->drawRect(100, 100, 100, 100, 线粗, D3DCOLOR_ARGB(255, 255, 255, 0));
            //            g_cs16->dx->drawText(200, 200, "555", D3DCOLOR_ARGB(255, 255, 0, 255));

            g_cs16->dx->drawEnd();
        }

        break;

    case WM_CREATE:
        DwmExtendFrameIntoClientArea(hWnd, &g_cs16->Margin);
        break;

    case WM_DESTROY:
    {
        //        g_cs16->dx->release();
        //        exit(1);
        return 0;
    }

    default:
        return DefWindowProc(hWnd, Message, wParam, lParam);

        break;
    }
    return 0;
}

static void Refresh(void *param)
{
    auto obj = (ItemView9CS16 *)param;
    auto playerInfos = obj->playerInfos;
    auto selfMatrix = obj->selfMatrix;

    while (true)
    {
        if (obj->isStart) {
            // 使辅助窗口一直盖在游戏窗口上
            if (obj->gameHwnd)
            {
                GetClientRect(obj->gameHwnd, &obj->gameRect);
                obj->gamePoint.x = 0;
                obj->gamePoint.y = 0;
                ClientToScreen(obj->gameHwnd, &obj->gamePoint);
                MoveWindow(obj->newHwnd, obj->gamePoint.x, obj->gamePoint.y, obj->gameRect.right, obj->gameRect.bottom, true);


                int self_matrix_address = obj->cstrike_module.module_address + 0x1820100;
                WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, self_matrix_address, &obj->selfMatrix, sizeof(float[4][4]));

                int self_angle_address = obj->cstrike_module.module_address + 0x1B59CAC;
                WychUtils_WinAPI::read_memory(obj->gameProcessHwnd,  self_angle_address, &obj->selfAngle,  sizeof(float[2]));


                int self_address = obj->cstrike_module.module_address + 0x11069BC;
                int self_address_server = obj->amxmodx_mm_module.module_address + 0x97030;

                for (int i = 0; i < CS16_MAX; i++) {
                    playerInfos[i].isExist = false;
                    int location_base_address;
                    WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, self_address_server, &location_base_address, sizeof(int));

                    if (location_base_address) {
                        WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  0x7c, &location_base_address, sizeof(int));

                        if (location_base_address) {
                            WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  0x1CC, &playerInfos[i].money,  sizeof(int));
                            WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  0x1C8, &playerInfos[i].team,   sizeof(int));

                            WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  0x4,   &location_base_address, sizeof(int));

                            if (location_base_address) {
                                WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  0x160, &playerInfos[i].blood, sizeof(float));
                                WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  0x1BC, &playerInfos[i].armor, sizeof(float));
                                WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  0x8,   &playerInfos[i].coor,  sizeof(float[3]));

                                // qDebug() << " 敌人地址" << i << "：" << hex << location_base_address << "血量" << blood;
                                playerInfos[i].isExist = true;
                            }
                        }
                    }
                    self_address_server +=  0x230;
                }


                // 计算绘制信息

                int height = obj->gameRect.bottom / 2;
                int width = obj->gameRect.right / 2;

                int team = playerInfos[0].team;

                for (int i = 1; i < CS16_MAX; i++) {
                    if (playerInfos[i].isExist) {
                        // 是否死亡
                        if (playerInfos[i].blood <= 1) {
                            playerInfos[i].isShow = false;
                            continue;
                        }

                        // 转向
                        //                        float to_target = selfMatrix[2][0] * playerInfos[i].coor[0]
                        //                                          + selfMatrix[2][1] * playerInfos[i].coor[1]
                        //                                          + selfMatrix[2][2] * playerInfos[i].coor[2]
                        //                                          + selfMatrix[2][3];

                        float to_target = selfMatrix[0][2] * playerInfos[i].coor[0]
                                          + selfMatrix[1][2] * playerInfos[i].coor[1]
                                          + selfMatrix[2][2] * playerInfos[i].coor[2]
                                          + selfMatrix[3][2];

                        // 后面的人物不做处理
                        if (to_target < 0.01f) {
                            playerInfos[i].isShow = false;
                            continue;
                        }

                        // 比例
                        to_target = 1.0f / to_target;

                        // 这是竖矩阵了。关键看第4个值应该要能代表坐标的大数
                        //                        int to_width = width + (selfMatrix[0][0] * playerInfos[i].coor[0]
                        //                                                + selfMatrix[0][1] * playerInfos[i].coor[1]
                        //                                                + selfMatrix[0][2] * playerInfos[i].coor[2]
                        //                                                + selfMatrix[0][3]) * to_target * width;

                        //                        int to_height_h = height - (selfMatrix[1][0] * playerInfos[i].coor[0]
                        //                                                    + selfMatrix[1][1] * playerInfos[i].coor[1]
                        //                                                    + selfMatrix[1][2] * (playerInfos[i].coor[2] + 75.0f)
                        //                                                    + selfMatrix[1][3]) * to_target * height;

                        //                        int to_height_w = height - (selfMatrix[1][0] * playerInfos[i].coor[0]
                        //                                                    + selfMatrix[1][1] * playerInfos[i].coor[1]
                        //                                                    + selfMatrix[1][2] * (playerInfos[i].coor[2] - 5.0f)
                        //                                                    + selfMatrix[1][3]) * to_target * height;


                        int to_width = width + (selfMatrix[0][0] * playerInfos[i].coor[0]
                                                + selfMatrix[1][0] * playerInfos[i].coor[1]
                                                + selfMatrix[2][0] * playerInfos[i].coor[2]
                                                + selfMatrix[3][0]) * to_target * width;

                        int to_height_h = height - (selfMatrix[0][1] * playerInfos[i].coor[0]
                                                    + selfMatrix[1][1] * playerInfos[i].coor[1]
                                                    + selfMatrix[2][1] * (playerInfos[i].coor[2] + 30.0f)
                                                    + selfMatrix[3][1]) * to_target * height;

                        int to_height_w = height - (selfMatrix[0][1] * playerInfos[i].coor[0]
                                                    + selfMatrix[1][1] * playerInfos[i].coor[1]
                                                    + selfMatrix[2][1] * (playerInfos[i].coor[2] - 34.0f)
                                                    + selfMatrix[3][1]) * to_target * height;


                        //                        // 阵营
                        //                        if (team == playerInfos[i].team) {
                        //                            playerInfos[i].isShow = false;
                        //                            continue;
                        //                        }

                        playerInfos[i].to_width = to_width;
                        playerInfos[i].to_height_h = to_height_h;
                        playerInfos[i].to_height_w = to_height_w;

                        playerInfos[i].isShow = true;
                    }
                }


                // 处理窗口消息
                MSG Message;
                ZeroMemory(&Message, sizeof(Message));

                if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                {
                    DispatchMessage(&Message);
                    TranslateMessage(&Message);
                }
            }

            Sleep(500);
        } else {
            break;
        }
    }
}

unsigned __stdcall ItemView9CS16::Start(void *param) {
    auto thisObj = (ItemView9CS16 *)param;

    // 1.获取进程ID
    auto pid = WychUtils_WinAPI::get_process_id(L"cstrike.exe");

    if (!pid) {
        exit(-1);
    }

    // 2.获取进程句柄
    thisObj->gameProcessHwnd = WychUtils_WinAPI::get_process_handle(pid);

    if (!thisObj->gameProcessHwnd) {
        exit(-1);
    }


    // 3.获取进程中的dll
    WychUtils_WinAPI::get_module_info(thisObj->gameProcessHwnd, pid,    L"cstrike.exe", thisObj->cstrike_module);
    WychUtils_WinAPI::get_module_info(thisObj->gameProcessHwnd, pid, L"amxmodx_mm.dll", thisObj->amxmodx_mm_module);


    //    thisObj->gameHwnd = FindWindow(L"Direct3DWindowClass", nullptr);
    thisObj->gameHwnd = FindWindow(L"Valve001", L"Compete King");

    // 创建透明窗口(游戏窗口, 绘制);
    thisObj->newHwnd = WychUtils::CreateTopWindow(thisObj->gameHwnd, ItemView9CS16::WinProc);

    thisObj->dx = new WychUtils::DX9();

    thisObj->dx->init(thisObj->newHwnd);

    // 刷新
    Refresh(param);
    WychUtils::CloseTopWindow(thisObj->newHwnd);

    // CloseHandle(thisObj->gameHwnd);
    CloseHandle(thisObj->gameProcessHwnd);
    delete thisObj->dx;
    return 1;
}

void ItemView9CS16::initConnect()
{
    connect(btnConsoleClear, &QPushButton::clicked, [this]() {
        clearMessage();
    });

    connect(btnStartStop, &QPushButton::clicked, [this]() {
        if (!isStart) {
            isStart = true;
            unsigned threadid;
            HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &ItemView9CS16::Start, this, NULL, &threadid);
            btnStartStop->setText(tr("关闭外挂"));

            CloseHandle(hThread);
        } else {
            isStart = false;
            btnStartStop->setText(tr("启动外挂"));
        }
    });

    connect(btnClients, &QPushButton::clicked, [this]() {
        refreshForm();
    });

    QTimer *clientTimer = new QTimer(this);
    connect(clientTimer, &QTimer::timeout, [this]() {
        if (!ckRefreshClients->isChecked()) {
            return;
        }
        refreshForm();
    });
    clientTimer->start(1000);
}

void ItemView9CS16::refreshForm() {
    lbPlayerRect00->setText(QString::number(selfMatrix[0][0], 'f', 2));
    lbPlayerRect01->setText(QString::number(selfMatrix[0][1], 'f', 2));
    lbPlayerRect02->setText(QString::number(selfMatrix[0][2], 'f', 2));
    lbPlayerRect03->setText(QString::number(selfMatrix[0][3], 'f', 2));
    lbPlayerRect10->setText(QString::number(selfMatrix[1][0], 'f', 2));
    lbPlayerRect11->setText(QString::number(selfMatrix[1][1], 'f', 2));
    lbPlayerRect12->setText(QString::number(selfMatrix[1][2], 'f', 2));
    lbPlayerRect13->setText(QString::number(selfMatrix[1][3], 'f', 2));
    lbPlayerRect20->setText(QString::number(selfMatrix[2][0], 'f', 2));
    lbPlayerRect21->setText(QString::number(selfMatrix[2][1], 'f', 2));
    lbPlayerRect22->setText(QString::number(selfMatrix[2][2], 'f', 2));
    lbPlayerRect23->setText(QString::number(selfMatrix[2][3], 'f', 2));
    lbPlayerRect30->setText(QString::number(selfMatrix[3][0], 'f', 2));
    lbPlayerRect31->setText(QString::number(selfMatrix[3][1], 'f', 2));
    lbPlayerRect32->setText(QString::number(selfMatrix[3][2], 'f', 2));
    lbPlayerRect33->setText(QString::number(selfMatrix[3][3], 'f', 2));

    int i = 0;

    for (i = 0; i < CS16_MAX; i++) {
        if (!playerInfos[i].isExist) {
            break;
        }

        if (i == 0)
        {
            this->infoGridModel->setItem(i, 0, new QStandardItem("玩家"));
            lbPlayerCoor->setText(tr("[%1,%2,%3]")
                                  .arg(playerInfos[i].coor[0], 0, 'f', 1, QLatin1Char(' '))
                                  .arg(playerInfos[i].coor[1], 0, 'f', 1, QLatin1Char(' '))
                                  .arg(playerInfos[i].coor[2], 0, 'f', 1, QLatin1Char(' ')));
            lbPlayerAngle->setText(tr("[%1,%2]").arg(selfAngle[0]).arg(selfAngle[1]));
        }
        else
        {
            if (playerInfos[i].team == playerInfos[0].team) {
                this->infoGridModel->setItem(i, 0, new QStandardItem(tr("队友:%1").arg(i)));
            } else {
                this->infoGridModel->setItem(i, 0, new QStandardItem(tr("敌人:%1").arg(i)));
            }
        }

        this->infoGridModel->setItem(i, 1, new QStandardItem(QString::number(playerInfos[i].blood)));
        this->infoGridModel->setItem(i, 2,
                                     new QStandardItem(tr("[%1,%2,%3]")
                                                       .arg(playerInfos[i].coor[0], 0, 'f', 1, QLatin1Char(' '))
                                                       .arg(playerInfos[i].coor[1], 0, 'f', 1, QLatin1Char(' '))
                                                       .arg(playerInfos[i].coor[2], 0, 'f', 1, QLatin1Char(' '))));
        this->infoGridModel->setItem(i, 3, new QStandardItem(QString::number(playerInfos[i].armor)));
        this->infoGridModel->setItem(i, 4, new QStandardItem(QString::number(playerInfos[i].money)));
    }

    auto removeCount = infoGridModel->rowCount() - i;

    if (removeCount > 0) {
        infoGridModel->removeRows(i, removeCount);
    }
}

void ItemView9CS16::clearMessage()
{
    edtMsg->clear();
}

void ItemView9CS16::postMessage(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg);

    QApplication::postEvent(this, event);
}

void ItemView9CS16::appendMessage(const QString& msg)
{
    QString text = edtMsg->toPlainText();

    text += QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

    //    text += QDateTime::currentDateTime().toString("[yyyy-MM-dd
    // hh:mm:ss.zzz] ");
    text += msg;

    if (text.back() != '\n') {
        text += "\n";
    }
    showMessage(text);
}

void ItemView9CS16::showMessage(const QString& msg)
{
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void ItemView9CS16::customEvent(QEvent *e)
{
    switch (e->type())
    {
    case qEventRecvMsg:
    {
        QStringEvent *event = dynamic_cast<QStringEvent *>(e);
        appendMessage(event->message);
    }
        e->accept();
        break;

    default:
        break;
    }
}

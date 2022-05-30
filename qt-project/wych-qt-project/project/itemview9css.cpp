#include "itemview9css.h"
#include "mainwindow.h"
#include "customevent.h"

ItemView9CSS *g_css;

ItemView9CSS::ItemView9CSS(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnect();
    g_css = this;
}

ItemView9CSS::~ItemView9CSS()
{}

void ItemView9CSS::initUI()
{
    auto layout = new QHBoxLayout(this);
    auto leftQWidget = new QWidget(this);
    auto leftQWidgetLayout = new QVBoxLayout(leftQWidget);
    auto leftQWidgetGroupBox1 = new QGroupBox("程序设置", this);

    leftQWidgetLayout->addWidget(leftQWidgetGroupBox1);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout = new QGridLayout(leftQWidgetGroupBox1);
    leftQWidgetGroupBox1->setFixedHeight(200);

    auto leftQWidgetGroupBox2 = new QGroupBox("自瞄设置", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox2);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout2 = new QGridLayout(leftQWidgetGroupBox2);
    leftQWidgetGroupBox2->setFixedHeight(130);

    auto leftQWidgetGroupBox3 = new QGroupBox("显示设置", this);
    leftQWidgetLayout->addWidget(leftQWidgetGroupBox3);
    leftQWidgetLayout->setAlignment(Qt::AlignTop);
    auto leftQWidgetGroup1Layout3 = new QGridLayout(leftQWidgetGroupBox3);
    leftQWidgetGroupBox3->setFixedHeight(90);


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
    ckConsoleEnable->setChecked(true);
    ckRefreshClients = new QCheckBox("刷新状态");
    btnStartStop = new QPushButton("启动辅助");
    btnConsoleClear = new QPushButton("清空控制台");
    btnClients = new QPushButton("刷新状态");


    sbRefresh = new QSpinBox();
    sbRefresh->setMinimum(1);
    sbRefresh->setMaximum(1000);
    sbRefresh->setSuffix(" x10 ms");
    sbRefresh->setValue(5);

    ckVersion = new QComboBox();

    GameInfo css(L"hl2.exe", L"Valve001", L"Counter-Strike Source");
    ckVersion->addItem("v6630498(20211230)", QVariant::fromValue(css));
    ckAim = new QCheckBox("启动自瞄");
    ckShowEnemy = new QCheckBox("显示敌人方框");
    ckShowEnemy->setChecked(true);
    ckShowFriend = new QCheckBox("显示队友方框");


    // 追加
    rbAimByDistance = new QRadioButton("按距离");
    rbAimByCross = new QRadioButton("按准星距离");
    rbAimByDistance2 = new QRadioButton("距离优先");
    rbAimByCross->setChecked(true);
    sbMaxDistance = new QSpinBox();
    sbMaxDistance->setMinimum(1);
    sbMaxDistance->setMaximum(10);
    sbMaxDistance->setSuffix("米内");
    sbMaxDistance->setValue(2);

    bgAim = new QButtonGroup(this);
    bgAim->addButton(rbAimByDistance,  0);
    bgAim->addButton(rbAimByCross,     1);
    bgAim->addButton(rbAimByDistance2, 2);

    rbDrawRect = new QRadioButton("显示方框");
    rbDrawRect->setChecked(true);
    rbDrawText = new QRadioButton("显示文字");
    bgDraw = new QButtonGroup(this);
    bgDraw->addButton(rbDrawRect, 0);
    bgDraw->addButton(rbDrawText, 1);

    edtMsg = new QPlainTextEdit();
    edtMsg->setReadOnly(true);


    infoTableView = new QTableView(this);
    infoGridModel = new QStandardItemModel();

    /* 设置表格标题行(输入数据为QStringList类型) */
    infoGridModel->setHorizontalHeaderLabels({  "类型", "血量",  "偏航角|俯仰角", "坐标", "护甲", "金钱" });
    infoTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    infoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    infoTableView->setModel(infoGridModel);


    leftQWidgetGroup1Layout->addWidget(ckVersion,    1, 0);
    leftQWidgetGroup1Layout->addWidget(sbRefresh,    2, 0);
    leftQWidgetGroup1Layout->addWidget(ckShowFriend, 3, 0);
    leftQWidgetGroup1Layout->addWidget(ckShowEnemy,  4, 0);
    leftQWidgetGroup1Layout->addWidget(ckAim,        5, 0);
    leftQWidgetGroup1Layout->addWidget(btnStartStop, 6, 0);

    leftQWidgetGroup1Layout2->addWidget(rbAimByDistance,   8, 0);
    leftQWidgetGroup1Layout2->addWidget(rbAimByCross,      9, 0);
    leftQWidgetGroup1Layout2->addWidget(rbAimByDistance2, 10, 0);
    leftQWidgetGroup1Layout2->addWidget(sbMaxDistance,    10, 1);

    leftQWidgetGroup1Layout3->addWidget(rbDrawRect, 8, 0);
    leftQWidgetGroup1Layout3->addWidget(rbDrawText, 9, 0);

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

LRESULT ItemView9CSS::WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_PAINT:

        if (g_css->dx->g_pd3dDevice != NULL) {
            g_css->dx->drawStart();

            auto playerInfos = g_css->playerInfos;

            for (int i = 1; i < CSS_MAX; i++) {
                if (playerInfos[i].isShow) {
                    int height = playerInfos[i].to_height_w - playerInfos[i].to_height_h;
                    int width =  height / CSS_rect_height_width_radio;
                    int y =   playerInfos[i].to_height_h;
                    int x =   playerInfos[i].to_width - width / 2;

                    if (g_css->selfTeam == playerInfos[i].team) {
                        g_css->dx->drawRect(x, y, width, height, g_css->fontSize, g_css->colorGreen);
                    } else {
                        g_css->dx->drawRect(x, y, width, height, g_css->fontSize, g_css->colorRed);
                    }
                }
            }

            //            g_css->dx->drawLine(D3DCOLOR_ARGB(255, 0, 0, 255), 20, 20, 66, 66, 线粗);
            //            g_css->dx->drawRect(100, 100, 100, 100, 线粗, D3DCOLOR_ARGB(255, 255, 255, 0));
            //            g_css->dx->drawText(200, 200, "555", D3DCOLOR_ARGB(255, 255, 0, 255));

            g_css->dx->drawEnd();
        }

        break;

    case WM_CREATE:
        DwmExtendFrameIntoClientArea(hWnd, &g_css->Margin);
        break;

    case WM_DESTROY:
    {
        //        g_css->dx->release();
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
    auto obj = (ItemView9CSS *)param;
    auto playerInfos = obj->playerInfos;
    auto selfMatrix = obj->selfMatrix;


    while (true)
    {
        if (!obj->isStart) {
            break;
        }

        // 使辅助窗口一直盖在游戏窗口上
        if (obj->gameHwnd)
        {
            float aimCoor[3]{ 0, 0, 0 };
            int   aim_min = INT_MAX;
            int   aim_index = 0;
            int   aim_distance = INT_MAX;
            int   aim_distance_index = 0;

            int self_matrix_address = obj->engine_module.module_address + CSS_self_matrix_offset;
            WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, self_matrix_address, &obj->selfMatrix, sizeof(float[4][4]));

            int self_angle_address = obj->engine_module.module_address + CSS_self_angle_offset;
            WychUtils_WinAPI::read_memory(obj->gameProcessHwnd,  self_angle_address, &obj->selfAngle,  sizeof(float[2]));


            //            int self_address = obj->cstrike_module.module_address + 0x11069BC;
            int self_address_server = obj->server_module.module_address + CSS_player_list_offset;

            for (int i = 0; i < CSS_MAX; i++) {
                playerInfos[i].isExist = false;
                int location_base_address;
                WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, self_address_server, &location_base_address, sizeof(int));

                if (location_base_address) {
                    WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  CSS_player_blood_offset, &playerInfos[i].blood, sizeof(int));
                    WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  CSS_player_armor_offset, &playerInfos[i].armor, sizeof(int));
                    WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  CSS_player_pos_offset,   &playerInfos[i].coor,
                                                  sizeof(float[3]));
                    WychUtils_WinAPI::read_memory(obj->gameProcessHwnd, location_base_address +  CSS_player_team_offset,  &playerInfos[i].team,  sizeof(int));

                    playerInfos[i].isExist = true;
                }
                self_address_server +=  CSS_player_next_offset;
            }

            obj->selfTeam = playerInfos[0].team;

            // 1.启动绘制敌我信息
            if (obj->ckShowFriend->isChecked() || obj->ckShowEnemy->isChecked() || obj->ckAim->isChecked()) {
                // 计算绘制信息
                int height = obj->gameRect.bottom / 2;
                int width = obj->gameRect.right / 2;

                for (int i = 1; i < CSS_MAX; i++) {
                    // 是否存在
                    if (!playerInfos[i].isExist) {
                        playerInfos[i].isShow = false;
                        continue;
                    }

                    // 阵营
                    if (obj->selfTeam == playerInfos[i].team) {
                        if (!obj->ckShowFriend->isChecked()) {
                            playerInfos[i].isShow = false;
                            continue;
                        }
                    } else {
                        if (!obj->ckShowEnemy->isChecked()) {
                            playerInfos[i].isShow = false;
                            continue;
                        }
                    }

                    // 是否死亡
                    if (playerInfos[i].blood <= 1) {
                        playerInfos[i].isShow = false;
                        continue;
                    }

                    // 转向 (竖矩阵)
                    float to_target = selfMatrix[2][0] * playerInfos[i].coor[0]
                                      + selfMatrix[2][1] * playerInfos[i].coor[1]
                                      + selfMatrix[2][2] * playerInfos[i].coor[2]
                                      + selfMatrix[2][3];

                    // 人物距离计算
                    int value = sqrt((playerInfos[0].coor[0] - playerInfos[i].coor[0]) * (playerInfos[0].coor[0] - playerInfos[i].coor[0])
                                     + (playerInfos[0].coor[1] - playerInfos[i].coor[1]) * (playerInfos[0].coor[1] - playerInfos[i].coor[1])
                                     + (playerInfos[0].coor[2] - playerInfos[i].coor[2]) * (playerInfos[0].coor[2] - playerInfos[i].coor[2]));
                    playerInfos[i].distance = value;

                    if (obj->selfTeam != playerInfos[i].team) {
                        if (value < aim_distance)
                        {
                            aim_distance = value;
                            aim_distance_index = i;
                        }
                    }


                    // 后面的人物不做处理
                    if (to_target < 0.01f) {
                        playerInfos[i].isShow = false;
                        continue;
                    }


                    // 比例
                    to_target = 1.0f / to_target;

                    // (竖矩阵)
                    int to_width = width + (selfMatrix[0][0] * playerInfos[i].coor[0]
                                            + selfMatrix[0][1] * playerInfos[i].coor[1]
                                            + selfMatrix[0][2] * playerInfos[i].coor[2]
                                            + selfMatrix[0][3]) * to_target * width;

                    int to_height_h = height - (selfMatrix[1][0] * playerInfos[i].coor[0]
                                                + selfMatrix[1][1] * playerInfos[i].coor[1]
                                                + selfMatrix[1][2] * (playerInfos[i].coor[2] CSS_rect_height_top)
                                                + selfMatrix[1][3]) * to_target * height;

                    int to_height_w = height - (selfMatrix[1][0] * playerInfos[i].coor[0]
                                                + selfMatrix[1][1] * playerInfos[i].coor[1]
                                                + selfMatrix[1][2] * (playerInfos[i].coor[2] CSS_rect_height_bottom)
                                                + selfMatrix[1][3]) * to_target * height;


                    playerInfos[i].to_width = to_width;
                    playerInfos[i].to_height_h = to_height_h;
                    playerInfos[i].to_height_w = to_height_w;
                    playerInfos[i].isShow = true;


                    if (obj->ckAim->isChecked()) {
                        if (obj->selfTeam != playerInfos[i].team) {
                            // 计算准星距离
                            int value = abs(width - to_width) + abs(height - to_height_h);

                            if (value < aim_min)
                            {
                                aim_min = value;
                                aim_index = i;
                            }
                        }
                    }
                }
            }


            //            if (obj->ckShowFriend->isChecked() || obj->ckShowEnemy->isChecked()) {
            // 处理窗口消息
            MSG Message;
            ZeroMemory(&Message, sizeof(Message));

            if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
            {
                DispatchMessage(&Message);
                TranslateMessage(&Message);
            }

            //            }

            // 2.启动自瞄
            if (obj->ckAim->isChecked()) {
                // 先计算偏航角
                for (int i = 1; i < CSS_MAX; i++) {
                    if (obj->selfTeam != playerInfos[i].team) {
                        // 偏航角 (同时支持-180~180)
                        // 这里用的是我们减去敌人
                        float x = playerInfos[i].coor[0] - playerInfos[0].coor[0];
                        float y =   playerInfos[i].coor[1]  - playerInfos[0].coor[1];
                        float z =    playerInfos[i].coor[2] - playerInfos[0].coor[2];
                        float angle = atan(y / x);                                                        // 弧度

                        if ((x >= 0.0f) && (y >= 0.0f)) playerInfos[i].angle[1] = angle / M_PI * 180.0f;  // 不同游戏根据视角不同这里也得变，有的可能是-180~180 有的是0~360
                        else if ((x < 0.0f) && (y >= 0.0f)) playerInfos[i].angle[1] = angle / M_PI * 180.0f + 180.0f;
                        else if ((x < 0.0f) && (y < 0.0f)) playerInfos[i].angle[1] = angle / M_PI * 180.0f + 180.0f;
                        else if ((x >= 0.0f) && (y < 0.0f)) playerInfos[i].angle[1] = angle / M_PI * 180.f + 360.0f;

                        // 俯仰角
                        playerInfos[i].angle[0] = atan(z / sqrt(x * x + y * y)) / M_PI * 180.f * -1;
                    }
                }

                if (aim_index > 0) {
                    if (GetAsyncKeyState(VK_MENU) & 0x8000) {
                        WychUtils_WinAPI::write_memory(obj->gameProcessHwnd, self_angle_address, &playerInfos[aim_index].angle, sizeof(float[2]));
                    }
                }
            }
        }

        Sleep(obj->sbRefresh->value());
    }
}

unsigned __stdcall ItemView9CSS::Start(void *param) {
    auto thisObj = (ItemView9CSS *)param;

    // 1.获取进程ID
    GameInfo gameInfo = thisObj->ckVersion->currentData().value<GameInfo>();

    auto pid = WychUtils_WinAPI::get_process_id(gameInfo.process_name);

    if (!pid) {
        thisObj->postMessage(tr("进程未找到![%1]").arg(gameInfo.process_name));
        thisObj->isStart = false;
        return 1;
    }

    // 2.获取进程句柄
    thisObj->gameProcessHwnd = WychUtils_WinAPI::get_process_handle(pid);

    if (!thisObj->gameProcessHwnd) {
        thisObj->postMessage(tr("进程句柄获取失败![%1]").arg(gameInfo.process_name));
        thisObj->isStart = false;
        return 1;
    }


    // 3.获取进程中的dll
    WychUtils_WinAPI::get_module_info(thisObj->gameProcessHwnd, pid, L"engine.dll", thisObj->engine_module);
    WychUtils_WinAPI::get_module_info(thisObj->gameProcessHwnd, pid, L"server.dll", thisObj->server_module);


    thisObj->gameHwnd = FindWindow(gameInfo.class_name, gameInfo.window_name);

    if (!thisObj->gameHwnd) {
        thisObj->postMessage(tr("程序窗口获取失败![%1]").arg(gameInfo.window_name));
        thisObj->isStart = false;
        return 1;
    }

    // 创建透明窗口(游戏窗口, 绘制);
    thisObj->newHwnd = WychUtils::CreateTopWindow(thisObj->gameHwnd, ItemView9CSS::WinProc);

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

unsigned __stdcall ItemView9CSS::Resize(void *param) {
    auto obj = (ItemView9CSS *)param;

    while (true)
    {
        if (!obj->isStart) {
            break;
        }

        // 使辅助窗口一直盖在游戏窗口上
        if (obj->gameHwnd)
        {
            GetClientRect(obj->gameHwnd, &obj->gameRect);
            obj->gamePoint.x = 0;
            obj->gamePoint.y = 0;
            ClientToScreen(obj->gameHwnd, &obj->gamePoint);
            MoveWindow(obj->newHwnd, obj->gamePoint.x, obj->gamePoint.y, obj->gameRect.right, obj->gameRect.bottom, true);
        }
        Sleep(2000);
    }
    return 1;
}

void ItemView9CSS::initConnect()
{
    connect(btnConsoleClear, &QPushButton::clicked, [this]() {
        clearMessage();
    });

    connect(btnStartStop, &QPushButton::clicked, [this]() {
        if (!isStart) {
            isStart = true;
            unsigned threadid;
            HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &ItemView9CSS::Resize, this, NULL, &threadid);
            CloseHandle(hThread);
            HANDLE hThread2 = (HANDLE)_beginthreadex(NULL, 0, &ItemView9CSS::Start, this, NULL, &threadid);
            CloseHandle(hThread2);

            btnStartStop->setText(tr("关闭辅助"));
        } else {
            isStart = false;
            btnStartStop->setText(tr("启动辅助"));
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
    clientTimer->start(100);
}

void ItemView9CSS::refreshForm() {
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

    for (i = 0; i < CSS_MAX; i++) {
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

        if (playerInfos[i].team == playerInfos[0].team) {
            this->infoGridModel->setItem(i, 2, new QStandardItem(tr("")));
        } else {
            this->infoGridModel->setItem(i, 2, new QStandardItem(tr("%1 | %2")
                                                                 .arg(playerInfos[i].angle[1], 0, 'f', 1, QLatin1Char(' '))
                                                                 .arg(playerInfos[i].angle[0], 0, 'f', 1, QLatin1Char(' '))));
        }


        this->infoGridModel->setItem(i, 3,
                                     new QStandardItem(tr("[%1,%2,%3]")
                                                       .arg(playerInfos[i].coor[0], 0, 'f', 1, QLatin1Char(' '))
                                                       .arg(playerInfos[i].coor[1], 0, 'f', 1, QLatin1Char(' '))
                                                       .arg(playerInfos[i].coor[2], 0, 'f', 1, QLatin1Char(' '))));
        this->infoGridModel->setItem(i, 4, new QStandardItem(QString::number(playerInfos[i].armor)));
        this->infoGridModel->setItem(i, 5, new QStandardItem(QString::number(playerInfos[i].money)));
    }

    auto removeCount = infoGridModel->rowCount() - i;

    if (removeCount > 0) {
        infoGridModel->removeRows(i, removeCount);
    }
}

void ItemView9CSS::clearMessage()
{
    edtMsg->clear();
}

void ItemView9CSS::postMessage(const QString& msg)
{
    QStringEvent *event = new QStringEvent(msg);

    QApplication::postEvent(this, event);
}

void ItemView9CSS::appendMessage(const QString& msg)
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

void ItemView9CSS::showMessage(const QString& msg)
{
    edtMsg->setPlainText(msg);
    QTextCursor cursor = edtMsg->textCursor();
    cursor.movePosition(QTextCursor::End);
    edtMsg->setTextCursor(cursor);
    edtMsg->repaint();
}

void ItemView9CSS::customEvent(QEvent *e)
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

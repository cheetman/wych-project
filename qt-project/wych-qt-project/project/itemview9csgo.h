#ifndef ITEMVIEW9CSGO_H
#define ITEMVIEW9CSGO_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QTextEdit>
#include <QApplication>
#include <QStandardItemModel>
#include <QTableView>
#include <QPlainTextEdit>
#include <QCheckBox>
#include "hv.h"
#include "evpp/TcpServer.h"
#include <d3d9types.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>
#include <QSpinBox>
#include "window.h"
#include "winapi.h"
#include "configs/csgo.hpp"

#define CSGO_MAX 32
#define CSGO_self_matrix_offset hazedumper::signatures::dwViewMatrix // 矩阵
#define CSGO_self_client_offset hazedumper::signatures::dwClientState  // 视角
#define CSGO_self_client_angle_offset hazedumper::signatures::dwClientState_ViewAngles  // 视角
#define CSGO_player_list_offset hazedumper::signatures::dwEntityList // 玩家列表
#define CSGO_player_next_offset 0x10     // 玩家next偏移
#define CSGO_player_team_offset hazedumper::netvars::m_iTeamNum    // 玩家列表
#define CSGO_player_blood_offset hazedumper::netvars::m_iHealth    // 玩家列表  - 血量
#define CSGO_player_armor_offset hazedumper::netvars::m_ArmorValue   // 玩家列表
#define CSGO_player_pos_offset hazedumper::netvars::m_vecOrigin     // 玩家列表

#define CSGO_rect_height_top +75.0f      // 矩形上边框
#define CSGO_rect_height_bottom -5.0f    // 矩形下边框
#define CSGO_rect_height_width_radio 2.5 // 矩形宽高比

class ItemView9CSGO : public QWidget {
    Q_OBJECT

public:

    explicit ItemView9CSGO(QWidget *parent = nullptr);
    ~ItemView9CSGO();

    HWND newHwnd, gameHwnd;
    HANDLE gameProcessHwnd;
    RECT gameRect{ 0, 0, 0, 0 };
    POINT gamePoint{ 0, 0 };
    WychUtils::DX9 *dx;
    MARGINS Margin;

    bool isStart = false;
    struct WychUtils_WinAPI::module_information client_module, engine_module;


    struct PlayerInfo
    {
        bool  isExist = false;       // 是否存在
        float coor[3] = { 0, 0, 0 }; // 坐标
        int   blood = 0;             // blood
        int   armor = 0;             // armor
        int   money = 0;             // 钱
        int   team = 0;              // 阵营

        // 绘制相关
        bool  isShow = false;        // 是否绘制
        float to_target = 0;         // 朝向
        int   to_width = 0;          // 朝向
        int   to_height_h = 0;       // 朝向
        int   to_height_w = 0;       // 朝向


        float angle[2] = { 0, 0 };   // 偏航角和俯仰角
    };
    PlayerInfo playerInfos[CSGO_MAX];

    struct GameInfo
    {
public:

        GameInfo() {}

        GameInfo(const wchar_t *process_name, const wchar_t *class_name, const wchar_t *window_name) {
            this->process_name = process_name;
            this->class_name = class_name;
            this->window_name = window_name;
        }

        const wchar_t *process_name;
        const wchar_t *class_name;
        const wchar_t *window_name;
    };


    float selfCoor[3] = { 0, 0, 0 };
    float selfMatrix[4][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    float selfAngle[2] = { 0, 0 };
    int selfTeam = 0;

    QSpinBox *sbRefresh;
    QCheckBox *ckAim;
    QCheckBox *ckShowEnemy;
    QCheckBox *ckShowFriend;
    QComboBox *ckVersion;

protected:

    void                      initUI();
    void                      initConnect();
    void                      postMessage(const QString& msg);
    void                      clearMessage();

    void                      appendMessage(const QString& msg);
    void                      showMessage(const QString& msg);
    virtual void              customEvent(QEvent *e);

    static unsigned __stdcall Start(void *param);
    static unsigned __stdcall Resize(void *param);

    static LRESULT            WinProc(HWND   hWnd,
                                      UINT   Message,
                                      WPARAM wParam,
                                      LPARAM lParam);

    int fontSize = 1;
    int colorRed = D3DCOLOR_XRGB(255, 0, 0);   // D3DCOLOR_ARGB(255, 255, 0, 0)
    int colorGreen = D3DCOLOR_XRGB(0, 255, 0); // D3DCOLOR_ARGB(255, 255, 0, 0)

private:

    QPushButton *btnStartStop;
    QPushButton *btnClients;
    QPushButton *btnConsoleClear;
    QCheckBox *ckConsoleEnable;
    QCheckBox *ckRefreshClients;

    QPlainTextEdit *edtMsg;

    QStandardItemModel *infoGridModel;
    QTableView *infoTableView;


    QLabel *lbPlayerCoor = new QLabel("[]");
    QLabel *lbPlayerAngle = new QLabel("[]");
    QLabel *lbPlayerRect00 = new QLabel;
    QLabel *lbPlayerRect01 = new QLabel;
    QLabel *lbPlayerRect02 = new QLabel;
    QLabel *lbPlayerRect03 = new QLabel;
    QLabel *lbPlayerRect10 = new QLabel;
    QLabel *lbPlayerRect11 = new QLabel;
    QLabel *lbPlayerRect12 = new QLabel;
    QLabel *lbPlayerRect13 = new QLabel;
    QLabel *lbPlayerRect20 = new QLabel;
    QLabel *lbPlayerRect21 = new QLabel;
    QLabel *lbPlayerRect22 = new QLabel;
    QLabel *lbPlayerRect23 = new QLabel;
    QLabel *lbPlayerRect30 = new QLabel;
    QLabel *lbPlayerRect31 = new QLabel;
    QLabel *lbPlayerRect32 = new QLabel;
    QLabel *lbPlayerRect33 = new QLabel;

    void refreshForm();

signals:
};


Q_DECLARE_METATYPE(ItemView9CSGO::GameInfo)

#endif // ITEMVIEW9CSGO_H

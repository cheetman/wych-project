#ifndef ITEMVIEW9L4D2_H
#define ITEMVIEW9L4D2_H

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

#define L4D2_MAX 1000
#define L4D2_self_matrix_offset 0x600F9C // 矩阵
#define L4D2_self_angle_offset 0x6B2080  // 视角

#define L4D2_player_list_offset 0x7DC774 // 玩家列表 2B03C774 2B03DF64  server.dll+7DE0C4(2B03E0C4)
#define L4D2_player_next_offset 0x10     // 玩家next偏移
#define L4D2_player_team_offset 0xE0     // 玩家列表
#define L4D2_player_blood_offset 0xEC    // 玩家列表  - 血量
#define L4D2_player_maxblood_offset 0xE8 // 玩家列表  - 血量
#define L4D2_player_armor_offset 0xD08   // 玩家列表
#define L4D2_player_pos_offset 0x2CC     // 玩家列表

#define L4D2_rect_height_top +75.0f      // 矩形上边框
#define L4D2_rect_height_bottom -5.0f    // 矩形下边框
#define L4D2_rect_height_width_radio 2.5 // 矩形宽高比

class ItemView9L4D2 : public QWidget {
    Q_OBJECT

public:

    explicit ItemView9L4D2(QWidget *parent = nullptr);
    ~ItemView9L4D2();

    HWND newHwnd, gameHwnd;
    HANDLE gameProcessHwnd;
    RECT gameRect{ 0, 0, 0, 0 };
    POINT gamePoint{ 0, 0 };
    WychUtils::DX9 *dx;
    MARGINS Margin;

    bool isStart = false;
    struct WychUtils_WinAPI::module_information main_module, server_module, engine_module, materialsystem_module;


    struct PlayerInfo
    {
        bool  isExist = false;       // 是否存在
        float coor[3] = { 0, 0, 0 }; // 坐标
        int   blood = 0;             // blood
        int   armor = 0;             // armor
        int   money = 0;             // 钱
        int   team = 0;              // 阵营
        int   address = 0;           // 阵营


        int custom1 = 0;             // 自定义
        int maxblood = 0;            // 自定义

        // 绘制相关
        bool  isShow = false;        // 是否绘制
        float to_target = 0;         // 朝向
        int   to_width = 0;          // 朝向
        int   to_height_h = 0;       // 朝向
        int   to_height_w = 0;       // 朝向


        float angle[2] = { 0, 0 };   // 偏航角和俯仰角
    };

    //    PlayerInfo *playerInfos = new PlayerInfo[L4D2_MAX];

    PlayerInfo playerInfos[L4D2_MAX];


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


Q_DECLARE_METATYPE(ItemView9L4D2::GameInfo)

#endif // ITEMVIEW9L4D2_H

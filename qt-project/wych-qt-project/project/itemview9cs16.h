#ifndef ITEMVIEW9CS16_H
#define ITEMVIEW9CS16_H

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
#include <QRadioButton>
#include <QButtonGroup>
#include "hv.h"
#include "evpp/TcpServer.h"
#include <d3d9types.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>
#include <QSpinBox>
#include "window.h"
#include "winapi.h"

#define CS16_MAX 32
#define CS16_self_matrix_offset 0x1820100 // 矩阵
#define CS16_self_angle_offset 0x19E10C4  // 视角
#define CS16_player_list_offset 0x97030   // 玩家列表
#define CS16_player_next_offset 0x230     // 玩家next偏移
#define CS16_player_1_offset 0x7c         // 玩家列表 - 1级偏移
#define CS16_player_1_money_offset 0x1CC  // 玩家列表 - 1级偏移
#define CS16_player_1_team_offset 0x1C8   // 玩家列表 - 1级偏移
#define CS16_player_2_offset 0x4          // 玩家列表 - 2级偏移
#define CS16_player_2_blood_offset 0x160  // 玩家列表 - 2级偏移
#define CS16_player_2_armor_offset 0x1BC  // 玩家列表 - 2级偏移
#define CS16_player_2_pos_offset 0x8      // 玩家列表 - 2级偏移

#define CS16_rect_height_top  +30.0f      // 矩形上边框
#define CS16_rect_height_bottom -34.0f    // 矩形下边框
#define CS16_rect_height_width_radio 2.5  // 矩形宽高比

class ItemView9CS16 : public QWidget {
    Q_OBJECT

public:

    explicit ItemView9CS16(QWidget *parent = nullptr);
    ~ItemView9CS16();

    HWND newHwnd, gameHwnd;
    HANDLE gameProcessHwnd;
    RECT gameRect{ 0, 0, 0, 0 };
    POINT gamePoint{ 0, 0 };
    WychUtils::DX9 *dx;
    MARGINS Margin;

    bool isStart = false;
    struct WychUtils_WinAPI::module_information amxmodx_mm_module, cstrike_module;


    struct PlayerInfo
    {
        bool  isExist = false;       // 是否存在
        float coor[3] = { 0, 0, 0 }; // 坐标
        float blood = 0;             // blood
        float armor = 0;             // armor
        int   money = 0;             // 钱
        int   team = 0;              // 阵营

        // 绘制相关
        bool  isShow = false;        // 是否绘制
        float to_target = 0;         // 朝向
        int   to_width = 0;          // 朝向
        int   to_height_h = 0;       // 朝向
        int   to_height_w = 0;       // 朝向


        float angle[2] = { 0, 0 };   // 偏航角和俯仰角

        int distance = 0;
    };
    PlayerInfo playerInfos[CS16_MAX];

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

    QRadioButton *rbAimByDistance;
    QRadioButton *rbAimByDistance2;
    QSpinBox *sbMaxDistance;
    QRadioButton *rbAimByCross;
    QButtonGroup *bgAim;

    QRadioButton *rbDrawRect;
    QRadioButton *rbDrawText;
    QButtonGroup *bgDraw;

    QCheckBox *ckSelfHp;
    QCheckBox *ckTeamHp;
    QCheckBox *ckEnemyHp;
    QSpinBox *sbSelfHp;
    QSpinBox *sbTeamHp;
    QSpinBox *sbEnemyHp;

protected:

    void                      initUI();
    void                      initConnect();
    void                      postMessage(const QString& msg);
    void                      clearConsole();

    void                      appendConsole(const QString& msg);
    void                      writeConsole(const QString& msg);
    virtual void              customEvent(QEvent *e);

    static unsigned __stdcall Start(void *param);
    static unsigned __stdcall Resize(void *param);

    static LRESULT            WinProc(HWND   hWnd,
                                      UINT   Message,
                                      WPARAM wParam,
                                      LPARAM lParam);

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


Q_DECLARE_METATYPE(ItemView9CS16::GameInfo)

#endif // ITEMVIEW9CS16_H

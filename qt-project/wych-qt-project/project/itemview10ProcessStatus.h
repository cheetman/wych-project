#ifndef Itemview10ProcessStatus_H
#define Itemview10ProcessStatus_H

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
#include <QGroupBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <Windows.h>
#include "winapi.h"

#include <components/capturebtn.h>
#include <events/eventstatusgrid.h>

#include <threads/threadscript.h>


typedef int  (*__stdcall SysMsgCallBack)(int, WPARAM, LPARAM);
typedef  void (*SetSysMsgCall)(SysMsgCallBack);
typedef  void (*SetHook)(int);


class Itemview10ProcessStatus : public QWidget {
    Q_OBJECT

public:

    explicit Itemview10ProcessStatus(QWidget *parent = nullptr);
    ~Itemview10ProcessStatus();

    bool start(int         port,
               const char *host = "0.0.0.0");

    // 读入进程
    bool buildProcess(DWORD pid);

    // 窗口更新
    bool updateWindowInfo(HWND handleWindow);
    bool isStart = false;
    class QSpinBox *sb_script_sleep;


    void postAppendConsole(const QString& msg);
    HWND activeWindowHandle = 0;


    static int __stdcall MysysMsgCallBack(int, WPARAM, LPARAM);

    //    void postAppendMessageText(class EventWinMessage *event);

    void appendMessageText(const QString&);

protected:

    void         initUI();
    void         initConnect();
    void         clearConsole();
    void         clearMessageText();
    void         appendConsole(const QString& msg);
    void         writeConsole(const QString& msg);
    virtual void customEvent(QEvent *e);

    //    static unsigned __stdcall RefreshScript(void *param);

private:

    HMODULE g_moduleMessage;
    HHOOK g_messageHook = NULL;
    HHOOK g_mouseHook = NULL;
    HOOKPROC hkMouseProc = NULL;
    HOOKPROC hkMessageProc = NULL;
    SetSysMsgCall hkSetSysMsgCall;
    SetHook hkSetHook;

    void buildMessageText(QEvent::Type, class EventWinMessage *);

    bool drawWindowFrame(HWND);
    bool eraseWindowFrame(HWND);

    bool readConfigDefaultHWND(QString process,
                               int    *HWND);
    bool writeConfigDefaultHWND(QString process,
                                int     HWND);


    void recursionScriptSave(QJsonObject      & json,
                             const QModelIndex& now = QModelIndex());
    bool recursionScriptSaveCheck(
        const QModelIndex& now = QModelIndex());

    void recursionScriptShow(QJsonObject  & json,
                             QStandardItem *now = NULL);

    void updateScriptStatus(QStandardItem            *item,
                            EventStatusGrid::GridType type);

    void updateScriptCount(QStandardItem *item);

    void postUpdateScriptStatus(QStandardItem            *item,
                                EventStatusGrid::GridType type);

    void postUpdateScriptCount(QStandardItem *item);

    ThreadScript *threadScript;


    WIN32_PROCESS_INFO processInfo = {
        0
    };

    WIN32_WINDOW_INFO windowInfo = { 0 };
    float dpi_window = 1.0f;

    QLineEdit *dos_e_magic;
    QLineEdit *dos_e_lfanew;

    QLineEdit *pe_Machine;
    QLineEdit *pe_NumberOfSections;
    QLineEdit *pe_TimeDateStamp;
    QLineEdit *pe_PointerOfSymbolTable;
    QLineEdit *pe_NumberOfSymbols;
    QLineEdit *pe_SizeOfOptionHeader;
    QLineEdit *pe_Characteristics;

    QLineEdit *ope_Magic;
    QLineEdit *ope_MajorLinkerVersion;
    QLineEdit *ope_MinorLinkerVersion;
    QLineEdit *ope_SizeOfCode;
    QLineEdit *ope_SizeOfInitializedData;
    QLineEdit *ope_SizeOfUninitializedData;
    QLineEdit *ope_AddressOfEntryPoint_RVA;
    QLineEdit *ope_BaseOfCode;
    QLineEdit *ope_BaseOfData;
    QLineEdit *ope_ImageBase;
    QLineEdit *ope_SectionAlignment;
    QLineEdit *ope_FileAlignment;
    QLineEdit *ope_MajorOperatingSystemVersion;
    QLineEdit *ope_MinorOperatingSystemVersion;
    QLineEdit *ope_MajorImageVersion;
    QLineEdit *ope_MinorImageVersion;
    QLineEdit *ope_MajorSubsystemVersion;
    QLineEdit *ope_MinorSubsystemVersion;
    QLineEdit *ope_Win32VersionValue;
    QLineEdit *ope_SizeOfImage;
    QLineEdit *ope_SizeOfHeaders;
    QLineEdit *ope_CheckSum;
    QLineEdit *ope_Subsystem;
    QLineEdit *ope_DllCharacteristics;
    QLineEdit *ope_SizeOfStackReserve;
    QLineEdit *ope_SizeOfStackCommit;
    QLineEdit *ope_SizeOfHeapReserve;
    QLineEdit *ope_SizeOfHeapCommit;
    QLineEdit *ope_LoaderFlags;
    QLineEdit *ope_NumberOfRvaAndSizes;
    QLineEdit *ope_DataDirectory;

    QLineEdit *tb_export_rva;
    QLineEdit *tb_import_rva;
    QLineEdit *tb_resource_rva;
    QLineEdit *tb_base_relocation_rva;
    QLineEdit *tb_process_name;
    QLineEdit *tb_window_handle2;
    QLineEdit *tb_process_handle;
    QLineEdit *tb_window_name;
    QLineEdit *tb_window_handle;
    HWND active_window_handle;

    QLineEdit *tb_window_size;
    QLineEdit *tb_window2_size;
    QLineEdit *tb_window_position;
    QLineEdit *tb_window2_position;

    //    QLineEdit *tb_mouse_position;
    //    QLineEdit *tb_mouse_ratio;
    //    QLineEdit *tb_mouse_ratio_position;
    //    QLineEdit *tb_mouse_rgb;

    QLineEdit *tb_check_position_x;
    QLineEdit *tb_check_position_y;
    QLineEdit *tb_check_position_x_radio;
    QLineEdit *tb_check_position_y_radio;
    QLineEdit *tb_check_position_x_radio_v;
    QLineEdit *tb_check_position_y_radio_v;
    QLineEdit *tb_check_position_r;
    QLineEdit *tb_check_position_g;
    QLineEdit *tb_check_position_b;
    QLineEdit *tb_check_position_a;
    QLineEdit *tb_check_position_r_up;
    QLineEdit *tb_check_position_g_up;
    QLineEdit *tb_check_position_b_up;
    QLineEdit *tb_check_position_a_up;
    QLineEdit *tb_check_position_r_down;
    QLineEdit *tb_check_position_g_down;
    QLineEdit *tb_check_position_b_down;
    QLineEdit *tb_check_position_a_down;
    QLineEdit *tb_check_remark;
    QCheckBox *ck_check_rgb_up;
    QCheckBox *ck_check_rgb_down;
    QCheckBox *ck_check_color_get;
    QLabel *tb_check_color;
    QLabel *tb_check_print_size;
    QPushButton *btn_color_select;

    int check_print_width;
    int check_print_height;
    class QRadioButton *rb_check_position;
    class QRadioButton *rb_check_position_radio;
    class QButtonGroup *bg_check_position_type;

    QLineEdit *tb_click_position_x;
    QLineEdit *tb_click_position_y;
    QLineEdit *tb_click_position_x_radio;
    QLineEdit *tb_click_position_y_radio;
    QLineEdit *tb_click_position_x_radio_v;
    QLineEdit *tb_click_position_y_radio_v;
    QCheckBox *ck_click_position_get;
    QLabel *tb_click_print_size;
    int click_print_width;
    int click_print_height;
    class QRadioButton *rb_click_position;
    class QRadioButton *rb_click_position_radio;
    class QButtonGroup *bg_click_position_type;


    QLineEdit *tb_export_foa;
    QLineEdit *tb_import_foa;
    QLineEdit *tb_resource_foa;
    QLineEdit *tb_base_relocation_foa;


    QLineEdit *export_Name;
    QLineEdit *export_Base;
    QLineEdit *export_NumberOfFunctions;
    QLineEdit *export_NumberOfNames;
    QLineEdit *export_AddressOfFunctions;    // RVA from base of image
    QLineEdit *export_AddressOfNames;        // RVA from base of image
    QLineEdit *export_AddressOfNameOrdinals; // RVA from base of image
    QLineEdit *export_AddressOfFunctions_foa;
    QLineEdit *export_AddressOfNames_foa;
    QLineEdit *export_AddressOfNameOrdinals_foa;


    QPushButton *btnScriptDetailEditSave;
    QPushButton *btnScriptDetailSave;
    QPushButton *btnStart;
    QPushButton *btnConsoleClear;
    QPushButton *btnWindowPrint;
    QPushButton *btnScriptAdd;
    QPushButton *btnScriptAddRoot;

    QPushButton *btnRefreshWindow;
    QPushButton *btnMouseHookStart;
    QPushButton *btnMessageHookStart;
    QPushButton *btnShowStyle;
    CaptureBtn *btnFindWindow;


    QCheckBox *ckScriptStart;
    QCheckBox *ckConsoleEnable;
    QCheckBox *ckScriptDetailSleep;

    QPlainTextEdit *edtMsg;
    QPlainTextEdit *edtMessage;

    QStandardItemModel *processGridModel;
    QTableView *processTableView;
    QStandardItemModel *moduleGridModel;
    QTableView *moduleTableView;
    QStandardItemModel *exportGridModel;
    QTableView *exportTableView;

    class QStandardItemModel *imWindows;
    class QTreeView *tvWindows;

    QStandardItemModel *import2GridModel;
    QTableView *import2TableView;
    QStandardItemModel *relocationGridModel;
    QTableView *relocationTableView;

    QStandardItemModel *fileScriptGridModel;
    QTreeView *fileScriptTableView;
    QStandardItemModel *scriptGridModel;
    QTreeView *scriptTableView;
    QStandardItemModel *scriptDetailGridModel;
    QTreeView *scriptDetailTableView;

    QTabWidget *tabScriptWidget;
    QGroupBox *script2tGroupBox;
    QGroupBox *leftQWidgetGroupBox2;
    QGroupBox *script3LayoutGroupBox1;
    QGroupBox *script3LayoutGroupBox2;
    QLabel *lb_scriptConditionType;
    QLabel *lb_scriptOperateType;

    class QComboBox *ck_dpi;

    //    QGroupBox *script3GroupBox;

    QPixmap pixmap;
    QSize pixmapSize;


    class QMenu *menu_fileScript;
    class QMenu *menu_fileScriptContent;
    class QMenu *menu_script;
    class QMenu *menu_scriptContent;
    class QMenu *menu_scriptContent2;
    class QMenu *menu_scriptContent3;
    class QMenu *menu_scriptDetail;
    class QMenu *menu_scriptDetailContent;

    class QAction *action_addFileScript;
    class QAction *action_removeFileScript;
    class QAction *action_addScript;
    class QAction *action_addRootScript;
    class QAction *action_removeScript;
    class QAction *action_testScript;
    class QAction *action_renameScript;
    class QAction *action_selectScript;

    class QAction *action_addScriptDetail;
    class QAction *action_addRootScriptDetail;
    class QAction *action_removeScriptDetail;

    class QSpinBox *sb_script_sleep_success;
    class QSpinBox *sb_script_sleep_failure;
    class QSpinBox *sb_script_sleep_deal;
    class QSpinBox *sb_script_return_deal;

    class QSpinBox *sb_script_return_failure;

    class QHBoxLayout *saScriptLayout1;
    class QHBoxLayout *saScriptLayout2;
    class QHBoxLayout *saScriptLayout3;
    class QHBoxLayout *saScriptLayout4;

    class QScrollArea *saScript;
    QWidget *saScriptContentWidget;

    QModelIndex selectedIdIndex;
    QString activeFileNo;

    // -1 新建未配置 1 已配置
    int activeFileStatus;


    QString activeScriptNo;
    int activeScriptType;
    int activeScriptTypeStatus;

    // -1 新建未配置 1 已配置 2 正在运行
    int activeScriptDetailStatus;
    int activeScriptDetailType;
    QString activeScriptDetailNo;
    QString activeScriptDetailRemark;
    QStandardItem *lastItem;

signals:
};

extern Itemview10ProcessStatus *g_itemview10ProcessStatus;
#endif // Itemview10ProcessStatus_H

#ifndef ITEMVIEW10SCRIPT_H
#define ITEMVIEW10SCRIPT_H

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

#include <components/pixmapwidget.h>


class Itemview10Script : public QWidget {
    Q_OBJECT

public:

    explicit Itemview10Script(QWidget *parent = nullptr);
    ~Itemview10Script();

    bool start(int         port,
               const char *host = "0.0.0.0");

    // 读入进程
    bool buildProcess(DWORD pid);

    // 窗口更新
    bool updateWindowInfo(HWND handleWindow);

protected:

    void initUI();
    void initConnect();
    void postMessage(const QString& msg);
    void clearMessage();

    void appendMessage(const QString& msg);
    void showMessage(const QString& msg);

    bool print();

private:

    void buildScriptDetailEdit();
    void clearScriptDetailEdit();
    WIN32_PROCESS_INFO processInfo = { 0 };
    WIN32_WINDOW_INFO windowInfo = { 0 };
    PixmapWidget *pixmapWidget;

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
    QLineEdit *tb_process_handle;
    QLineEdit *tb_window_name;
    QLineEdit *tb_window_handle;
    QLineEdit *tb_window_size;
    QLineEdit *tb_window2_size;
    QLineEdit *tb_window_position;
    QLineEdit *tb_window2_position;
    QLineEdit *tb_mouse_position;
    QLineEdit *tb_mouse_ratio;
    QLineEdit *tb_mouse_ratio_position;
    QLineEdit *tb_mouse_rgb;

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

    class QRadioButton *rb_printWindow;
    class QRadioButton *rb_printClient;
    class QButtonGroup *bg_printConfig;

    class QRadioButton *rb_printWin32;
    class QRadioButton *rb_printAdb;
    class QButtonGroup *bg_printType;

    class QButtonGroup *bg_scriptType;
    class QRadioButton *rb_scriptTypeCondition;
    class QRadioButton *rb_scriptTypeDeal;
    class QRadioButton *rb_scriptTypeConditionAndDeal;


    class QButtonGroup *bg_scriptConditionType;
    class QRadioButton *rb_scriptConditionTypePixel;
    class QRadioButton *rb_scriptConditionTypeEdgeDetect;
    class QRadioButton *rb_scriptConditionTypeOCR;

    class QButtonGroup *bg_scriptOperateType;
    class QRadioButton *rb_scriptOperateTypeClick;
    class QRadioButton *rb_scriptOperateTypeMove;

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

    QPushButton *btnScriptSave;
    QPushButton *btnRefreshWindow;


    QCheckBox *ckConsoleEnable;
    QCheckBox *ckScriptDetailSleep;

    QPlainTextEdit *edtMsg;

    QStandardItemModel *processGridModel;
    QTableView *processTableView;
    QStandardItemModel *moduleGridModel;
    QTableView *moduleTableView;
    QStandardItemModel *exportGridModel;
    QTableView *exportTableView;
    QStandardItemModel *WindowsGridModel;
    QTreeView *WindowsTableView;
    QStandardItemModel *import2GridModel;
    QTableView *import2TableView;
    QStandardItemModel *relocationGridModel;
    QTableView *relocationTableView;
    QStandardItemModel *scriptGridModel;
    QTreeView *scriptTableView;

    QStandardItemModel *scriptDetailGridModel;
    QTreeView *scriptDetailTableView;

    QTabWidget *tabScriptWidget;

    //    QGroupBox *script3GroupBox;

    QSize pixmapSize;

    class QMenu *menu_script;
    class QMenu *menu_scriptContent;
    class QMenu *menu_scriptContent2;
    class QMenu *menu_scriptDetail;
    class QMenu *menu_scriptDetailContent;
    class QAction *action_addScript;
    class QAction *action_addRootScript;
    class QAction *action_removeScript;
    class QAction *action_addScriptDetail;
    class QAction *action_addRootScriptDetail;
    class QAction *action_removeScriptDetail;


    //    QModelIndex selectedIdIndex;
    QString activeScriptTypeNo;

    int activeScriptType;

    int activeScriptTypeStatus;

    // -1 新建未配置 1 已配置 2 正在运行
    int activeScriptDetailStatus;
    int activeScriptDetailType;
    QString activeScriptDetailNo;
    QString activeScriptDetailRemark;

signals:
};

#endif // ITEMVIEW10SCRIPT_H

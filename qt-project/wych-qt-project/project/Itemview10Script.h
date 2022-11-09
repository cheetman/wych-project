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

#include <components/pixmapwidget.h>


class Itemview10Script : public QWidget {
    Q_OBJECT

public:

    explicit Itemview10Script(QWidget *parent = nullptr);
    ~Itemview10Script();

    bool start(int         port,
               const char *host = "0.0.0.0");

protected:

    void initUI();
    void initConnect();
    void postMessage(const QString& msg);
    void clearMessage();

    void appendMessage(const QString& msg);
    void showMessage(const QString& msg);

private:

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

    QPushButton *btnStart;
    QPushButton *btnDebugPrivilege;
    QPushButton *btnRemoteInject;
    QPushButton *btnReflectiveInject;
    QPushButton *btnConsoleClear;
    QCheckBox *ckConsoleEnable;
    QPushButton *btnWindowPrint;

    QPlainTextEdit *edtMsg;

    QStandardItemModel *processGridModel;
    QTableView *processTableView;
    QStandardItemModel *moduleGridModel;
    QTableView *moduleTableView;
    QStandardItemModel *exportGridModel;
    QTableView *exportTableView;
    QStandardItemModel *WindowsGridModel;
    QTableView *WindowsTableView;
    QStandardItemModel *import2GridModel;
    QTableView *import2TableView;
    QStandardItemModel *relocationGridModel;
    QTableView *relocationTableView;
    QStandardItemModel *relocation2GridModel;
    QTableView *relocation2TableView;

signals:
};

#endif // ITEMVIEW10SCRIPT_H

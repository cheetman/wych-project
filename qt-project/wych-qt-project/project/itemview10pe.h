#ifndef ITEMVIEW10PE_H
#define ITEMVIEW10PE_H

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


class ItemView10PE : public QWidget {
    Q_OBJECT

public:

    explicit ItemView10PE(QWidget *parent = nullptr);
    ~ItemView10PE();

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
    QLineEdit *tb_export_size;
    QLineEdit *tb_import_size;
    QLineEdit *tb_resource_size;
    QLineEdit *tb_base_relocation_size;
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

    //    QPushButton *btnClients;
    QPushButton *btnConsoleClear;
    QCheckBox *ckConsoleEnable;

    //    QCheckBox *ckRefreshClients;


    QPlainTextEdit *edtMsg;

    //    QStandardItemModel *infoGridModel;
    //    QTableView *infoTableView;
    QStandardItemModel *tableGridModel;
    QTableView *tableTableView;
    QStandardItemModel *exportGridModel;
    QTableView *exportTableView;
    QStandardItemModel *importGridModel;
    QTableView *importTableView;
    QStandardItemModel *import2GridModel;
    QTableView *import2TableView;
    QStandardItemModel *relocationGridModel;
    QTableView *relocationTableView;
    QStandardItemModel *relocation2GridModel;
    QTableView *relocation2TableView;

    LPVOID pFileBuffer = NULL;
    PIMAGE_BASE_RELOCATION pRelocationTableBase = NULL;
    PIMAGE_IMPORT_DESCRIPTOR pImportDescriptorBase = NULL;
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    PIMAGE_NT_HEADERS32 pNTHeader32 = NULL;
    PIMAGE_NT_HEADERS64 pNTHeader64 = NULL;

    bool RVA_TO_FOA(PIMAGE_NT_HEADERS32   pNTHeader32,
                    PIMAGE_NT_HEADERS64   pNTHeader64,
                    PIMAGE_SECTION_HEADER pSectionHeader,
                    IN DWORD              RVA,
                    OUT PDWORD            FOA);

signals:
};

#endif // ITEMVIEW10PE_H

#ifndef PE_H
#define PE_H
#include "windows.h"

#include <QString>

class PE  {
public:

    PE(const wchar_t *path,
       void          *parent);
    ~PE();

    PIMAGE_DOS_HEADER m_lpDosHeader = NULL;

    // Nt头(包含文件标识、文件头、可选文件头)
    // 地址 = e_lfanew(foa)
    PIMAGE_NT_HEADERS32 m_lpNtHeader32 = NULL;
    PIMAGE_NT_HEADERS64 m_lpNtHeader64 = NULL;

    // 节表
    // 地址 = NT头地址(foa) + NT头大小(PE文件标识(4字节) + 文件头(20字节) + 可选头(SizeOfOptionalHeader字节))
    PIMAGE_SECTION_HEADER m_lpSecHeader = NULL;

    // 数据目录
    // 地址 = OptionalHeader.DataDirectory
    PIMAGE_DATA_DIRECTORY m_lpExportTable = NULL;
    PIMAGE_DATA_DIRECTORY m_lpImportTable = NULL;
    PIMAGE_DATA_DIRECTORY m_lpResourceTable = NULL;
    PIMAGE_DATA_DIRECTORY m_lpRelocationTable = NULL;
    DWORD m_foaExportTable = NULL; // VirtualAddress(rva)  -> foa
    DWORD m_foaImportTable = NULL;
    DWORD m_foaResourceTable = NULL;
    DWORD m_foaRelocationTable = NULL;

    // 导出表
    // 地址 = 数据目录[0:IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress -> foa
    PIMAGE_EXPORT_DIRECTORY m_lpExportDirectory = NULL;
    DWORD m_foaAddressOfFunctions = NULL;
    DWORD m_foaAddressOfNames = NULL;
    DWORD m_foaAddressOfNameOrdinals = NULL;

    // 导入表(第一个表的地址,使用需要while)
    // 地址 = 数据目录[1:IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress -> foa
    PIMAGE_IMPORT_DESCRIPTOR m_lpImportDescriptor = NULL;

    // 重定位表(第一个表的地址,使用需要while)
    // 地址 = 数据目录[5:IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress -> foa
    PIMAGE_BASE_RELOCATION m_lpRelocationBase = NULL;


    PIMAGE_BASE_RELOCATION m_lpBaseReloc = NULL;
    PIMAGE_IMPORT_DESCRIPTOR m_lpImport = NULL;
    PIMAGE_THUNK_DATA m_lpThunkData = NULL;
    PIMAGE_IMPORT_BY_NAME m_ImprotName = NULL;
    LPVOID m_lpFileData = NULL;
    size_t m_fileSize = 0;

    void *parent = NULL;

    void createFile();
    bool build();
    bool isX86();

    bool isX64();
    bool rvaToFoa(IN DWORD   RVA,
                  OUT PDWORD FOA);

private:

    void warning(const QString& msg);
};

#endif // PE_H

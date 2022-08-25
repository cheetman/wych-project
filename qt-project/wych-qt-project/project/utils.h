#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <Windows.h>


#define DEREF(name) *(UINT_PTR *)(name)
#define DEREF_64(name) *(DWORD64 *)(name)
#define DEREF_32(name) *(DWORD *)(name)
#define DEREF_16(name) *(WORD *)(name)
#define DEREF_8(name) *(BYTE *)(name)

class Utils {
public:

    Utils();
    static QString FromSpecialEncoding(const QString& InputStr);
    static QString ToSpecialEncoding(const QString& InputStr);
    static QString SizeToString(const quint64 val);
    static size_t  ReadFile(IN LPCTSTR  file_in,
                            OUT LPVOID *pFileBuffer);

    static bool    RVA_TO_FOA(PIMAGE_NT_HEADERS32   pNTHeader,
                              PIMAGE_SECTION_HEADER pSectionHeader,
                              IN DWORD              RVA,
                              OUT PDWORD            FOA);
    static bool RVA_TO_FOA_64(PIMAGE_NT_HEADERS64   pNTHeader,
                              PIMAGE_SECTION_HEADER pSectionHeader,
                              IN DWORD              RVA,
                              OUT PDWORD            FOA);
    static  bool FOA_TO_RVA(
        PIMAGE_NT_HEADERS32   pNTHeader,
        PIMAGE_SECTION_HEADER pSectionHeader,
        IN DWORD              FOA,
        OUT PDWORD            RVA);
    static  bool FOA_TO_RVA_64(
        PIMAGE_NT_HEADERS64   pNTHeader,
        PIMAGE_SECTION_HEADER pSectionHeader,
        IN DWORD              FOA,
        OUT PDWORD            RVA);

    static HANDLE WINAPI LoadRemoteLibraryR(HANDLE hProcess,
                                            LPVOID lpBuffer,
                                            DWORD  dwLength,
                                            LPVOID lpParameter);

    static DWORD Rva2Offset(DWORD    dwRva,
                            UINT_PTR uiBaseAddress);

    static DWORD GetReflectiveLoaderOffset(VOID *lpReflectiveDllBuffer);
};

#endif // UTILS_H

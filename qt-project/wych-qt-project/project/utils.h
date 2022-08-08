#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <Windows.h>


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
};

#endif // UTILS_H

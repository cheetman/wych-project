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
    static size_t  ReadFile(IN LPCSTR   file_in,
                            OUT LPVOID *pFileBuffer);
};

#endif // UTILS_H

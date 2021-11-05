#ifndef UTILS_H
#define UTILS_H

#include <QString>



class Utils
{
public:
    Utils();
    static QString FromSpecialEncoding(const QString &InputStr);
    static QString ToSpecialEncoding(const QString &InputStr);
    static QString SizeToString(const quint64 val);
};

#endif // UTILS_H

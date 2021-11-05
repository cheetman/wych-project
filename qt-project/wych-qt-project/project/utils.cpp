#include "utils.h"

#include <QString>

Utils::Utils()
{

}

QString Utils::SizeToString(const quint64 val)
{
    if(val == NULL){
        return "";
    }
    if(val > 1024 * 1024 * 1024){
        return QString::number(val/(1024.0f * 1024.0f * 1024.0f),'f',1) + "GB";
    }
    else if(val > 1024 * 1024){
        return QString::number(val/(1024.0f * 1024.0f),'f',1) + "MB";
    }
    else if(val > 1024){
        return QString::number(val/(1024.0f),'f',1) + "KB";
    }
    else {
        return QString::number(val) + "B";
    }

}

QString Utils::FromSpecialEncoding(const QString &InputStr)
{
#ifdef Q_OS_WIN
    return  QString::fromLocal8Bit(InputStr.toLatin1());
#else
    QTextCodec *codec = QTextCodec::codecForName("gbk");
    if (codec)
    {
        return codec->toUnicode(InputStr.toLatin1());
    }
    else
    {
        return QString("");
    }
#endif
}

QString Utils::ToSpecialEncoding(const QString &InputStr)
{
#ifdef Q_OS_WIN
    return QString::fromLatin1(InputStr.toLocal8Bit());
#else
    QTextCodec *codec= QTextCodec::codecForName("gbk");
    if (codec)
    {
        return QString::fromLatin1(codec->fromUnicode(InputStr));
    }
    else
    {
        return QString("");
    }
#endif
}

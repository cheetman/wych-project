#include "utils.h"

#include <QString>
#include <Windows.h>
#include <tchar.h>

Utils::Utils()
{}

QString Utils::SizeToString(const quint64 val)
{
    if (val == NULL) {
        return "";
    }

    if (val > 1024 * 1024 * 1024) {
        return QString::number(val / (1024.0f * 1024.0f * 1024.0f), 'f', 1) + "GB";
    }
    else if (val > 1024 * 1024) {
        return QString::number(val / (1024.0f * 1024.0f), 'f', 1) + "MB";
    }
    else if (val > 1024) {
        return QString::number(val / (1024.0f), 'f', 1) + "KB";
    }
    else {
        return QString::number(val) + "B";
    }
}

QString Utils::FromSpecialEncoding(const QString& InputStr)
{
#ifdef Q_OS_WIN
    return QString::fromLocal8Bit(InputStr.toLatin1());

#else // ifdef Q_OS_WIN
    QTextCodec *codec = QTextCodec::codecForName("gbk");

    if (codec)
    {
        return codec->toUnicode(InputStr.toLatin1());
    }
    else
    {
        return QString("");
    }
#endif // ifdef Q_OS_WIN
}

QString Utils::ToSpecialEncoding(const QString& InputStr)
{
#ifdef Q_OS_WIN
    return QString::fromLatin1(InputStr.toLocal8Bit());

#else // ifdef Q_OS_WIN
    QTextCodec *codec = QTextCodec::codecForName("gbk");

    if (codec)
    {
        return QString::fromLatin1(codec->fromUnicode(InputStr));
    }
    else
    {
        return QString("");
    }
#endif // ifdef Q_OS_WIN
}

size_t Utils::ReadFile(IN LPCTSTR file_in, OUT LPVOID *pFileBuffer) {
    FILE *fp;

    fp = _tfopen(file_in, TEXT("rb"));

    if (fp == NULL) {
        MessageBox(NULL, TEXT("fp == null"), NULL, NULL);
        return 0;
    }
    LPVOID ptempFileBuffer;
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    ptempFileBuffer = malloc(file_size);
    fseek(fp, 0, SEEK_SET); // 将指针指回文件头
    fread(ptempFileBuffer, file_size, 1, fp);

    if (ptempFileBuffer == NULL) {
        MessageBox(NULL, TEXT("ptempfilebuffer == null"), NULL, NULL);
        return 0;
    }
    *pFileBuffer = ptempFileBuffer; // 赋值，完成工作
    fclose(fp);                     // 收尾
    return file_size;
}

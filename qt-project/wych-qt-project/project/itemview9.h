#ifndef ITEMVIEW9_H
#define ITEMVIEW9_H


#include <QDir>
#include <QLabel>
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>
#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QString>
#include <QTableWidget>
#include <QLineEdit>

#include "winsock2.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <winternl.h>

#include <uv.h>


typedef struct _THREAD_BASIC_INFORMATION
{
    NTSTATUS ExitStatus;
    PVOID TebBaseAddress;
    CLIENT_ID ClientId;
    KAFFINITY AffinityMask;
    KPRIORITY Priority;
    KPRIORITY BasePriority;
} THREAD_BASIC_INFORMATION;





class ItemView9 : public QWidget
{
    Q_OBJECT
public:
    explicit ItemView9(QWidget *parent = nullptr);

public slots:
//    void openPlayer();
//    void btnUpDir();
//    void btnPlayer();
//    void btnRename();
//    void fileTableRowDoubleClicked(const QModelIndex &);
//    void leftTabFavorTableRowDoubleClicked(const QModelIndex &);
//    void getFlagFromDialog();

//    void printList();
//    void printError(QString errorMsg);
//    void replySkipRead();
//      __stdcall unsigned testThread(void * pParam);

private:
//    QString host;
//    int port;
//    QString username;
//    QString password;
//    QString path;
//    QStandardItemModel *fileGridModel;
//    QStandardItemModel *leftTabFavorTableModel;
//    QTableView *fileTableView;
//    QLabel *labelWorkDir;
//    QLabel *labelFtpStatus;
//    QDir *dirFtpPath;
//    QString strCdPath;
//    QString strNowPath;
//    int ftpStatus;
//    bool existDialog;

//      __stdcall unsigned testThread2(void * pParam);
    HANDLE hThread;



signals:
//    void sendVideoPlayer(QString url);

};

#endif // ITEMVIEW9_H

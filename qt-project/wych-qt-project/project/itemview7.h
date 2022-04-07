#ifndef ITEMVIEW7_H
#define ITEMVIEW7_H

#include "itemview7vlcplayer.h"
#include "qftp.h"

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


class ItemView7 : public QWidget
{
    Q_OBJECT
public:
    explicit ItemView7(QWidget *parent = nullptr);

public slots:
    void openPlayer();
    void btnUpDir();
    void btnPlayer();
    void ftpCommandStarted(int);
    void ftpCommandFinished(int,bool);
    void ftpStateChanged(int);
    void ftpListInfo(const QUrlInfo &);
    void fileTableRowDoubleClicked(const QModelIndex &);
    void leftTabFavorTableRowDoubleClicked(const QModelIndex &);
    void getFlagFromDialog();


private:
    QFtp *ftp;
    QString ip;
    int port;
    QString account;
    QString password;
    QString path;
    QStandardItemModel *fileGridModel;
    QStandardItemModel *leftTabFavorTableModel;
    QTableView *fileTableView;
    QLabel *labelWorkDir;
    QLabel *labelFtpStatus;
    QDir *dirFtpPath;
    QString strCdPath;
    QString strNowPath;
    int ftpStatus;
    QVector<QUrlInfo> vectorListInfos;
    bool existDialog;

signals:
    void sendVideoPlayer(QString url);

};

#endif // ITEMVIEW7_H

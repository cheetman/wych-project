#ifndef ITEMVIEW8_H
#define ITEMVIEW8_H

#include "itemview7vlcplayer.h"
#include "components/selectdialog.h"

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
#include <qwebdav.h>
#include <qwebdavdirparser.h>
#include <QTableWidget>
#include <QScrollArea>
#include <QInputDialog>
#include <QTextCodec>
#include <QComboBox>
#include <QSplitter>
#include <QDesktopServices>
#include <QMap>


class ItemView8 : public QWidget {
    Q_OBJECT

public:

    explicit ItemView8(QWidget *parent = nullptr);

public slots:

    void openPlayer();
    void btnUpDir();
    void btnPlayer();
    void btnRename();

    void fileTableRowClicked(const QModelIndex&);
    void fileTableRowDoubleClicked(const QModelIndex&);
    void leftTabFavorTableRowDoubleClicked(const QModelIndex&);
    void getFlagFromDialog();

    void printList();
    void printError(QString errorMsg);
    void replySkipRead();

    //    void addOperateButton(QTableWidget *table, int row);
    //    void slot_remove_row();
    //    void slot_edit_row();

private:

    QString host;
    int port;
    QString username;
    QString password;
    QString path;
    QString tmpPath;
    QStandardItemModel *fileGridModel;
    QStandardItemModel *leftTabFavorTableModel;
    QTableView *fileTableView;
    QLabel *labelWorkDir;
    QLabel *labelFtpStatus;
    QDir *dirFtpPath;
    QString strCdPath;
    QString strNowPath;
    int ftpStatus;
    bool existDialog = false;

    //    ItemView7VlcPlayer *playerDialog;


    QWebdav w;
    QWebdavDirParser p;
    QString m_path;
    QString pathParent;
    QList<QNetworkReply *>m_replyList;

    QPixmap *pixmap;
    QLabel *imgLabel;
    QLabel *imgLabel2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;


    QLabel *labelVideoCode;
    QLabel *labelVideoName;
    QLabel *labelVideoPubDate;
    QLabel *labelVideoLevel;
    QLabel *labelVideoDetail;
    QLabel *labelVideoQuality;
    QLabel *labelVideoIsU;
    QLabel *labelVideoPublisher;
    QLabel *labelVideoActor;
    QLabel *labelVideoSize;
    QLabel *labelVideoDirector;


    QComboBox *comboAvPage;
    QComboBox *comboDownloadPage;
    SelectDialog *selectDialog = NULL;

    bool openSelectFlag = false;

    QMap<QString, int>CacheRowIndexs;

protected:

    void resizeEvent(QResizeEvent *event);

signals:

    void sendVideoPlayer(QString url);
};

#endif // ITEMVIEW8_H

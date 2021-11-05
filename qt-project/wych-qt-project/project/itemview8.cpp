
#include "itemview8.h"
#include "itemview7player.h"
#include "itemview7vlcplayer.h"
#include "qftp.h"
#include "utils.h"

#include <QTextCodec>



ItemView8::ItemView8(QWidget *parent) : QWidget(parent)
{


        fileTableView = new QTableView(this);
        fileGridModel = new QStandardItemModel();
        /* 设置表格标题行(输入数据为QStringList类型) */
        fileGridModel->setHorizontalHeaderLabels({  "操作","文件名", "类型", "大小"});
        fileTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        fileTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        fileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        /* 自适应所有列，让它布满空间 */
//        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        /* 设置表格视图数据 */
        fileTableView->setModel(fileGridModel);


        /* 左侧浏览区域 */
        auto leftGroupBox = new QGroupBox(tr("浏览"),this);
        auto leftLayout = new QHBoxLayout(this);
        auto leftTabWidget = new QTabWidget(this);
        leftGroupBox->setLayout(leftLayout);
        leftLayout->addWidget(leftTabWidget);
        auto leftTabWidget1 = new QWidget(this);
        auto leftTabLayout1 = new QVBoxLayout(this);
        leftTabWidget1->setLayout(leftTabLayout1);
        auto leftTabFavorTableView = new QTableView(this);
        leftTabFavorTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        leftTabFavorTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        leftTabFavorTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        leftTabFavorTableModel = new QStandardItemModel(this);
        leftTabFavorTableModel->setHorizontalHeaderLabels({  "名称","Host","端口", "路径", "用户名","密码"});


        QFile file("D:\\ftpFavorDirList.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString value = file.readAll();
        file.close();
        QJsonParseError parseJsonErr;
        QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
        if (! (parseJsonErr.error == QJsonParseError::NoError)) {
            QMessageBox::about(NULL, "提示", "配置文件错误！");
            return;
        }

        QJsonArray jsonArray = document.array();
        for (int i = 0; i < jsonArray.size(); i++) {
               QJsonObject jsonObj = jsonArray.at(i).toObject();
               leftTabFavorTableModel->setItem(i, 0, new QStandardItem(jsonObj["name"].toString()));
               leftTabFavorTableModel->setItem(i, 1, new QStandardItem(jsonObj["host"].toString()));
               leftTabFavorTableModel->setItem(i, 2, new QStandardItem(jsonObj["port"].toString()));
               leftTabFavorTableModel->setItem(i, 3, new QStandardItem(jsonObj["dir"].toString()));
               leftTabFavorTableModel->setItem(i, 4, new QStandardItem(jsonObj["username"].toString()));
               leftTabFavorTableModel->setItem(i, 5, new QStandardItem(jsonObj["password"].toString()));
        }

        leftTabFavorTableView->setModel(leftTabFavorTableModel);
        labelFtpStatus = new QLabel(this);
        labelFtpStatus->setText(tr("FTP状态：无"));
        leftTabLayout1->addWidget(labelFtpStatus);
        leftTabLayout1->addWidget(leftTabFavorTableView);
        auto leftTabWidget2 = new QWidget(this);
        auto leftTabLayout2 = new QVBoxLayout(this);
        leftTabWidget2->setLayout(leftTabLayout2);
        leftTabWidget->addTab(leftTabWidget1, tr("WebDav收藏夹"));
        leftTabWidget->addTab(leftTabWidget2, tr("信息"));

        /* 文件操作按钮区域 */

        auto fileBtnGroupBox = new QGroupBox(tr("操作"),this);
        auto layout1_2 = new QHBoxLayout(this);
        auto btnUpDir = new QPushButton(tr("向上"),fileBtnGroupBox);
        auto btnPlay = new QPushButton(tr("播放"),fileBtnGroupBox);
        labelWorkDir = new QLabel(fileBtnGroupBox);
        labelWorkDir->setText("无");
        btnUpDir->setFixedWidth(80);
        btnPlay->setFixedWidth(80);
        layout1_2->setAlignment(Qt::AlignLeft);

        layout1_2->addWidget(new QLabel("当前目录：",fileBtnGroupBox));
        layout1_2->addWidget(labelWorkDir);
        layout1_2->addWidget(btnUpDir);
        layout1_2->addWidget(btnPlay);
        fileBtnGroupBox->setLayout(layout1_2);




        auto fileGroupBox = new QGroupBox(tr("列表"),this);
        auto fileGroupBoxLayout = new QVBoxLayout(this);
        fileGroupBoxLayout->addWidget(fileTableView);
        fileGroupBox->setLayout(fileGroupBoxLayout);

        auto mainLayout = new QGridLayout(this);
//        mainLayout->addWidget(btnOpenPlayer, 0,0);
        mainLayout->addWidget(leftGroupBox, 1,0,2,1);
        mainLayout->addWidget(fileBtnGroupBox,1,1);
        mainLayout->addWidget(fileGroupBox, 2,1);
        mainLayout->setColumnMinimumWidth(0,200);







        setLayout(mainLayout);
//        connect(btnOpenPlayer, SIGNAL(clicked()), this, SLOT(openPlayer()));
        connect(btnUpDir, SIGNAL(clicked()), this, SLOT(btnUpDir()));
        connect(btnPlay, SIGNAL(clicked()), this, SLOT(btnPlayer()));

        ftpStatus = -1;
        connect(fileTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(fileTableRowDoubleClicked(const QModelIndex &)));
        connect(leftTabFavorTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(leftTabFavorTableRowDoubleClicked(const QModelIndex &)));


        w.setConnectionSettings(QWebdav::HTTP, "127.0.0.1", "/", "USERNAME", "PASSWORD", 80);
        connect(&p, SIGNAL(finished()), this, SLOT(printList()));
        connect(&p, SIGNAL(errorChanged(QString)), this, SLOT(printError(QString)));
        connect(&w, SIGNAL(errorChanged(QString)), this, SLOT(printError(QString)));





}

void ItemView8::fileTableRowDoubleClicked(const QModelIndex &current)
{

      auto filename =  fileGridModel->item(current.row(),1)->text();
      auto path =  fileGridModel->item(current.row(),5)->text();
      qDebug() << filename << "fsfd" << path;
      fileGridModel->clear();
      fileTableView->setModel(fileGridModel);
      p.listDirectory(&w, path);
}

void ItemView8::leftTabFavorTableRowDoubleClicked(const QModelIndex &current)
{

      auto host =  leftTabFavorTableModel->item(current.row(),1)->text();
      auto port =  leftTabFavorTableModel->item(current.row(),2)->text().toUInt();
      auto username =  leftTabFavorTableModel->item(current.row(),4)->text();
      auto password =  leftTabFavorTableModel->item(current.row(),5)->text();



    fileGridModel->clear();
    fileGridModel->setHorizontalHeaderLabels({ "操作","文件名", "类型", "大小","日期","路径"});
    fileTableView->setModel(fileGridModel);
    labelFtpStatus->setText(tr("FTP状态：正在登陆..."));

    m_path = "/";
    w.setConnectionSettings(QWebdav::HTTP, host, m_path, username, password, port);
    p.listDirectory(&w, m_path);

}

void ItemView8::btnUpDir()
{
         qDebug()<<"目录向上" ;

         auto list = labelWorkDir->text().split("/",Qt::SkipEmptyParts);
         QString path = "/";
         for (short i = 0;i< list.size() - 1; i++) {
             path += list[i] +="/";
         }

         fileGridModel->clear();
         fileGridModel->setHorizontalHeaderLabels({ "操作","文件名", "类型", "大小","日期","路径"});
         fileTableView->setModel(fileGridModel);
         p.listDirectory(&w,path);
}

void ItemView8::btnPlayer()
{
    if(!existDialog){
        existDialog = true;
        auto playerDialog = new ItemView7VlcPlayer(this);
        connect(this,&ItemView8::sendVideoPlayer,playerDialog,&ItemView7VlcPlayer::getVideoData);
        connect(playerDialog,&ItemView7VlcPlayer::closeDialog ,this,&ItemView8::getFlagFromDialog);
        playerDialog->setModal(false);
        playerDialog->show();
    }else{
        emit sendVideoPlayer();
    }
}
void ItemView8::getFlagFromDialog()
{
       existDialog = false;
}

void ItemView8::openPlayer()
{

    const QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
       const QString filePath =
           QFileDialog::getOpenFileName(this, tr("Open File"),
                                        musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first(),
                                        tr("MP3 files (*.mp3);;All files (*.*)"));
       if (!filePath.isEmpty()){
           auto playerView = new ItemView7Player(this);
           playerView->setModal(false);
           playerView->show();
       }

}

void ItemView8::printList()
{

    qDebug() << "QWebdav::printList() ";
    QList<QWebdavItem> list = p.getList();
    labelWorkDir->setText(p.path());
    fileGridModel->setHorizontalHeaderLabels({ "操作","文件名", "类型", "大小","日期","路径"});

    int index = 0;
    foreach(QWebdavItem item, list) {
        qDebug() << item.name();
//        QNetworkReply *reply = w.get(item.path());
//        connect(reply, SIGNAL(readyRead()), this, SLOT(replySkipRead()));
//        m_replyList.append(reply);


        fileGridModel->setItem(index, 1, new QStandardItem(item.name()));
        fileGridModel->setItem(index, 2, new QStandardItem(item.isDir()?tr("目录"):tr("文件") ));
        fileGridModel->setItem(index, 3, new QStandardItem(Utils::SizeToString(item.size())));
        fileGridModel->setItem(index, 4, new QStandardItem(item.lastModifiedStr()));
        fileGridModel->setItem(index++, 5, new QStandardItem(item.path()));
    }

    fileTableView->setModel(fileGridModel);


}

void ItemView8::printError(QString errorMsg)
{
    if(errorMsg == "Connection refused"){
        labelFtpStatus->setText(tr("FTP状态：连接被拒绝"));
    }
    qDebug() << "QWebdav::printErrors()  errorMsg == " << errorMsg;
}

void ItemView8::replySkipRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if (reply==0)
        return;

    QByteArray ba = reply->readAll();

    qDebug() << "QWebdav::replySkipRead()   skipped " << ba.size() << " reply->url() == " << reply->url().toString(QUrl::RemoveUserInfo);
}





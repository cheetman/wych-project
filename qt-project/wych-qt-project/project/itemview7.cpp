#if _MSC_VER >=1600    // MSVC2015>1899,对于MSVC2010以上版本都可以使用
#pragma execution_character_set("utf-8")
#endif

#include "itemview7.h"
#include "itemview7player.h"
#include "itemview7vlcplayer.h"
#include "qftp.h"
#include "utils.h"

#include <QTextCodec>



ItemView7::ItemView7(QWidget *parent) : QWidget(parent)
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
        leftTabWidget->addTab(leftTabWidget1, tr("Ftp收藏夹"));
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

        ftp = new QFtp(this);
        ftpStatus = -1;
        connect(ftp,SIGNAL(commandStarted(int)),this,SLOT(ftpCommandStarted(int)));
        connect(ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(ftpCommandFinished(int,bool)));
        connect(ftp,SIGNAL(stateChanged(int)),this,SLOT(ftpStateChanged(int)));
        connect(ftp,SIGNAL(listInfo(const QUrlInfo &)),this,SLOT(ftpListInfo(const QUrlInfo &)));
        connect(fileTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(fileTableRowDoubleClicked(const QModelIndex &)));
        connect(leftTabFavorTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(leftTabFavorTableRowDoubleClicked(const QModelIndex &)));







}

void ItemView7::fileTableRowDoubleClicked(const QModelIndex &current)
{
          auto filename =  fileGridModel->item(current.row(),1)->text();
          qDebug() << filename;
          strCdPath = filename;
          ftp->cd(Utils::ToSpecialEncoding(filename));
          vectorListInfos.clear();
          fileGridModel->clear();
          fileTableView->setModel(fileGridModel);
          ftp->list();
}

void ItemView7::leftTabFavorTableRowDoubleClicked(const QModelIndex &current)
{
    switch (ftpStatus) {
        case QFtp::HostLookup:
        case QFtp::Connecting:
        case QFtp::Connected:
        case QFtp::LoggedIn:  {
        return;
        }
    }
          auto host =  leftTabFavorTableModel->item(current.row(),1)->text();
          auto port =  leftTabFavorTableModel->item(current.row(),2)->text().toUInt();
          auto username =  leftTabFavorTableModel->item(current.row(),4)->text();
          auto password =  leftTabFavorTableModel->item(current.row(),5)->text();
          ftp->connectToHost(host,port);
          ftp->login(username,password);
          vectorListInfos.clear();
          fileGridModel->clear();
          fileTableView->setModel(fileGridModel);
          ftp->list();

}

void ItemView7::btnUpDir()
{
         qDebug()<<"目录向上，当前状态：" << ftp->state();
         strCdPath = "..";
         ftp->cd("..");
         vectorListInfos.clear();
         fileGridModel->clear();
         fileTableView->setModel(fileGridModel);
         ftp->list();
}

void ItemView7::btnPlayer()
{
    if(!existDialog){
        existDialog = true;
        auto playerDialog = new ItemView7VlcPlayer(this);
        connect(this,&ItemView7::sendVideoPlayer,playerDialog,&ItemView7VlcPlayer::getVideoData);
        connect(playerDialog,&ItemView7VlcPlayer::closeDialog ,this,&ItemView7::getFlagFromDialog);
        playerDialog->setModal(false);
        playerDialog->show();
    }else{
        emit sendVideoPlayer("");
    }
}
void ItemView7::getFlagFromDialog()
{
       existDialog = false;
}

void ItemView7::openPlayer()
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

void ItemView7::ftpListInfo(const QUrlInfo &urlInfo)
{
      vectorListInfos.push_back(urlInfo);




//    QTreeWidgetItem *item = new QTreeWidgetItem;
//    item->setText(0, _FromSpecialEncoding(urlInfo.name()));
//    item->setText(1, formatSize(urlInfo.size()));
//    item->setText(3, urlInfo.lastModified().toString("yyyy-MM-dd hh:mm"));
//    QPixmap pixmap(urlInfo.isDir() ? "F：/desk/images/dir.png" : "../myFTP/file.png");
//    item->setIcon(0, pixmap);
//    isDirectory[urlInfo.name()] = urlInfo.isDir();
//    QString name = item->text(0);
//     if (isDirectory.value(name))
//     {
//         item->setText(2,"folder");
//     }
//     else item->setText(2," ");

//    ui->treeWidget->addTopLevelItem(item);
//    if (!ui->treeWidget->currentItem()) {
//        ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
//        ui->treeWidget->setEnabled(true);
//    }
}

void ItemView7::ftpCommandStarted(int)
{
    int id=ftp->currentCommand();
    switch (id) {
    case QFtp::ConnectToHost:
        labelFtpStatus->setText(tr("FTP状态：正在连接服务器..."));
        qDebug()<<"connecting to the qftp...";
        break;
    case QFtp::Login:
        labelFtpStatus->setText(tr("FTP状态：正在登陆..."));
        qDebug()<<"logining ...";
        break;
    case QFtp::Get:
        qDebug()<<"loading ...";
        break;
    case QFtp::Close:
        qDebug()<<"run to close ...";
    default:
        break;
    }

}

void ItemView7::ftpCommandFinished(int, bool error)
{

    if(ftp->currentCommand()==QFtp::ConnectToHost){
        if(error){
            qDebug()  << "ErrorString " << ftp->errorString();
            QMessageBox::information(this, "FTP", QStringLiteral("无法连接到 FTP 服务器，请检查主机名是否正确！"));
//            ftp->abort();
//            ftp->deleteLater();
//            ftp = NULL;
        }else{

        }
            qDebug()<<"FTP服务器连接成功！";
    }
    else if(ftp->currentCommand()==QFtp::Login){
        if(error){
            qDebug()  << "ErrorString " << ftp->errorString();
            QMessageBox::information(this, "FTP", QStringLiteral("无法连接到 FTP 服务器，请检查用户名密码是否正确！"));
            ftp->close();
        }else
        {
            qDebug()<<"FTP服务器登录成功！";
            strNowPath = "\\";
            labelWorkDir->setText(strNowPath);
        }
    }
    else if(ftp->currentCommand()==QFtp::Get){
        if(error)
            qDebug()<<"load has found error";
        else{
            qDebug()<<"load successful";
        }
    }
    else if(ftp->currentCommand()==QFtp::List){
        if(error){
            qDebug()  << "ErrorString " << ftp->errorString();
        }else{
            fileGridModel->setHorizontalHeaderLabels({ "操作","文件名", "类型", "大小","日期"});
             for (int i = 0;i < vectorListInfos.size() ;i++){
                 auto item = vectorListInfos[i];
                 fileGridModel->setItem(i, 1, new QStandardItem(Utils::FromSpecialEncoding(item.name())));
                 fileGridModel->setItem(i, 2, new QStandardItem(item.isDir()?tr("目录"):tr("文件") ));
                 fileGridModel->setItem(i, 3, new QStandardItem(QString::number(item.size())));
                 fileGridModel->setItem(i, 4, new QStandardItem(item.lastModified().toString("yyyy-MM-dd")));
             }
            fileTableView->setModel(fileGridModel);
            qDebug()  << "list over";
        }
    }
    else if(ftp->currentCommand()==QFtp::Close){
            qDebug()<<"closed";
    }
    else if(ftp->currentCommand()==QFtp::Cd){
        if(error){
            qDebug()  << "ErrorString " << ftp ->errorString();
        }else{

            if(strCdPath == ".."){
               auto list = strNowPath.split("\\", QString::SkipEmptyParts);
               strNowPath = "\\";
               for(int i = 0; i< list.size() - 1;++i)
               {
                   QString tmp = list.at(i);
                   strNowPath += tmp ;
                   strNowPath += "\\";
               }
                labelWorkDir->setText(strNowPath);

            }else{
                strNowPath += strCdPath;
                strNowPath += "\\";
                labelWorkDir->setText(strNowPath);
            }

        }
    }
}

void ItemView7::ftpStateChanged(int state)
{
    ftpStatus = state;
    switch (state) {
        case QFtp::Unconnected: {
            labelFtpStatus->setText(tr("FTP状态：没有连接到主机"));
            qDebug()<<"state changed: 没有连接到主机";
            break;
        }
        case QFtp::HostLookup: {
            labelFtpStatus->setText(tr("FTP状态：正在进行主机名查找"));
            qDebug()<<"state changed: 正在进行主机名查找";
            break;
        }
        case QFtp::Connecting: {
            labelFtpStatus->setText(tr("FTP状态：正在尝试连接到主机"));
            qDebug()<<"state changed: 正在尝试连接到主机";
            break;
        }
        case QFtp::Connected: {
            labelFtpStatus->setText(tr("FTP状态：已连接"));
            qDebug()<<"state changed: 已实现与主机的连接";
            break;
        }
        case QFtp::LoggedIn: {
            labelFtpStatus->setText(tr("FTP状态：已登陆"));
            qDebug()<<"state changed: 已实现连接和用户登录";
            break;
        }
        case QFtp::Closing: {
            labelFtpStatus->setText(tr("FTP状态：连接正在关闭"));
            qDebug()<<"state changed: 连接正在关闭";
            break;
        }
        default:
            break;
        }
}





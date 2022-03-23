
#include "itemview8.h"
#include "itemview7player.h"
#include "itemview7vlcplayer.h"
#include "qftp.h"
#include "utils.h"




ItemView8::ItemView8(QWidget *parent) : QWidget(parent)
{

    //  一.数据

    // 视频信息数据
    labelVideoCode = new QLabel("",this);
    labelVideoName = new QLabel("",this);
    labelVideoName->setMaximumWidth(400);
    labelVideoPubDate = new QLabel("",this);
    labelVideoLevel = new QLabel("",this);
    labelVideoDetail = new QLabel("",this);
    labelVideoQuality = new QLabel("",this);
    labelVideoIsU = new QLabel("",this);
    labelVideoPublisher = new QLabel("",this);
    labelVideoActor = new QLabel("",this);
    labelVideoSize = new QLabel("",this);
    labelVideoDirector = new QLabel("",this);

    comboAvPage = new QComboBox(this);
    comboDownloadPage = new QComboBox(this);
    auto btnOpenWiki = new QPushButton(tr("打开Wiki"),this);
    auto btnSearch = new QPushButton(tr("搜索资源"),this);
    auto leftTabWidget2InfoGridLayout = new QGridLayout(this);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("编号:",this), 0,0);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("名称:",this),1,0);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("发行日期:",this), 2,0);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("评价:",this), 3,0);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("细节:",this), 4,0);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("画质:",this), 5,0);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("去:",this), 6,0);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("主要演员:",this), 7,0);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("导演:",this), 4,2);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("发行商:",this), 3,2);
    leftTabWidget2InfoGridLayout->addWidget(new QLabel("视频大小:",this), 5,2);
    leftTabWidget2InfoGridLayout->addWidget(btnOpenWiki, 6,2);
    leftTabWidget2InfoGridLayout->addWidget(btnSearch, 7,2);

    leftTabWidget2InfoGridLayout->addWidget(labelVideoCode, 0,1);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoName, 1,1,1,3);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoPubDate, 2,1);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoLevel, 3,1);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoDetail, 4,1);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoQuality, 5,1);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoIsU, 6,1);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoActor, 7,1);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoDirector, 4,3);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoPublisher, 3,3);
    leftTabWidget2InfoGridLayout->addWidget(labelVideoSize, 5,3);

    leftTabWidget2InfoGridLayout->addWidget(comboAvPage, 6,3);
    leftTabWidget2InfoGridLayout->addWidget(comboDownloadPage, 7,3);

    comboAvPage->addItem("df");
    comboDownloadPage->addItem("df");

    // 按钮
    auto btnUpDir = new QPushButton(tr("向上"),this);
    auto btnPlay = new QPushButton(tr("播放"),this);
    auto btnRename = new QPushButton(tr("重命名"),this);
    btnRename->setShortcut(Qt::Key_F2);
    btnUpDir->setFixedWidth(80);
    btnPlay->setFixedWidth(80);


    // 目录表格
    fileTableView = new QTableView(this);
    fileGridModel = new QStandardItemModel();
    /* 设置表格标题行(输入数据为QStringList类型) */
    fileGridModel->setHorizontalHeaderLabels({  "操作","文件名", "类型", "大小"});
    fileTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    fileTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* 自适应所有列，让它布满空间 */
    //tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    /* 设置表格视图数据 */
    fileTableView->setModel(fileGridModel);

    // 文件表格
    auto leftTabFavorTableView = new QTableView(this);
    leftTabFavorTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    leftTabFavorTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftTabFavorTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    leftTabFavorTableModel = new QStandardItemModel(this);
    leftTabFavorTableModel->setHorizontalHeaderLabels({  "名称","Host","端口", "路径", "用户名","密码"});



    // Label
    labelFtpStatus = new QLabel(this);
    labelFtpStatus->setText(tr("FTP状态：无"));




    // 初始化
    // 读取配置
    QSettings settings("configs/setting.ini", QSettings::IniFormat);
    QFile file(settings.value("DefaultDir/FtpFavorDirList","D:\\ftpFavorDirList.json").toString());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString value = file.readAll();
        file.close();
        QJsonParseError parseJsonErr;
        auto document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
        if (! (parseJsonErr.error == QJsonParseError::NoError)) {
            QMessageBox::about(this, "提示", "配置文件错误！");
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
    }else{
             QMessageBox::warning(this,tr("错误"), tr("配置文件打开失败！"));
    }

    QString videoInfoPath("D:\\TesData\\AppConsole\\Data\\VideoInfo");



        auto splitterMain = new QSplitter(Qt::Horizontal,this);


        // 二、布局

        // 1.1 左侧浏览
        auto leftGroupBox = new QGroupBox(tr("浏览"),splitterMain);
        auto leftLayout = new QHBoxLayout(leftGroupBox);
        auto leftTabWidget = new QTabWidget(this);
        leftLayout->addWidget(leftTabWidget);
        auto leftTabWidget1 = new QWidget(this);
        auto leftTabLayout1 = new QVBoxLayout(leftTabWidget1);
        leftTabFavorTableView->setModel(leftTabFavorTableModel);
        leftTabLayout1->addWidget(labelFtpStatus);
        leftTabLayout1->addWidget(leftTabFavorTableView);

        // 1.2 左侧图片
        auto leftTabWidget2 = new QWidget(this);
        auto leftTabLayout2 = new QVBoxLayout(leftTabWidget2);
        imgLabel = new QLabel(this);
        pixmap = new QPixmap("C:\\Users\\汪意超\\Pictures\\Saved Pictures\\001OdAkagy1gyybblipk3j60h00fvajh02.jpg");
        scrollArea = new QScrollArea(this);
        scrollAreaWidgetContents = new QWidget(scrollArea);
        scrollAreaWidgetContents->setBackgroundRole(QPalette::Mid);
//      scrollAreaWidgetContents->setMinimumSize(QSize(400, 400));
        scrollArea->setBackgroundRole(QPalette::Dark);  // 背景色
//        scrollArea->setWidget(imgLabel);
//        scrollArea->setWidget(imgLabel2);
        QVBoxLayout *pLayout=new QVBoxLayout(scrollAreaWidgetContents);
        pLayout->setMargin(0);
        pLayout->setSpacing(0);
        pLayout->addWidget(imgLabel);
        scrollArea->setWidget(scrollAreaWidgetContents);
        leftTabLayout2->addWidget(scrollArea);
        auto leftTabWidget2Info = new QWidget(this);
        leftTabLayout2->addWidget(leftTabWidget2Info);
        leftTabWidget2Info->setLayout(leftTabWidget2InfoGridLayout);
        leftTabWidget->addTab(leftTabWidget1, tr("WebDav收藏夹"));
        leftTabWidget->addTab(leftTabWidget2, tr("信息"));



//         imgLabel2 = new QLabel(this);

//        auto img=new QImage();
//        if(! ( img->load("C:\\Users\\汪意超\\Pictures\\Saved Pictures\\001OdAkagy1gyybblipk3j60h00fvajh02.jpg") ) ) //加载图像
//             {
//                 QMessageBox::information(this,
//                                          tr("打开图像失败"),
//                                          tr("打开图像失败!"));
//                 delete img;
//                 return;
//             }

//        img->scaled(100,
//                    100,
//                    Qt::KeepAspectRatio);
//        imgLabel->setPixmap(QPixmap::fromImage(*img));
//        imgLabel->setMinimumWidth(50);
//        imgLabel->setScaledContents(true);



//        imgLabel->setScaledContents(true);
//        imgLabel->setPixmap(pixmap->scaled(imgLabel->size(),Qt::KeepAspectRatioByExpanding));
//        imgLabel->setMinimumWidth(100);
//        imgLabel->setMinimumHeight(100);
//        imgLabel->setMaximumWidth(500);
//        imgLabel->setMaximumHeight(500);

//         imgLabel->setFixedWidth(300);
//          imgLabel->setFixedHeight(300);
//        imgLabel->resizeEvent();
//        imgLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
//        ->setAlignment(Qt::AlignTop);
//        leftTabLayout2->addStretch();


//        pLayout->addWidget(imgLabel2);

//        auto *pLayout = new QGridLayout(scrollAreaWidgetContents);
//        pLayout->addWidget(imgLabel, 0, 0);
//        pLayout->addWidget(imgLabel2, 1, 0);

//            imgLabel->setPixmap(pixmap->scaled(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width(),Qt::KeepAspectRatio));
//            imgLabel->setFixedSize(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width());
//            imgLabel2->setPixmap(pixmap->scaled(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width(),Qt::KeepAspectRatio));
//            imgLabel2->setFixedSize(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width());



        // 2.1 文件按钮

        auto widgetMainRigth = new QWidget(splitterMain);
        auto mainLayoutRight = new QVBoxLayout(widgetMainRigth);
        auto fileBtnGroupBox = new QGroupBox(tr("操作"),this);
        auto layout1_2 = new QHBoxLayout(fileBtnGroupBox);
        labelWorkDir = new QLabel(fileBtnGroupBox);
        labelWorkDir->setText("无");
        layout1_2->setAlignment(Qt::AlignLeft);
        layout1_2->addWidget(new QLabel("当前目录：",fileBtnGroupBox));
        layout1_2->addWidget(labelWorkDir);
        layout1_2->addWidget(btnUpDir);
        layout1_2->addWidget(btnPlay);
        layout1_2->addWidget(btnRename);


        // 2.2 文件列表
        auto fileGroupBox = new QGroupBox(tr("列表"),this);
        auto fileGroupBoxLayout = new QVBoxLayout(fileGroupBox);
        fileGroupBoxLayout->addWidget(fileTableView);
         mainLayoutRight->addWidget(fileBtnGroupBox);
         mainLayoutRight->addWidget(fileGroupBox);
//        auto mainLayout = new QGridLayout(this);
//        mainLayout->addWidget(btnOpenPlayer, 0,0);
//        mainLayout->addWidget(leftGroupBox, 1,0,2,1);
//        mainLayout->addWidget(fileBtnGroupBox,1,1);
//        mainLayout->addWidget(fileGroupBox, 2,1);
//        mainLayout->setColumnMinimumWidth(0,200);
        auto mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(splitterMain);




        setLayout(mainLayout);
//        connect(btnOpenPlayer, SIGNAL(clicked()), this, SLOT(openPlayer()));
        connect(btnUpDir, SIGNAL(clicked()), this, SLOT(btnUpDir()));
        connect(btnPlay, SIGNAL(clicked()), this, SLOT(btnPlayer()));
        connect(btnRename, SIGNAL(clicked()), this, SLOT(btnRename()));

        connect(btnOpenWiki,&QPushButton::clicked,[=](){


        });

        ftpStatus = -1;
        connect(fileTableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(fileTableRowClicked(const QModelIndex &)));
        connect(fileTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(fileTableRowDoubleClicked(const QModelIndex &)));
        connect(leftTabFavorTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(leftTabFavorTableRowDoubleClicked(const QModelIndex &)));


//        w.setConnectionSettings(QWebdav::HTTP, "127.0.0.1", "/", "USERNAME", "PASSWORD", 80);
        connect(&p, SIGNAL(finished()), this, SLOT(printList()));
        connect(&p, SIGNAL(errorChanged(QString)), this, SLOT(printError(QString)));
        connect(&w, SIGNAL(errorChanged(QString)), this, SLOT(printError(QString)));

}


//
void ItemView8::resizeEvent(QResizeEvent *event)
{
    //    imgLabel2->setPixmap(pixmap->scaled(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width(),Qt::KeepAspectRatio));
    //    imgLabel2->setFixedSize(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width());

    imgLabel->setPixmap(pixmap->scaled(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width(),Qt::KeepAspectRatio));
    imgLabel->setFixedSize(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width());
    scrollAreaWidgetContents->setFixedWidth(scrollArea->width() - 2);
    scrollAreaWidgetContents->setFixedHeight((float)pixmap->height()/ pixmap->width() * scrollArea->width() );

    QWidget::resizeEvent(event);
}


void ItemView8::fileTableRowClicked(const QModelIndex &current)
{

    auto filename =  fileGridModel->item(current.row(),1)->text();
    auto path =  fileGridModel->item(current.row(),5)->text();

    auto code = filename;
    auto index = code.indexOf(" ");
    if(index > 0){
        code =  code.left(index);
    }
    index = code.indexOf("(");
    if(index > 0){
        code =  code.left(index);
    }

      QFile file(tr("D:\\TesData\\AppConsole\\Data\\VideoInfo\\Cover\\%1%2").arg(code).arg(".jpg"));
      if(file.exists()){
        pixmap->load(file.fileName());
        imgLabel->setPixmap(pixmap->scaled(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width(),Qt::KeepAspectRatio));
        imgLabel->setFixedSize(scrollArea->width(), (float)pixmap->height()/ pixmap->width() * scrollArea->width());
        scrollAreaWidgetContents->setFixedWidth(scrollArea->width() - 2);
        scrollAreaWidgetContents->setFixedHeight((float)pixmap->height()/ pixmap->width() * scrollArea->width() );
      }

      QFile file2(tr("D:\\TesData\\AppConsole\\Data\\VideoInfo\\Info\\%1%2").arg(code).arg(".json"));
      if(file2.exists()){
          if(file2.open(QIODevice::ReadOnly | QIODevice::Text)){
              auto value = file2.readAll();
              file2.close();
              QJsonParseError parseJsonErr;
              auto document = QJsonDocument::fromJson(value, &parseJsonErr);
              if (! (parseJsonErr.error == QJsonParseError::NoError)) {
                  QMessageBox::about(this, "提示", "配置文件错误！");
                  return;
              }
             auto jsonObj = document.object();
             labelVideoCode->setText(jsonObj["Code"].toString());
             labelVideoName ->setText(jsonObj["Name"].toString());
             labelVideoActor->setText(jsonObj["Actor"].toString());
             labelVideoDirector->setText(jsonObj["Director"].toString());
             labelVideoPubDate->setText(jsonObj["SaleDate"].toString());
             labelVideoLevel->setText(jsonObj["Evaluation"].toString());
             labelVideoPublisher ->setText(jsonObj["Producer"].toString());
        }
      }

      QFile file3(tr("D:\\TesData\\AppConsole\\Data\\VideoInfo\\FileInfo\\%1%2").arg(code).arg(".json"));
      if(file3.exists()){
          if(file3.open(QIODevice::ReadOnly | QIODevice::Text)){
              auto value = file3.readAll();
              file3.close();
              QJsonParseError parseJsonErr;
              auto document = QJsonDocument::fromJson(value, &parseJsonErr);
              if (! (parseJsonErr.error == QJsonParseError::NoError)) {
                  QMessageBox::about(this, "提示", "配置文件错误！");
                  return;
              }
             auto jsonObj = document.object();
             labelVideoSize ->setText(jsonObj["FileSize"].toString());
        }
      }

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

    host =  leftTabFavorTableModel->item(current.row(),1)->text();
    port =  leftTabFavorTableModel->item(current.row(),2)->text().toUInt();
    username =  leftTabFavorTableModel->item(current.row(),4)->text();
    password =  leftTabFavorTableModel->item(current.row(),5)->text();


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
        playerDialog->exec();
//        QString param1=playerDialog->getParam();
        qDebug()<< "关闭Dialog";
        delete playerDialog;

    }else{
       auto rowIndex = fileTableView->currentIndex().row();
       if(rowIndex >= 0){
           auto type =fileGridModel->item(rowIndex,2)->text();
           auto path =fileGridModel->item(rowIndex,5)->text();
           if(type == "目录"){

                p.listDirectory(&w, path);
                openSelectFlag = true;
                tmpPath = path;
           }else{
               auto url = QString("http://%1:%2@%3:%4%5").arg(username).arg(password).arg(host).arg(port).arg(path);
    //           auto newurl = url.toLocal8Bit().toPercentEncoding("/:");
               url = QUrl::toPercentEncoding(url,"/:@");
    //           qDebug() << "url1   " << url ;
    //           auto url2 = QUrl(url).url(QUrl::FullyEncoded);
    //           qDebug() << "url2   " << url2 ;
               emit sendVideoPlayer(url);
           }
       }
    }
}


void ItemView8::btnRename()
{

    auto rowIndex = fileTableView->currentIndex().row();
    if(rowIndex >= 0){
        auto path =fileGridModel->item(rowIndex,5)->text();
        auto filename =fileGridModel->item(rowIndex,1)->text();
        auto workname =labelWorkDir->text();
        bool ok;
       QString text = QInputDialog::getText(this, tr("重命名"),tr("请输入新的名称"), QLineEdit::Normal,filename, &ok);
       if (ok && !text.isEmpty())
       {

           auto response =  w.move( path,  workname + text ,false);

             if( response->error() == QNetworkReply::NoError) {
                 fileGridModel->setItem(rowIndex,1,new QStandardItem(text));
                 fileGridModel->setItem(rowIndex,5,new QStandardItem(workname + text));
                 //fileGridModel->removeRow(1);
             }else{
                 QMessageBox::warning(this,tr("错误"), tr("重命名失败[%1]").arg(response->error()) );
             }
       }
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
    if(openSelectFlag){
        openSelectFlag = false;

        QList<QWebdavItem> list = p.getList();

        QStringList strList;
        foreach(QWebdavItem item, list) {
            if(!item.isDir()){
                strList.append(item.name());
            }
        }

        if (selectDialog==NULL) //如果对象没有被创建过，就创建对象
               selectDialog = new SelectDialog(this);

        selectDialog->setHeaderList(strList);
        int ret=selectDialog->exec();// 以模态方式显示对话框
         if (ret==QDialog::Accepted) //OK键被按下
         {
            qDebug() << "QDialog::Accepted";
           auto url = QString("http://%1:%2@%3:%4%5%6").arg(username).arg(password).arg(host).arg(port).arg(tmpPath).arg(selectDialog->getResult());
           qDebug() << url;
           url = QUrl::toPercentEncoding(url,"/:@");
           emit sendVideoPlayer(url);

         }else{
             qDebug() << "QDialog::Rejected";

         }

    }else{



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
    fileTableView->setColumnHidden(5, true);


    }

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

struct MyProperty
{
    QTableWidget *table;
    int row;
};
Q_DECLARE_METATYPE(MyProperty)  //注意声明

//void ItemView8::addOperateButton(QTableWidget *table, int row)
//{
//    QPushButton* delBtn = new QPushButton;
//     QPushButton* edtBtn = new QPushButton;
//    QHBoxLayout *h_box_layout = new QHBoxLayout(); //添加水平布局控件
//    MyProperty myProperty; //自定义类，后面有解释
//    memset(&myProperty, 0, sizeof(MyProperty));
//    myProperty.row = row;
//    myProperty.table = table;
//    QVariant v;
//    v.setValue(myProperty);
//    QWidget *widget = new QWidget(); //添加部件

//    delBtn->setIcon(QIcon(":/res/button/delete.png"));
//    delBtn->setToolTip("删除");
//    delBtn->setProperty("sender", v);
//    QObject::connect(delBtn, SIGNAL(clicked()), this, SLOT(slot_remove_row())); //设置删除动作槽函数

//    edtBtn->setIcon(QIcon(":/res/button/edit.png"));
//    edtBtn->setToolTip("修改");
//    edtBtn->setProperty("sender", v);
//    QObject::connect(edtBtn, SIGNAL(clicked()), this, SLOT(slot_edit_row())); //设置编辑动作槽函数

//    h_box_layout->addWidget(edtBtn);
//    h_box_layout->addWidget(delBtn);
//    h_box_layout->setContentsMargins(0, 0, 0, 0);
//    h_box_layout->setSpacing(1);
//    widget->setLayout(h_box_layout);
//    table->setCellWidget(row, 0, widget);
//}

//void ItemView8::slot_remove_row()
//{


//}
//void ItemView8::slot_edit_row()
//{


//}

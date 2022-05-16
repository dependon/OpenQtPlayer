#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTime>
#include <QTimer>
#include <QListWidgetItem>
#include <QSettings>
#include <QFileInfo>
#include <QShortcut>
#include <QDesktopWidget>
#include <QContextMenuEvent>
#include <QGraphicsView>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    saveAllSetting();
    delete ui;
}

void MainWindow::acceptargv(int &argc, char **argv)
{
    if(0!=argc)
    {
        QString path;
        for(int i=0; i<argc; i++)//参数个数
        {
            path=argv[i];
        }
        openFileandPlay(path);
    }
}

void MainWindow::openFileonly(const QString &filename)
{
    QString filePath=filename;
    if(filePath.contains("file://"))
    {
        filePath.replace("file://","");
    }
    QFileInfo info(filePath);
    QString suffix="*."+info.suffix();
    filePath=info.canonicalFilePath();
    if(!filePath.isEmpty() && info.isFile()){
        if(!isExistencelocallist(filePath))
        {
            qDebug()<<"是否为文件" <<info.isFile();
            if(ApisupportSuffix().contains(suffix,Qt::CaseSensitive))
            {
                m_playlist->addMedia(QUrl::fromLocalFile(filePath));
                m_localPaths <<filePath;
                QListWidgetItem *item=new QListWidgetItem(ui->locallistWidget);
                item->setText(filePath);
            }

        }
    }
}

void MainWindow::openFileandPlay(const QString &filename)
{
    QString filePath=filename;
    if(filePath.contains("file://"))
    {
        filePath.replace("file://","");
    }
    QFileInfo info(filePath);
    QString suffix="*."+info.suffix();

    qDebug()<<"33" <<info.canonicalFilePath();

    filePath=info.canonicalFilePath();
    if(!filePath.isEmpty() && info.isFile()){
        if(filePath.contains("file://"))
        {
            filePath.replace("file://","");
        }
        if(!isExistencelocallist(filePath))
        {
            if(ApisupportSuffix().contains(suffix,Qt::CaseSensitive))
            {
                qDebug() <<info.path();
                qDebug() <<info.filePath();
                qDebug() << info.isFile();
                qDebug() <<QCoreApplication::applicationDirPath();
                m_playlist->addMedia(QUrl::fromLocalFile(filePath));
                m_localPaths <<filePath;
                QListWidgetItem *item=new QListWidgetItem(ui->locallistWidget);
                item->setText(filePath);
                ui->locallistWidget->setCurrentItem(item);
                m_player->setMedia(QUrl::fromLocalFile(filePath));
                mediaPlay();
                setWindowTitle(QFileInfo(filePath).fileName());
                for(int i=0;i<m_playlist->mediaCount();i++)
                {
                    if(m_player->currentMedia()==m_playlist->media(i))
                    {
                        m_playlist->setCurrentIndex(i);
                    }
                }
            }
        }
        else {
            if(ApisupportSuffix().contains(suffix,Qt::CaseSensitive))
            {
                ApiSetlocallistcurrentitem(ui->locallistWidget,filePath);
                m_player->setMedia(QUrl::fromLocalFile(filePath));
                mediaPlay();
                setWindowTitle(QFileInfo(filePath).fileName());
                for(int i=0;i<m_playlist->mediaCount();i++)
                {
                    if(m_player->currentMedia()==m_playlist->media(i))
                    {
                        m_playlist->setCurrentIndex(i);
                    }
                }
            }

        }
    }
}

void MainWindow::openUrlStr(const QString &str)
{
    if(!str.isEmpty())
    {
        m_player->setMedia(QUrl(str));
        m_player->play();
        setWindowTitle("online Src");
    }
}

void MainWindow::openUrlDialogShow()
{
    addUrlDialog dialog;
    dialog.exec();
}

void MainWindow::init()
{
    initUi();
    initNew();
    initConnect();
    initAllSetting();
    initKey();
    initRightMenu();
}

void MainWindow::initNew()
{
    m_player=new QMediaPlayer();
    m_url=new QMediaPlaylist();
    m_playlist=new QMediaPlaylist();

    m_mainScene=new QGraphicsScene();
    m_videoOut=new QGraphicsVideoItem();
    m_videoLOGO=new QGraphicsTextItem();


    m_mainScene->setBackgroundBrush(Qt::black);
    ui->graphicsView->setScene(m_mainScene);
    m_mainScene->addItem(m_videoOut);
    m_videoOut->setAspectRatioMode(Qt::IgnoreAspectRatio);

    m_player->setVolume(100);
    m_player->setVideoOutput(m_videoOut);
    m_player->setPlaylist(m_playlist);

    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    ui->graphicsView->viewport()->installEventFilter(this);
    ui->graphicsView->viewport()->setMouseTracking (true);
//    ui->ttbWidget->installEventFilter(this);
//    this->setMouseTracking (true);
//    setAttribute(Qt::WA_TransparentForMouseEvents, true);



}

void MainWindow::initConnect()
{
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::slotdurationChange);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::slotpositionChange);
    connect(m_player, SIGNAL(volumeChanged(int)), this, SLOT(slotvolumeChange(int)));
    connect(ui->movieSlider, SIGNAL(sliderReleased()), this, SLOT(slotsliderReleased()));
    //connect(ui->sliderProgress,SIGNAL(valueChanged(int)), this, SLOT(sliderProgressValueChanged(int)));
    connect(ui->movieSlider, SIGNAL(sliderMoved(int)), this, SLOT(slotsliderMoved(int)));
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), SLOT(slotmediaStatusChange(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(slotstateChange(QMediaPlayer::State)));
    connect(ui->voiceSlider, SIGNAL(valueChanged(int)), this, SLOT(slotsvoiceChange(int)));

    connect(App,&Application::sigopenUrlStr,this,&MainWindow::openUrlStr);

}

void MainWindow::initUi()
{
    ui->menuBar->hide();
    ui->statusBar->hide();
    ui->mainToolBar->hide();
    ui->switchListButton->hide();
    setWindowTitle(tr("Movie"));
    setWindowIcon(QIcon(":/image/play.svg"));

    QDesktopWidget *deskdop = QApplication::desktop();
    move((deskdop->width() - this->width())/2, (deskdop->height() - this->height())/2);

}

void MainWindow::initKey()
{
    //左右快进
    connect(new QShortcut(QKeySequence(Qt::Key_Left),this), &QShortcut::activated, this, [=]{
        m_player->setPosition(m_player->position() - 5000);
    });
    connect(new QShortcut(QKeySequence(Qt::Key_Right),this), &QShortcut::activated, this, [=]{
        m_player->setPosition(m_player->position() + 5000);
    });

    //左右切换影片
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left),this), &QShortcut::activated, this, &MainWindow::on_previousBtn_clicked);
    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right),this), &QShortcut::activated, this, &MainWindow::on_nextBtn_clicked);

    //暂停和播放
    connect(new QShortcut(QKeySequence(Qt::Key_Enter),this), &QShortcut::activated, this, &MainWindow::on_playBtn_clicked);
    connect(new QShortcut(QKeySequence(Qt::Key_Return),this), &QShortcut::activated, this, &MainWindow::on_playBtn_clicked);

    //全屏或者返回
    connect(new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Return),this), &QShortcut::activated, this, &MainWindow::on_fullScreenBtn_clicked);
    connect(new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Enter),this), &QShortcut::activated, this, &MainWindow::on_fullScreenBtn_clicked);

    //音量加减
    connect(new QShortcut(QKeySequence(Qt::Key_Up),this), &QShortcut::activated, this, [=]{

        int addVulume=m_player->volume()+5;
        if(addVulume>100)
        {
            m_player->setVolume(100);
        }
        else {
            m_player->setVolume(addVulume);
        }

    });
    connect(new QShortcut(QKeySequence(Qt::Key_Down),this), &QShortcut::activated, this, [=]{
        int downVulume=m_player->volume()-5;
        if(downVulume<0)
        {
            m_player->setVolume(0);
        }
        else {
            m_player->setVolume(downVulume);
        }
    });

    //打开文件
    connect(new QShortcut(QKeySequence(Qt::Key_O),this), &QShortcut::activated, this, &MainWindow::openFiles);
    connect(new QShortcut(QKeySequence(Qt::Key_U),this), &QShortcut::activated, this, &MainWindow::openUrlDialogShow);
}

void MainWindow::initRightMenu()
{
    m_RightClickMenu = new QMenu(this);

    m_actionPlay = new QAction("播放/暂停", this);
    m_actionopenFile = new QAction("打开文件", this);
    m_actionopenUrl = new QAction("打开Url", this);
    m_actionInfo = new QAction("影片信息", this);
    m_actionFullscreen = new QAction("全屏", this);
    m_actionCapture = new QAction("截图", this);

    m_RightClickMenu->addAction(m_actionPlay);
    m_RightClickMenu->addAction(m_actionopenFile);
    m_RightClickMenu->addAction(m_actionopenUrl);
    //    m_RightClickMenu->addAction(m_actionInfo);
    m_RightClickMenu->addAction(m_actionFullscreen);
    m_RightClickMenu->addAction(m_actionCapture);
    //    m_RightClickMenu->addAction(m_actionCapture);

    connect(m_actionPlay, &QAction::triggered, this, &MainWindow::on_playBtn_clicked);
    connect(m_actionopenFile, &QAction::triggered, this, &MainWindow::openFiles);
    connect(m_actionopenUrl, &QAction::triggered, this, &MainWindow::openUrlDialogShow);
    //    connect(m_actionInfo, &QAction::triggered, this, &MainWindow::openUrlDialogShow);
    connect(m_actionFullscreen, &QAction::triggered, this, &MainWindow::on_fullScreenBtn_clicked);
    connect(m_actionCapture, &QAction::triggered, this, &MainWindow::slotGrabCapture);
}

void MainWindow::resizeMovieWindow()
{
    ui->graphicsView->size();
    m_mainScene->setSceneRect(QRect(0, 0, (ui->graphicsView->size().width()-2) * m_playMultiple,(ui->graphicsView->size().height()-2)  *m_playMultiple));
    m_videoOut->setSize(QSize(m_mainScene->width(),m_mainScene->height()));
}

void MainWindow::setUITime()
{

    QTime timer(0,0,0);
    timer = timer.addMSecs(m_player->duration());
    m_totalTime=timer.toString("hh:mm:ss");
    if(nullptr!=m_totalTime)
    {
        ui->timeLabel->setText(m_currentTime+"/"+m_totalTime);
    }
    else {
        ui->timeLabel->setText("00:00:00/00:00:00");
    }
}

void MainWindow::setTimeEnd()
{
    m_currentTime="00:00:00";
    m_totalTime="00:00:00";
    ui->movieSlider->setValue(0);
    ui->timeLabel->setText("00:00:00/00:00:00");
}

void MainWindow::saveAllSetting()
{
    QSettings settings(SETTINGPATH,QSettings::IniFormat);
    settings.clear();
    settings.setValue("Movie/Voice",ui->voiceSlider->value());
    int indexLocal=1;
    if (m_player->isMuted()) {
        settings.setValue("Movie/VoiceState",1);
    } else {
        settings.setValue("Movie/VoiceState",2);
    }
    for(QString str :ApiGetListWidgetPaths(ui->locallistWidget))
    {
        settings.setValue("Movie/localPath"+QString::number(indexLocal++),str);
    }


}

void MainWindow::initAllSetting()
{
    QSettings settings(SETTINGPATH,QSettings::IniFormat);

    //取值本地地址
    QString strLocalPath;
    int localIndex=1;
    do
    {
        strLocalPath=settings.value("Movie/localPath"+QString::number(localIndex++)).toString();
        if(nullptr!=strLocalPath)
        {
            m_playlist->addMedia(QUrl::fromLocalFile(strLocalPath));
            //
            QListWidgetItem *item=new QListWidgetItem(ui->locallistWidget);
            item->setToolTip(strLocalPath);
            item->setText(QFileInfo(strLocalPath).fileName());


            ui->locallistWidget->setCurrentItem(item);
        }
    }while(nullptr!=strLocalPath);
    if(localIndex>1 ||ui->locallistWidget->count()>0)
    {
        ui->locallistWidget->setCurrentRow(0);
    }
    //上次音量大小
    int lastVoice= settings.value("Movie/Voice").toInt();
    ui->voiceSlider->setValue(lastVoice);
    m_player->setVolume(lastVoice);

    //上次音量状态
    int lastVoiceState= settings.value("Movie/VoiceState").toInt();
    if(1==lastVoiceState)
    {
        m_player->setMuted(true);
        ui->VoiceBtn->setIcon(QIcon(":/svg/audio-volume-muted-symbolic-dark.svg"));
    }
    else if(2==lastVoiceState)
    {
        m_player->setMuted(false);
        setMediaVoice(m_player->volume());
    }
    else
    {
        setMediaVoice(50);
    }
    ui->stackFrame->hide();
    resizeMovieWindow();
    ui->hideStackBtn->setIcon(QIcon(":/svg/pane-show.svg"));
}

void MainWindow::mediaPlay()
{
    m_player->play();
    resizeMovieWindow();
}

bool MainWindow::isExistencelocallist(const QString &path)
{
    bool iRet=false;
    if(ui->locallistWidget->findItems(path,Qt::MatchContains).count()>0)
    {
        iRet=true;
    }
    return iRet;
}

void MainWindow::playExistenceLocalPath(const QString &filename)
{
    m_player->setMedia(QUrl::fromLocalFile(filename));
    mediaPlay();
    setWindowTitle(QFileInfo(filename).fileName());
    for(int i=0;i<m_playlist->mediaCount();i++)
    {
        if(m_player->currentMedia()==m_playlist->media(i))
        {
            m_playlist->setCurrentIndex(i);
        }
    }
}

void MainWindow::setMediaVoice(int index)
{
    m_player->setVolume(index);
    if (index == 0) {
        ui->VoiceBtn->setIcon(QIcon(":/svg/audio-volume-muted-symbolic-dark.svg"));
    } else if (index < 50) {
        ui->VoiceBtn->setIcon(QIcon(":/svg/audio-volume-low-symbolic-dark.svg"));
    } else if (index <=100) {
        ui->VoiceBtn->setIcon(QIcon(":/svg/audio-volume-high-symbolic-dark.svg"));
    }
}

void MainWindow::openFiles()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Open File"),
                                                          windowTitle(),
                                                          tr("All videos (*)(%1)").arg(ApisupportSuffix().join(" "))
                                                          , 0,
                                                          QFileDialog::HideNameFilterDetails);
    int index=0;
    for(auto filename :filenames)
    {
        QFileInfo info(filename);
        filename=info.canonicalFilePath();

        if(!filename.isEmpty() && info.isFile()){
            if(!isExistencelocallist(filename))
            {
                m_playlist->addMedia(QUrl::fromLocalFile(filename));
                m_localPaths <<filename;
                QListWidgetItem *item=new QListWidgetItem(ui->locallistWidget);
                item->setText(info.fileName());
                item->setToolTip(filename);
                if(index==0)
                {
                    ui->locallistWidget->setCurrentItem(item);
                    m_player->setMedia(QUrl::fromLocalFile(filename));
                    mediaPlay();
                    setWindowTitle(QFileInfo(filename).fileName());
                    for(int i=0;i<m_playlist->mediaCount();i++)
                    {
                        if(m_player->currentMedia()==m_playlist->media(i))
                        {
                            m_playlist->setCurrentIndex(i);
                        }
                    }
                }

            }
            else {

                if(index==0)
                {
                    ApiSetlocallistcurrentitem(ui->locallistWidget,filename);
                    m_player->setMedia(QUrl::fromLocalFile(filename));
                    mediaPlay();
                    setWindowTitle(QFileInfo(filename).fileName());
                    for(int i=0;i<m_playlist->mediaCount();i++)
                    {
                        if(m_player->currentMedia()==m_playlist->media(i))
                        {
                            m_playlist->setCurrentIndex(i);
                        }
                    }
                }
            }
            index++;
        }
    }
}

void MainWindow::openFilesonly(const QStringList &filenames)
{
    for(auto filename :filenames)
    {
        QFileInfo info(filename);
        filename=info.canonicalFilePath();

        if(!filename.isEmpty() && info.isFile()){
            if(!isExistencelocallist(filename))
            {
                m_playlist->addMedia(QUrl::fromLocalFile(filename));
                m_localPaths <<filename;
                QListWidgetItem *item=new QListWidgetItem(ui->locallistWidget);
                item->setText(filename);
            }
        }
    }
}


void MainWindow::slotdurationChange(qint64 index)
{
    ui->movieSlider->setMaximum(index);
    QTime timer(0,0,0);
    timer = timer.addMSecs(index);
    m_totalTime=timer.toString("hh:mm:ss");
}

void MainWindow::slotpositionChange(qint64 index)
{
    if (!ui->movieSlider->isSliderDown())
        ui->movieSlider->setValue(index);

    QTime timer(0,0,0);
    timer = timer.addMSecs(index);
    m_currentTime=timer.toString("hh:mm:ss");
    setUITime();
}

void MainWindow::slotvolumeChange(int index)
{
    ui->voiceSlider->setValue(index);
    ui->voiceSlider->setToolTip(QString::number(index));
}

void MainWindow::slotsliderReleased()
{
    m_player->setPosition(ui->movieSlider->value());
}

void MainWindow::slotsliderMoved(int index)
{
    m_player->setPosition(index);
}

void MainWindow::slotstateChange(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        ui->playBtn->setIcon(QIcon(":/svg/media-playback-pause.svg"));
    } else if (state == QMediaPlayer::PausedState) {
        ui->playBtn->setIcon(QIcon(":/svg/media-playback-start.svg"));
    } else if (state == QMediaPlayer::StoppedState) {
        ui->playBtn->setIcon(QIcon(":/svg/media-playback-start.svg"));
    }
}

void MainWindow::slotmediaStatusChange(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {//自动播放下一个
        if (m_playlist->playbackMode() != QMediaPlaylist::CurrentItemOnce) {
            on_nextBtn_clicked();
        }
        else {
            on_stopBtn_clicked();
        }
    }
}

void MainWindow::slotsvoiceChange(int index)
{
    setMediaVoice(index);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeMovieWindow();
    return QMainWindow::resizeEvent(event);
}

void MainWindow::showEvent(QShowEvent *event)
{
    resizeMovieWindow();
    return QMainWindow::showEvent(event);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(m_RightClickMenu)
    {
        m_RightClickMenu->exec(QCursor::pos());
    }
//    event->accept();
    return QMainWindow::contextMenuEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    return QMainWindow::mouseMoveEvent(event);
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
//    if (obj == ui->graphicsView)
    {
        if (this->isFullScreen() && event->type() == QEvent::MouseMove)
        {
            qDebug()<<obj->objectName();
            {
                QPoint pos = mapFromGlobal(QCursor::pos());
                if (height() - 80 < pos.y()&& height() > pos.y())
                {
                    qDebug()<<"11111";
                    qDebug()<<"height:" <<height() <<"pos.y:"<<pos.y();
                    ui->ttbWidget->show();
                }
                else {
                    qDebug()<<"22222";
                    qDebug()<<"height:" <<height() <<"pos.y:"<<pos.y();
                    ui->ttbWidget->hide();
                }
            }
            return true;
        }
    }
        return QObject::eventFilter(obj, event);

}

void MainWindow::on_previousBtn_clicked()
{

    if(0==ui->stackedWidget->currentIndex()){
        m_playlist->previous();
        QMediaContent mdeia = m_playlist->media(m_playlist->currentIndex());
        int index=0;
        while(nullptr == mdeia.canonicalUrl().toString())
        {
            if(index>10)
            {
                return;//列表错误
            }
            index++;
            m_playlist->previous();
            mdeia = m_playlist->media(m_playlist->currentIndex());
        }
        m_player->setMedia(mdeia);
        mediaPlay();

        QString path = ApiUrl2localPath(m_player->currentMedia().canonicalUrl());
        ApiSetlocallistcurrentitem(ui->locallistWidget,path);
        setWindowTitle(QFileInfo(path).fileName());
    }
    else {
        m_url->previous();
        QMediaContent mdeia = m_url->media(m_url->currentIndex());
        m_player->setMedia(mdeia);
        mediaPlay();
    }
}

void MainWindow::on_playBtn_clicked()
{
    if(m_player){
        if (m_player->state() == QMediaPlayer::PlayingState) {
            m_player->pause();
        } else if (m_player->state() == QMediaPlayer::PausedState) {
            mediaPlay();
            QString path = ApiUrl2localPath(m_player->currentMedia().canonicalUrl());
            ApiSetlocallistcurrentitem(ui->locallistWidget,path);
            setWindowTitle(QFileInfo(path).fileName());
        } else if (m_player->state() == QMediaPlayer::StoppedState) {
            mediaPlay();
            QString path = ApiUrl2localPath(m_player->currentMedia().canonicalUrl());
            ApiSetlocallistcurrentitem(ui->locallistWidget,path);
            setWindowTitle(QFileInfo(path).fileName());
        }
    }
}

void MainWindow::on_nextBtn_clicked()
{
    if(0==ui->stackedWidget->currentIndex()){
        m_playlist->next();
        QMediaContent mdeia = m_playlist->media(m_playlist->currentIndex());
        int index=0;
        while(nullptr == mdeia.canonicalUrl().toString())
        {
            if(index>10)
            {
                return;//列表错误
            }
            index++;
            m_playlist->next();
            mdeia = m_playlist->media(m_playlist->currentIndex());
        }
        m_player->setMedia(mdeia);
        mediaPlay();
        QString path = ApiUrl2localPath(m_player->currentMedia().canonicalUrl());
        ApiSetlocallistcurrentitem(ui->locallistWidget,path);
        setWindowTitle(QFileInfo(path).fileName());
    }
    else {
        m_url->next();
        QMediaContent mdeia = m_url->media(m_url->currentIndex());
        m_player->setMedia(mdeia);
        mediaPlay();
    }
}

void MainWindow::on_stopBtn_clicked()
{
    m_player->stop();
    QTimer::singleShot(100,[=]{
        setTimeEnd();
        setWindowTitle(tr("Movie"));
    });

}

void MainWindow::on_fullScreenBtn_clicked()
{
    if (!this->isFullScreen())
    {
        showFullScreen();
//        ui->stackFrame->hide();
        ui->stackFrame->setVisible(false);
        if(m_actionFullscreen)
        {
            m_actionFullscreen->setText(tr("返回正常大小"));
        }
        resizeMovieWindow();
        ui->hideStackBtn->setIcon(QIcon(":/svg/pane-show.svg"));
        ui->fullScreenBtn->setIcon(QIcon(":/svg/view-restore.svg"));
    }
    else {
        showNormal();
        //        ui->stackFrame->show();
        if(m_actionFullscreen)
        {
            m_actionFullscreen->setText(tr("全屏"));
        }
        ui->ttbWidget->setVisible(true);
        resizeMovieWindow();
        ui->hideStackBtn->setIcon(QIcon(":/svg/pane-hide.svg"));
        ui->fullScreenBtn->setIcon(QIcon(":/svg/view-fullscreen.svg"));
    }
}

void MainWindow::on_VoiceBtn_clicked()
{
    if (m_player->isMuted()) {
        m_player->setMuted(false);
        setMediaVoice(m_player->volume());
    } else {
        m_player->setMuted(true);
        ui->VoiceBtn->setIcon(QIcon(":/svg/audio-volume-muted-symbolic-dark.svg"));
    }
}

void MainWindow::on_openFileBtn_clicked()
{
    openFiles();
}

void MainWindow::on_hideStackBtn_clicked()
{
    if(ui->stackFrame->isVisible())
    {
        ui->stackFrame->hide();
        QTimer::singleShot(50,[=]{
            resizeMovieWindow();
        });
        ui->hideStackBtn->setIcon(QIcon(":/svg/pane-show.svg"));
    }
    else {
        ui->stackFrame->show();
        QTimer::singleShot(50,[=]{
            resizeMovieWindow();
        });
        ui->hideStackBtn->setIcon(QIcon(":/svg/pane-hide.svg"));
    }
}

void MainWindow::on_locallistWidget_itemDoubleClicked(QListWidgetItem *item)
{
    playExistenceLocalPath(item->toolTip());
}

void MainWindow::on_localBtnAdd_clicked()
{
    openFiles();
}

void MainWindow::on_localBtnDel_clicked()
{
    if(nullptr!=ui->locallistWidget->currentItem())
    {
        QString filename=ui->locallistWidget->currentItem()->toolTip();
        QMediaContent  mediaContent = QMediaContent(QUrl::fromLocalFile(filename));
        for(int i=0;i<m_playlist->mediaCount();i++){
            if(mediaContent==m_playlist->media(i))
            {
                m_playlist->removeMedia(i);
                ui->locallistWidget->takeItem(ui->locallistWidget->currentRow());
            }
        }
    }
}

void MainWindow::slotGrabCapture()
{
    QPixmap pix = QPixmap::grabWidget(ui->graphicsView);
    QString path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) +"/" + \
            QDateTime::currentDateTime().toString()+QString::number(QDateTime::currentMSecsSinceEpoch())+".png";
    pix.save(path);
}


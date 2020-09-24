#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTime>
#include <QTimer>
#include <QListWidgetItem>
#include <QSettings>

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

void MainWindow::init()
{
    initUi();
    initNew();
    initConnect();
    initAllSetting();
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
    connect(ui->voiceSlider, SIGNAL(valueChanged(int)), this, SLOT(slotsvoiceChange(int)));
}

void MainWindow::initUi()
{
    ui->menuBar->hide();
    ui->statusBar->hide();
    ui->mainToolBar->hide();
    setWindowTitle(tr("are you crazzy!!!"));
    //    setStyleSheet("color:gray; background-color:black;");
    //    setStyleSheet("border:0px solid red;");
}

void MainWindow::resizeMovieWindow()
{
    ui->graphicsView->size();
    m_mainScene->setSceneRect(QRect(0, 0, (ui->graphicsView->size().width()-2) * m_playMultiple,(ui->graphicsView->size().height()-2)  *m_playMultiple));
    m_videoOut->setSize(QSize(m_mainScene->width(),m_mainScene->height()));
}

void MainWindow::setUITime()
{
    ui->timeLabel->setText(m_currentTime+"/"+m_totalTime);
}

void MainWindow::setTimeEnd()
{
    m_currentTime="00:00:00";
    m_totalTime="00:00:00";
    ui->movieSlider->setValue(0);
}

void MainWindow::saveAllSetting()
{
    QSettings settings(SETTINGPATH,QSettings::IniFormat);
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
            item->setText(strLocalPath);
            ui->locallistWidget->setCurrentItem(item);
        }
    }while(nullptr!=strLocalPath);
    //上次音量大小
    int lastVoice= settings.value("Movie/Voice").toInt();
    ui->voiceSlider->setValue(lastVoice);
    m_player->setVolume(lastVoice);

    //上次音量状态
    int lastVoiceState= settings.value("Movie/VoiceState").toInt();
    if(1==lastVoiceState)
    {
        m_player->setMuted(true);
    }
    else
    {
        m_player->setMuted(false);
    }

}

void MainWindow::mediaPlay()
{
    m_player->play();
    resizeMovieWindow();
}

bool MainWindow::isExistencelocallist(const QString &path)
{
    bool iRet=true;
    if(ui->locallistWidget->findItems(path,Qt::MatchContains).count()>0)
    {
        iRet=false;
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
    m_player->setVolume(index);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeMovieWindow();
    return QMainWindow::resizeEvent(event);
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
        } else if (m_player->state() == QMediaPlayer::StoppedState) {
            mediaPlay();
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
        setUITime();
    });

}

void MainWindow::on_fullScreenBtn_clicked()
{
    if (!this->isFullScreen())
    {
        showFullScreen();
        ui->stackFrame->hide();
        resizeMovieWindow();
    }
    else {
        showNormal();
        ui->stackFrame->show();
        resizeMovieWindow();
    }
}

void MainWindow::on_VoiceBtn_clicked()
{
    if (m_player->isMuted()) {
        m_player->setMuted(false);
    } else {
        m_player->setMuted(true);
    }
}

void MainWindow::on_openFileBtn_clicked()
{
//    QString filename = QFileDialog::getOpenFileName(this, "打开媒体文件", nullptr);
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Open File"),
                                                          windowTitle(),
                                                          tr("All videos (*)(%1)").arg(ApisupportSuffix().join(" "))
                                                          , 0,
                                                          QFileDialog::HideNameFilterDetails);
    int index=0;
    for(auto filename :filenames)
    {
        if(!filename.isEmpty()){
            m_playlist->addMedia(QUrl::fromLocalFile(filename));
            if(isExistencelocallist(filename))
            {
                m_localPaths <<filename;
                QListWidgetItem *item=new QListWidgetItem(ui->locallistWidget);
                item->setText(filename);
                if(index==0)
                {
                    ui->locallistWidget->setCurrentItem(item);
                    m_player->setMedia(QUrl::fromLocalFile(filename));
                    mediaPlay();
                    setWindowTitle(QFileInfo(filename).fileName());
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

void MainWindow::on_hideStackBtn_clicked()
{

    if(ui->stackFrame->isVisible())
    {
        ui->stackFrame->hide();
        resizeMovieWindow();
    }
    else {
        ui->stackFrame->show();
        resizeMovieWindow();
    }
}

void MainWindow::on_locallistWidget_itemDoubleClicked(QListWidgetItem *item)
{
    playExistenceLocalPath(item->text());
}

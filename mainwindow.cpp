#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTime>
#include <QTimer>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    initNew();
    initConnect();
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

//   this->
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

bool MainWindow::isExistencelocallist(const QString &path)
{
    bool iRet=true;
    if(ui->locallistWidget->findItems(path,Qt::MatchContains).count()>0)
    {
        iRet=false;
    }
    return iRet;
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
    ui->movieSlider->setValue(index);
    ui->movieSlider->setToolTip(QString::number(index));
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
        m_player->play();

        QString path = ApiUrl2localPath(m_player->currentMedia().canonicalUrl());
        ApiSetlocallistcurrentitem(ui->locallistWidget,path);
    }
    else {
        m_url->previous();
        QMediaContent mdeia = m_url->media(m_url->currentIndex());
        m_player->setMedia(mdeia);
        m_player->play();
    }
}

void MainWindow::on_playBtn_clicked()
{
    if(m_player){
        if (m_player->state() == QMediaPlayer::PlayingState) {
            m_player->pause();
        } else if (m_player->state() == QMediaPlayer::PausedState) {
            m_player->play();
        } else if (m_player->state() == QMediaPlayer::StoppedState) {
            m_player->play();
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
        m_player->play();
        QString path = ApiUrl2localPath(m_player->currentMedia().canonicalUrl());
        ApiSetlocallistcurrentitem(ui->locallistWidget,path);
    }
    else {
        m_url->next();
        QMediaContent mdeia = m_url->media(m_url->currentIndex());
        m_player->setMedia(mdeia);
        m_player->play();
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
        ui->menuBar->hide();
        ui->statusBar->hide();
    }
    else {
        showNormal();
        ui->menuBar->show();
        ui->statusBar->show();
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
    QString filename = QFileDialog::getOpenFileName(this, "打开媒体文件", nullptr);
    if(!filename.isEmpty()){
        m_player->setMedia(QUrl::fromLocalFile(filename));
        m_player->play();
        setWindowTitle(QFileInfo(filename).fileName());
        m_playlist->addMedia(QUrl::fromLocalFile(filename));
        resizeMovieWindow();
        QTimer::singleShot(1000,[=]{
//            QStringList SLMD = m_player->availableMetaData();
//            //qDebug() << SLMD;
//            QString str;
//            for(int i=0; i<SLMD.size(); i++){
//                if(SLMD.at(i)=="PixelAspectRatio" || SLMD.at(i)=="Resolution"){
//                    str= SLMD.at(i) + ": " + QString::number(m_player->metaData(SLMD.at(i)).toSize().width()) + " X " + QString::number(m_player->metaData(SLMD.at(i)).toSize().height()) + "\n";
//                    qDebug()<<str;
//                }else{
//                    str= SLMD.at(i) + ": " + m_player->metaData(SLMD.at(i)).toString() + "\n";
//                    qDebug()<<str;
//                }
//            }

//            qDebug()<<m_player->mediaStatus();
//            qDebug()<<m_player-> isMuted() ;
            qDebug()<<m_player-> isAudioAvailable() ;
            qDebug()<<m_player-> isVideoAvailable() ;
            qDebug()<<m_player-> isSeekable() ;
            if(m_player-> isSeekable() && isExistencelocallist(filename))
            {
                m_localPaths <<filename;
                QListWidgetItem *item=new QListWidgetItem(ui->locallistWidget);
                item->setText(filename);
                ui->locallistWidget->setCurrentItem(item);
            }
        });

    }
}

void MainWindow::on_hideStackBtn_clicked()
{
//    ui->hideStackBtn->
    if(ui->stackFrame->isVisible())
    {
        ui->stackFrame->hide();
    }
    else {
        ui->stackFrame->show();
    }
}

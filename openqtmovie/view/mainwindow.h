#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsVideoItem>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGraphicsScene>

#include "normalAPI.h"
#include "addurldialog.h"
#include "application.h"

#define SETTINGPATH "/tmp/openqtmovie/config.ini"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void acceptargv(int &argc, char **argv);

    //整体初始化函数
    void init();

    //初始化类创建
    void initNew();

    //初始化信号槽连接
    void initConnect();

    //初始化UI
    void initUi();

    //初始化快捷键
    void initKey();

    //初始化右键菜单
    void initRightMenu();

    //自适应窗口
    void resizeMovieWindow();

    //播放时间设置
    void setUITime();

    //设置结束时间
    void setTimeEnd();

    //保存所有配置
    void saveAllSetting();

    //初始化配置文件
    void initAllSetting();

    //自适应播放
    void mediaPlay();

    //是否在本地文件list中存在
    bool isExistencelocallist(const QString &path);

    //播放已经存在的本地文件
    void playExistenceLocalPath(const QString & filename);

    //设置音量
    void setMediaVoice(int index);

    //openFiles
    void openFiles();

    void openFilesonly(const QStringList &fileList);

    void openFileonly(const QString &filename);

    void openFileandPlay(const QString &filename);

    //openUrl
    void openUrlDialogShow();

    void openUrlStr(const QString &str);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void on_previousBtn_clicked();
    void on_playBtn_clicked();
    void on_nextBtn_clicked();
    void on_stopBtn_clicked();
    void on_fullScreenBtn_clicked();
    void on_VoiceBtn_clicked();
    void on_openFileBtn_clicked();
    void slotdurationChange(qint64 index);
    void slotpositionChange(qint64 index);
    void slotvolumeChange(int index);
    void slotsliderReleased();
    void slotsliderMoved(int index);

    void slotstateChange(QMediaPlayer::State state);
    void slotmediaStatusChange(QMediaPlayer::MediaStatus status);
    void slotsvoiceChange(int index);

    void on_hideStackBtn_clicked();

    void on_locallistWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_localBtnAdd_clicked();

    void on_localBtnDel_clicked();

private:
    Ui::MainWindow *ui;

    QMediaPlayer * m_player{nullptr};
    QMediaPlaylist *m_url{nullptr};
    QMediaPlaylist *m_playlist{nullptr};

    QGraphicsScene *m_mainScene{nullptr};
    QGraphicsVideoItem *m_videoOut{nullptr};
    QGraphicsTextItem *m_videoLOGO{nullptr};

    QMenu *m_RightClickMenu{nullptr};
    float m_playMultiple {1};

    QString m_currentTime;
    QString m_totalTime;

    QStringList m_localPaths{nullptr};


    //右键菜单Action
    QAction * m_actionPlay ;
    QAction * m_actionopenFile ;
    QAction * m_actionopenUrl  ;
    QAction * m_actionInfo ;
    QAction * m_actionFullscreen ;
    QAction * m_actionCapture ;
};

#endif // MAINWINDOW_H

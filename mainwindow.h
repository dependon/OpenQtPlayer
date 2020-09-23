#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsVideoItem>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGraphicsScene>

#include "normalAPI.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void initNew();
    void initConnect();

    void resizeMovieWindow();

    void setUITime();

    void setTimeEnd();

    bool isExistencelocallist(const QString &path);
protected:
    void resizeEvent(QResizeEvent *event) override;
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

    void slotmediaStatusChange(QMediaPlayer::MediaStatus status);
    void slotsvoiceChange(int index);

    void on_hideStackBtn_clicked();

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
};

#endif // MAINWINDOW_H

#include "normalAPI.h"


QString ApiUrl2localPath(const QUrl &url)
{
    QString path = url.toString();
    if(path.startsWith("file://"))
        path = path.mid(7);
    return path;
}

bool ApiSetlocallistcurrentitem(QListWidget *listWidget, const QString &path, Qt::MatchFlags flag)
{
    int iRet=false;
    QList<QListWidgetItem*> listItems = listWidget->findItems(path,flag);
    if(1==listItems.count())
    {
        listWidget->setCurrentItem(listItems.first());
        iRet=true;
    }
    return iRet;
}

QStringList ApiGetListWidgetPaths(QListWidget *listWidget)
{
    int row=0;
    QStringList list;
    QString line;
    while(row<(listWidget->count()))
    {
        list << listWidget->item(row)->toolTip();
        row++;
    }
    return list;
}

const QStringList ApisupportMovieSuffix()
{
    const QStringList video_filetypes {
        "*.3g2", "*.3ga", "*.3gp", "*.3gp2", "*.3gpp", "*.amv", "*.asf", "*.asx", "*.avf", "*.avi", "*.bdm", "*.bdmv",\
        "*.bik", "*.clpi", "*.cpi", "*.dat", "*.divx", "*.drc", "*.dv", "*.dvr-ms", "*.f4v", "*.flv", "*.gvi", "*.gxf", \
        "*.hdmov", "*.hlv", "*.iso", "*.letv", "*.lrv", "*.m1v", "*.m2p", "*.m2t", "*.m2ts", "*.m2v", "*.m3u", "*.m3u8",\
        "*.m4v", "*.mkv", "*.moov", "*.mov", "*.mov", "*.mp2", "*.mp2v", "*.mp4", "*.mp4v", "*.mpe", "*.mpeg", "*.mpeg1",\
        "*.mpeg2", "*.mpeg4", "*.mpg", "*.mpl", "*.mpls", "*.mpv", "*.mpv2", "*.mqv", "*.mts", "*.mts", "*.mtv", "*.mxf", \
        "*.mxg", "*.nsv", "*.nuv", "*.ogg", "*.ogm", "*.ogv", "*.ogx", "*.ps", "*.qt", "*.qtvr", "*.ram", "*.rec", "*.rm", \
        "*.rm", "*.rmj", "*.rmm", "*.rms", "*.rmvb", "*.rmx", "*.rp", "*.rpl", "*.rv", "*.rvx", "*.thp", "*.tod", "*.tp",\
        "*.trp", "*.ts", "*.tts", "*.txd", "*.vcd", "*.vdr", "*.vob", "*.vp8", "*.vro", "*.webm", "*.wm", "*.wmv", "*.wtv", "*.xesc", "*.xspf"
    };
    return video_filetypes;

}

const QStringList ApisupportMusicSuffix()
{
    const QStringList audio_filetypes {"*.mp3", "*.ogg", "*.wav", "*.wma", "*.m4a", "*.aac", "*.ac3", "*.ape", "*.flac", "*.ra", "*.mka", "*.dts", "*.opus"};
    return audio_filetypes;
}

const QStringList ApisupportSuffix()
{
     const QStringList filetypes {
        "*.3g2", "*.3ga", "*.3gp", "*.3gp2", "*.3gpp", "*.amv", "*.asf", "*.asx", "*.avf", "*.avi", "*.bdm", "*.bdmv",\
        "*.bik", "*.clpi", "*.cpi", "*.dat", "*.divx", "*.drc", "*.dv", "*.dvr-ms", "*.f4v", "*.flv", "*.gvi", "*.gxf", \
        "*.hdmov", "*.hlv", "*.iso", "*.letv", "*.lrv", "*.m1v", "*.m2p", "*.m2t", "*.m2ts", "*.m2v", "*.m3u", "*.m3u8",\
        "*.m4v", "*.mkv", "*.moov", "*.mov", "*.mov", "*.mp2", "*.mp2v", "*.mp4", "*.mp4v", "*.mpe", "*.mpeg", "*.mpeg1",\
        "*.mpeg2", "*.mpeg4", "*.mpg", "*.mpl", "*.mpls", "*.mpv", "*.mpv2", "*.mqv", "*.mts", "*.mts", "*.mtv", "*.mxf", \
        "*.mxg", "*.nsv", "*.nuv", "*.ogg", "*.ogm", "*.ogv", "*.ogx", "*.ps", "*.qt", "*.qtvr", "*.ram", "*.rec", "*.rm", \
        "*.rm", "*.rmj", "*.rmm", "*.rms", "*.rmvb", "*.rmx", "*.rp", "*.rpl", "*.rv", "*.rvx", "*.thp", "*.tod", "*.tp",\
        "*.trp", "*.ts", "*.tts", "*.txd", "*.vcd", "*.vdr", "*.vob", "*.vp8", "*.vro", "*.webm", "*.wm", "*.wmv", "*.wtv", "*.xesc", "*.xspf",\
         "*.mp3", "*.ogg", "*.wav", "*.wma", "*.m4a", "*.aac", "*.ac3", "*.ape", "*.flac", "*.ra", "*.mka", "*.dts", "*.opus"
    };
     return filetypes;
}


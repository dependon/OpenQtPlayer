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

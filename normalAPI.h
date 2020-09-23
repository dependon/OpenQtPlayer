#ifndef NORMALAPI_H
#define NORMALAPI_H

#include <QString>
#include <QUrl>
#include <QListWidgetItem>
#include <QListWidget>

QString ApiUrl2localPath(const QUrl & url);

bool ApiSetlocallistcurrentitem(QListWidget *listWidget, const QString &path,Qt::MatchFlags flag= Qt::MatchContains);


#endif // NORMALAPI_H

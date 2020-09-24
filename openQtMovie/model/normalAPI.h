#ifndef NORMALAPI_H
#define NORMALAPI_H

#include <QString>
#include <QStringList>
#include <QUrl>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMediaPlaylist>

QString ApiUrl2localPath(const QUrl & url);

bool ApiSetlocallistcurrentitem(QListWidget *listWidget, const QString &path,Qt::MatchFlags flag= Qt::MatchContains);


QStringList ApiGetListWidgetPaths(QListWidget *listWidget);

const QStringList ApisupportMovieSuffix();

const QStringList ApisupportMusicSuffix();

const QStringList ApisupportSuffix();

#endif // NORMALAPI_H

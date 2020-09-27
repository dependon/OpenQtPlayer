#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QCommandLineParser *commandLine= new QCommandLineParser();
    commandLine->process(a);

    QStringList toOpenFiles;
    if (commandLine->positionalArguments().length() > 0) {
        toOpenFiles = commandLine->positionalArguments();
    }
    qDebug()<<toOpenFiles;
    if(toOpenFiles.count()>0)
    {
        w.openFileandPlay(toOpenFiles.first());

    }
//    w.acceptargv(argc, argv);
    w.show();

    return a.exec();
}

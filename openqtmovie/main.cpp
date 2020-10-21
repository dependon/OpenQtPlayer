#include "mainwindow.h"
#include "application.h"
#include <QCommandLineParser>
int main(int argc, char *argv[])
{
    Application a(argc, argv);
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
    w.show();

    return a.exec();
}

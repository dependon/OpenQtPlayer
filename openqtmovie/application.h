#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#define App (static_cast<Application*>(QCoreApplication::instance()))

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int &argc, char **argv);
    ~Application();

signals:
    void sigopenUrlStr(const QString &);
};

#endif // APPLICATION_H

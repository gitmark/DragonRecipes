#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("plugins");
    QCoreApplication::setLibraryPaths(QStringList(dir.absolutePath()));
    a.setWindowIcon(QIcon(":/images/AppIcon.icns"));
    MainWindow w;
    w.show();
    return a.exec();
}

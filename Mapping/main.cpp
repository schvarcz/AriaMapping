#include <QApplication>
#include "mainwindow.h"
#include <X11/Xlib.h>

int main(int argc, char *argv[])
{
    XInitThreads();
    Aria::init();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int ret = a.exec();
    Aria::exit();
    return ret;
}

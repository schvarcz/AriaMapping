#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Aria::init();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int ret = a.exec();
    Aria::exit();
    return ret;
}

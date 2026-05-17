#include "mainwindow.hpp"
#include <QApplication>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
    srand(static_cast<unsigned>(time(nullptr)));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
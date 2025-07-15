/*
 * Created by Giuseppe Cioffi on 15/07/2025
 * Simple radar simulation with periodic beep sound using Qt6/C++
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

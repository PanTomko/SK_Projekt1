#include "mainwindow.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.connect_to_server();
    std::cout << "Hello World !" << std::endl;

    return a.exec();
}

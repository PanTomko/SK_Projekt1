#include "server.h"

#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //char file[255];
    Server server;
    server.start();

    return a.exec();
}

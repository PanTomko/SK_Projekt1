#include "server.h"

#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server server;
    server.start();

    return a.exec();
}

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    w.start_server();
    w.connect(&w.server, &QTcpServer::newConnection, &w, &MainWindow::accept_new_connection);

    return a.exec();
}

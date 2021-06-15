#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_server()
{
    server.listen(QHostAddress::Any, PORT);
    std::cout << server.errorString().toStdString() << std::endl;
}

void MainWindow::stop_server()
{
    server.close();
}

void MainWindow::accept_new_connection()
{
    if(server.hasPendingConnections())
    {
        QTcpSocket *socket = server.nextPendingConnection();

    }
}


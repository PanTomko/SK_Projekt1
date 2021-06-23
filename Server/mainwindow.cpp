#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    stop_server();
    delete ui;
}

void MainWindow::start_server()
{
    server.listen(QHostAddress::Any, PORT);
    std::cout << server.errorString().toStdString() << std::endl;
    std::cout << "Server start" << std::endl;
}

void MainWindow::stop_server()
{
    server.close();

    for(auto & x : connections){
        x->close();
        delete x;
    }
}

void MainWindow::accept_new_connection()
{
    if(server.hasPendingConnections())
    {
        QTcpSocket *socket = server.nextPendingConnection();
        socket->setObjectName( QString::number(socket->socketDescriptor()) );

        std::cout << "peer : " << socket->objectName().toStdString() << " connected !" << std::endl;

        connect( socket, &QAbstractSocket::disconnected, this, &MainWindow::on_peer_disconnect );

        connected++;
        connections.push_back( socket );
    }
}

bool MainWindow::delete_file(char filename[])
{
    char path[65] = "E:\\Rest\\Drive\\";
    strcat(path, filename);

    if (remove(path) != 0){
        perror("File deletion failed");
        return false;
    }
    else {
        file_list.erase(std::remove(file_list.begin(), file_list.end(), filename), file_list.end());
        std::cout << "File deleted successfully";
        return true;
    }

void MainWindow::on_peer_disconnect()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    std::cout << "peer : " << socket->objectName().toStdString() << " disconnected !" << std::endl;

    connected--;
    connections.erase( std::find_if(connections.begin(), connections.end(), [=]( auto x){ return x->objectName() == socket->objectName();} ));

    delete socket;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
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
        delete x;
    }
}

void MainWindow::accept_new_connection()
{
    if(server.hasPendingConnections())
    {
        Client *client = new Client{server.nextPendingConnection()};
        client->socket->setObjectName( QString::number(client->socket->socketDescriptor()) );
        client->th = new std::thread( &Client::run, client );

        std::cout << "peer : " << client->socket->objectName().toStdString() << " connected !" << std::endl;

        connect( client->socket, &QAbstractSocket::disconnected, this, &MainWindow::on_peer_disconnect );

        connected++;
        connections.push_back( client );
    }
}

void MainWindow::on_peer_disconnect()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    std::cout << "peer : " << socket->objectName().toStdString() << " disconnected !" << std::endl;

    connected--;

    auto it = std::find_if(connections.begin(), connections.end(), [=]( auto x){ return x->socket->objectName() == socket->objectName();} );

    delete  *it;

    connections.erase( it );
}

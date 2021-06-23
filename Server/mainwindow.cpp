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
        client->th = new std::thread( &MainWindow::run_client, this, client );

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

void MainWindow::run_client(Client *client)
{
    while(client->is_running())
    {
        QByteArray data = client->socket->read(16);

        if(!data.isEmpty())
        {
            if(!strcmp(data.data(), "UP")) tokenH_upload_file(client);
            else if(!strcmp(data.data(), "DEL")) tokenH_delete_file(client);
            else if(!strcmp(data.data(), "DOWN")) tokenH_download_file(client);
            else std::cout << "wrong token : " << data.data() << std::endl;
        }

        for(auto & bc : client->broadcast_list )
        {
            client->socket->write(bc, sizeof(bc));
            client->socket->waitForReadyRead(); // let client process msg form client is not important
        }
    }
}

void MainWindow::tokenH_upload_file(const Client *client)
{
    std::cout << "Upload form : "<< client->socket->socketDescriptor() << std::endl;
}

void MainWindow::tokenH_delete_file(const Client *client){}
void MainWindow::tokenH_download_file(const Client *client){}

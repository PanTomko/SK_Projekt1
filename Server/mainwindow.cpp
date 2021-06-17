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
    std::cout << "test" << std::endl;
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

void MainWindow::on_peer_disconnect()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    std::cout << "peer : " << socket->objectName().toStdString() << " disconnected !" << std::endl;

    connected--;
    connections.erase( std::find_if(connections.begin(), connections.end(), [=]( auto x){ return x->objectName() == socket->objectName();} ));

    delete socket;
}

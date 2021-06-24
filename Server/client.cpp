#include "client.h"

#include <iostream>

Client::Client(qintptr socketDescriptor, QObject *parent) : QThread(parent)
{
    std::cout << "client made : " << socketDescriptor << '.' << std::endl;
    this->socketDescriptor = socketDescriptor;
}

Client::~Client()
{
    std::cout << "delete client " << socketDescriptor << '.' << std::endl;
    wait();
}

void Client::run()
{
    std::cout << "client run : " << socketDescriptor << '.' << std::endl;

    ini();

    while(true)
    {
        if(socket->waitForReadyRead()){
            std::cout << '.';
        }
    }

    exec();
}

void Client::ini()
{
    std::cout << "client ini : " << socketDescriptor << '.' << std::endl;
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor)){
        std::cout << "socket error : " << socketDescriptor << '.' << std::endl;
        emit error(socket->error());
        return;
    }

    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    connect(socket, &QIODevice::readyRead, this, &Client::readyRead, Qt::DirectConnection);
    connect(socket, &QAbstractSocket::disconnected, this, &Client::disconnected, Qt::DirectConnection);

    //socket->connect(socket, &QIODevice::aboutToClose, this, &Client::client_disconncted);
}

void Client::readyRead()
{
    // Code
}

void Client::disconnected()
{
    std::cout << "peer disconnected : " << socketDescriptor << std::endl;

    emit clientDisconected(this);

    socket->deleteLater();
    exit(0);
}

#include "client.h"
#include "token.h"

#include <iostream>

Client::Client(qintptr socketDescriptor, QObject *parent) : QThread(parent)
{
    std::cout << "client made : " << socketDescriptor << '.' << std::endl;
    this->socketDescriptor = socketDescriptor;
}

Client::~Client()
{
    wait();
    std::cout << "delete client : " << socketDescriptor << '.' << std::endl;
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

    connect(socket, &QAbstractSocket::readyRead, this, &Client::readyRead, Qt::DirectConnection);
    connect(socket, &QAbstractSocket::disconnected, this, &Client::disconnected, Qt::DirectConnection);

    _running = true;
}

TOKEN Client::readTOKEN()
{
    QByteArray data = socket->read(sizeof(TOKEN));
    return toToken(data);
}

void Client::writeTOKEN(TOKEN token)
{
    socket->write((char*)&token, sizeof(TOKEN));
}

void Client::run()
{
    std::cout << "client run : " << socketDescriptor << '.' << std::endl;

    ini();

    while(is_running())
    {
        for(auto & broadcast : broadcast_list)
        {
            socket->write(broadcast, sizeof(broadcast));
        }

        socket->waitForDisconnected(0);
    }

    exec();
}

void Client::readyRead()
{
    TOKEN token = readTOKEN();
    emit tokenRecived(this, token);
}

void Client::disconnected()
{
    std::cout << "peer disconnected : " << socketDescriptor << std::endl;

    emit clientDisconected(this);

    _running = false;

    socket->deleteLater();
    exit(0);
}

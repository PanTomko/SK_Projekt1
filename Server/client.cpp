#include "client.h"

#include <iostream>
#include <QByteArray>

Client::Client()
{
}

Client::Client(QTcpSocket *socket)
{
    this->socket = socket;
}

Client::~Client()
{
    _running = false;
    th->join();
    delete th;

    socket->close();
    delete socket;
}

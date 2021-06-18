#include "client.h"

#include <iostream>

Client::Client()
{
}

Client::Client(QTcpSocket *socket)
{
    this->socket = socket;
}

Client::~Client()
{

}

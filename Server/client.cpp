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

void Client::run()
{

    while(is_running())
    {
        QByteArray data = socket->read(16);
        if(data.count() > 0)
        {
            std::cout << data.constData() << std::endl;
        }

        for(auto & bc : broadcast_list )
        {
            // TODO : implement broadcast
        }
    }
}

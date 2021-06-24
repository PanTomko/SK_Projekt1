#include "server.h"

#include <iostream>

void Server::start()
{
    if(listen(QHostAddress::Any, _port))
        std::cout << "server start." << std::endl;
    else
        std::cout << "server failed start : " << errorString().toStdString() << '.' << std::endl;
}

Server::Server(QObject *parent) : QTcpServer(parent)
{
    std::cout << "server ini" << std::endl;
    _port = 2466;
}

void Server::incomingConnection(qintptr handle)
{
    std::cout << "peer connected : " << handle << '.' << std::endl;

    Client *client = new Client(handle);

    connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    connect(client, &Client::clientDisconected, this, &Server::on_client_disconnect);

    connected_peers.push_back(client);

    client->start();
}

void Server::on_client_disconnect(Client* client)
{
    std::cout << "server peer disconnected : " << client->socketDescriptor << std::endl;

    connected_peers.erase(std::find_if(connected_peers.begin(), connected_peers.end(),
                                       [=](auto x){return x->socketDescriptor == client->socketDescriptor;}));
}

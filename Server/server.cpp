#include "server.h"

#include <iostream>

void Server::start()
{
    if(listen(QHostAddress::Any, _port))
        std::cout << "server start." << std::endl;
    else
        std::cout << "server failed start : " << errorString().toStdString() << '.' << std::endl;
}

void Server::handleClientToken(Client *client, TOKEN token)
{
    std::cout << "(server) client " << client->socketDescriptor << " token : " << (int)token << '.' << std::endl;

    switch (token) {
        case TOKEN::TOKEN_UPLOAD:   handleToken_UPLOAD(client);     break;
        case TOKEN::TOKEN_DELETE:   handleToken_DELETE(client);     break;
        case TOKEN::TOKEN_DOWNLOAD: handleToken_DOWNLOAD(client);   break;
        default:
            std::cout << "client " << client->socketDescriptor << " wrong token." << std::endl;
            break;
    }
}

void Server::handleToken_UPLOAD(Client *client)
{

}

void Server::handleToken_DELETE(Client *client)
{

}

void Server::handleToken_DOWNLOAD(Client *client)
{

}

Server::Server(QObject *parent) : QTcpServer(parent)
{
    std::cout << "server ini." << std::endl;
    _port = 2466;
}

void Server::incomingConnection(qintptr handle)
{
    std::cout << "peer connected : " << handle << '.' << std::endl;

    Client *client = new Client(handle, this);

    connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    connect(client, &Client::clientDisconected, this, &Server::on_client_disconnect);
    connect(client, &Client::tokenRecived, this, &Server::handleClientToken, Qt::DirectConnection);

    connected_peers.push_back(client);

    client->start();
}

void Server::on_client_disconnect(Client* client)
{
    std::cout << "server peer disconnected : " << client->socketDescriptor << '.' << std::endl;

    connected_peers.erase(std::find_if(connected_peers.begin(), connected_peers.end(),
                                       [=](auto x){return x->socketDescriptor == client->socketDescriptor;}));
}

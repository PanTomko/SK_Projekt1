#include "server.h"

#include <iostream>

void Server::start()
{
    if(listen(QHostAddress::Any, _port))
        std::cout << "server start." << std::endl;
    else
        std::cout << "server failed start : " << errorString().toStdString() << '.' << std::endl;

    _mkdir("data"); // creating directory if such don't exist
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
    client->writeTOKEN(TOKEN::TOKEN_OK); // one for locking msg
    client->writeTOKEN(TOKEN::TOKEN_OK); // one for update

    client->socket->waitForReadyRead();
    QString file_name = client->socket->read(255).data();
    std::cout << "file name : " << file_name.toStdString() << '.' << std::endl;

    client->writeTOKEN(TOKEN::TOKEN_OK);

    client->socket->waitForReadyRead();
    qint64 file_size = *(qint64*)client->socket->read(sizeof(qint64)).data();
    std::cout << "file size : " << file_size << '.' << std::endl;

    client->writeTOKEN(TOKEN::TOKEN_OK);
    client->socket->flush(); // this is so QT dont wait for its turn in event loop

    QFile file( "data/" + file_name );
    file.open(QIODevice::WriteOnly);

    while(file_size != 0)
    {
        client->socket->waitForReadyRead();
        QByteArray data = client->socket->readAll();
        file.write(data.data(), data.size());
        file_size -= data.size();
        std::cout << file_size << std::endl;
    }

    file.close();
    file_list.push_back(file_name.toStdString());
}

void Server::handleToken_DELETE(Client *client)
{

}

void Server::handleToken_DOWNLOAD(Client *client)
{

}

void Server::broadcast(TOKEN token, std::string file_name)
{
    for(auto client : connected_peers )
    {
        //client->broadcast_list
    }
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

bool Server::delete_file(char filename[])
{
    if (remove(filename) != 0){
        perror("File deletion failed");
        return false;
    }
    else {
        std::vector<std::string>::iterator position = std::find(file_list.begin(), file_list.end(), filename);
        if (position != file_list.end()) {
            file_list.erase(position);
        }
        std::cout << "File deleted successfully" << std::endl;
        return true;
    }
}

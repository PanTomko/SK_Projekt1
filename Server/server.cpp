#include "server.h"

#include <iostream>

void Server::start()
{
    if(listen(QHostAddress::Any, _port))
        std::cout << "server start." << std::endl;
    else
        std::cout << "server failed start : " << errorString().toStdString() << '.' << std::endl;

    _mkdir("data"); // creating directory if such don't exist
    setFileList();
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
    }

    file.close();
    file_list.push_back(file_name.toStdString());

    broadcast(TOKEN::TOKEN_UPLOADED, file_name.toStdString());

    std::cout << "upload done." << std::endl;
}

void Server::handleToken_DELETE(Client *client)
{
    client->writeTOKEN(TOKEN::TOKEN_OK); // one for locking msg
    client->writeTOKEN(TOKEN::TOKEN_OK); // one for update

    client->socket->waitForReadyRead();
    char file_name[255];
    client->socket->read(file_name, 255);
    std::string name = file_name;
    if(fileExist(file_name))
    {
        delete_file(file_name);
        client->writeTOKEN(TOKEN::TOKEN_OK);
    }
    else
    {
        client->writeTOKEN(TOKEN::TOKEN_ABORT);
        std::cout << "file asked for doesn't exist." << std::endl;
    }
    broadcast(TOKEN::TOKEN_DELETED, name);

    std::cout << "deletion done." << std::endl;
}

void Server::handleToken_DOWNLOAD(Client *client)
{
    client->writeTOKEN(TOKEN::TOKEN_OK); // one for locking msg
    client->writeTOKEN(TOKEN::TOKEN_OK); // one for update

    client->socket->waitForReadyRead();
    char file_name[255];
    client->socket->read(file_name, 255);

    if(fileExist(file_name))
    {
        client->writeTOKEN(TOKEN::TOKEN_OK);
    }
    else
    {
        client->writeTOKEN(TOKEN::TOKEN_ABORT);
        std::cout << "file asked for doesn't exist." << std::endl;
    }


    QFile file(tr("data/") + file_name);
    file.open(QIODevice::ReadOnly);
    qint64 file_size = file.size();

    std:: cout << "file_path : " << (tr("data/") + file_name).toStdString() << '.' << std::endl;
    std:: cout << "file_size : " << file_size << '.' << std::endl;

    client->socket->write((char*)&file_size, sizeof(qint64));

    while(file_size != 0)
    {
        QByteArray data = file.read(1024*4);
        int sended = 0;
        while(sended != data.size())
        {
            sended += client->socket->write(data.data() + sended, data.size() - sended);
            client->socket->flush();
            client->socket->waitForDisconnected(0); // if peer sudenly disconnect QT evenet loop will kick in
        }
        file_size -= sended;
    }

    file.close();
}

void Server::onClientReady(Client *client)
{
    int size = file_list.size();
    client->socket->write((char*)&size, sizeof(int));

    for (auto & name : file_list)
    {
        char buffer[255];
        strcpy(buffer, name.c_str());
        client->socket->write(buffer, sizeof (buffer));
    }
    client->socket->flush();
}

void Server::broadcast(TOKEN token, std::string msg)
{
    for(auto client : connected_peers )
    {
        Broadcast broadcast;
        broadcast.token = token;
        strcpy(broadcast.msg, msg.c_str());

        client->broadcast_list.push_back(broadcast);
    }
}

bool Server::fileExist(std::string file)
{
    if(std::find(file_list.begin(), file_list.end(), file) != file_list.end())
        return true;
    else
        return false;
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
    connect(client, &Client::socketReady, this, &Server::onClientReady, Qt::DirectConnection);

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
    char path[260] = "data\\";
    strcat(path, filename);

    if (remove(path) != 0){
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

void Server::setFileList()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("data\\")) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
          file_list.push_back(ent->d_name);
      }
      closedir (dir);
    }
    file_list.erase(file_list.begin(),file_list.begin()+2);
}

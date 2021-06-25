#ifndef SERVER_H
#define SERVER_H

#include "client.h"

#include <vector>

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT
public:

    void start();
    void handleClientToken(Client* client, TOKEN token);

    void handleToken_UPLOAD(Client* client);
    void handleToken_DELETE(Client* client);
    void handleToken_DOWNLOAD(Client* client);

    explicit Server(QObject *parent = nullptr);

protected:
    void incomingConnection( qintptr handle ) override;

private:
    void on_client_disconnect(Client* client);

    std::vector<Client*> connected_peers;

    int _port;
};

#endif // SERVER_H

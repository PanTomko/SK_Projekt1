#ifndef SERVER_H
#define SERVER_H

#include "client.h"

#include <vector>

// windows only
#include <direct.h>

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

    void broadcast(TOKEN token, std::string msg);

    explicit Server(QObject *parent = nullptr);
    std::vector<std::string> file_list;

protected:
    void incomingConnection( qintptr handle ) override;

private:
    void on_client_disconnect(Client* client);

    std::vector<Client*> connected_peers;
    bool delete_file(char filename[]);

    int _port;
};

#endif // SERVER_H

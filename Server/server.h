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

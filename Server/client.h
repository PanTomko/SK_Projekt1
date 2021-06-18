#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <vector>

//#include <mutex>

class Client
{
public:

    std::vector<char[271]>broadcast_list;
   // std::mutex active_transmission;

    QTcpSocket *socket;

    Client();
    Client(QTcpSocket*socket);

    virtual ~Client();
};

#endif // CLIENT_H

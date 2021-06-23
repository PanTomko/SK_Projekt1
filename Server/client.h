#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <vector>

#include <thread>
#include <mutex>

class Client
{
public:

    std::vector<char[271]>broadcast_list;
   // std::mutex active_transmission;

    QTcpSocket *socket;
    std::thread *th;

    bool is_running() { return _running; }


    Client();
    Client(QTcpSocket*socket);

    virtual ~Client();
private:
    bool _running = true;
};

#endif // CLIENT_H

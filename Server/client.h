#ifndef CLIENT_H
#define CLIENT_H

#include "token.h"
#include "broadcast.h"

#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QFile>

#include <vector>

class Client : public QThread
{
    Q_OBJECT
public:
    QTcpSocket *socket;
    QMutex mutex_broadcast_list;
    qintptr socketDescriptor;

    TOKEN readTOKEN();
    void writeTOKEN(TOKEN token);
    void writeBroadcast(Broadcast* broadcast);

    bool is_running(){ return _running;};

    void run() override;

    std::vector<Broadcast> broadcast_list;

    Client(qintptr socketDescriptor, QObject *parent = 0);
    virtual ~Client();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void clientReady(Client* client);
    void clientDisconected(Client* client);

    void tokenRecived(Client* client, TOKEN token);

public slots:
    void readyRead();
    void disconnected();

private:
    void ini();
    bool _running;
};

#endif // CLIENT_H

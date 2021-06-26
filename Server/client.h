#ifndef CLIENT_H
#define CLIENT_H

#include "token.h"

#include <QTcpSocket>
#include <QThread>
#include <QFile>

#include <vector>

class Client : public QThread
{
    Q_OBJECT
public:
    QTcpSocket *socket;
    qintptr socketDescriptor;

    TOKEN readTOKEN();
    void writeTOKEN(TOKEN token);

    bool is_running(){ return _running;};

    void run() override;

    std::vector<char[255 + sizeof(TOKEN)]> broadcast_list;

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

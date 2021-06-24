#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QThread>

class Client : public QThread
{
    Q_OBJECT
public:
    QTcpSocket *socket;
    qintptr socketDescriptor;

    void run() override;

    Client(qintptr socketDescriptor, QObject *parent = 0);
    virtual ~Client();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void clientReady(Client* client);
    void clientDisconected(Client* client);

public slots:
    void readyRead();
    void disconnected();

private:
    void ini();
};

#endif // CLIENT_H

#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTextStream>
#include "servercontroller.h"

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(QObject *parent = nullptr);
    void startServer();

private slots:
    void handleNewConnection();
    //void readPlayerCount();

private:
    QTcpServer *tcpServer;
    ServerController *serverController;
    int clientIdCounter = 1;
    int desiredPlayers = 0;
    QTextStream standardInput;
};

#endif // GAMESERVER_H

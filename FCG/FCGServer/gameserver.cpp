#include "gameserver.h"
#include <QCoreApplication>
#include <QHostAddress>
#include <qtcpsocket.h>

const quint16 PORT = 12345;

GameServer::GameServer(QObject *parent)
    : QObject(parent),
    standardInput(stdin)
{
    tcpServer = new QTcpServer(this);
    //serverController = new ServerController(this);
}

void GameServer::startServer()
{
    while (desiredPlayers < 1 || desiredPlayers > 4) {
        qInfo() << "请输入游戏玩家数量(1-4):";
        standardInput >> desiredPlayers;
        standardInput.readLine();
    }
    //serverController->setDesiredPlayers(desiredPlayers);

    if (!tcpServer->listen(QHostAddress::Any, PORT)) {
        qCritical() << "无法启动服务器:" << tcpServer->errorString();
        QCoreApplication::exit(EXIT_FAILURE);
        return;
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &GameServer::handleNewConnection);
    qInfo() << "服务器启动，监听端口" << PORT;
}

void GameServer::handleNewConnection()
{
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
        qInfo() << "客户端" << clientIdCounter << "已连接:"
                << clientSocket->peerAddress().toString();

        //serverController->addClient(clientSocket, clientIdCounter++);
    }
}

void GameServer::readPlayerCount()
{
}

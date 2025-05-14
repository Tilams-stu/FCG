#include "gameserver.h"
#include <QApplication>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMessageBox>

const quint16 PORT = 12345;

GameServer::GameServer(QWidget *parentWidget, QObject *parent)
    : QObject(parent), m_parentWidget(parentWidget)
{
    tcpServer = new QTcpServer(this);
    serverController = new ServerController(this);
}

void GameServer::startServer()
{
    // 使用输入对话框获取玩家数量
    bool ok;
    int players = QInputDialog::getInt(m_parentWidget,
                                       tr("游戏设置"),
                                       tr("请输入玩家数量（1-4）:"),
                                       2,  // 默认值
                                       1,  // 最小值
                                       4,  // 最大值
                                       1,  // 步进
                                       &ok);

    if (!ok) {
        QMessageBox::information(m_parentWidget,
                                 tr("取消操作"),
                                 tr("用户取消了服务器启动"));
        return;
    }

    desiredPlayers = players;
    serverController->setDesiredPlayers(desiredPlayers);

    if (!tcpServer->listen(QHostAddress::Any, PORT)) {
        QMessageBox::critical(m_parentWidget,
                              tr("服务器错误"),
                              tr("无法启动服务器:\n%1").arg(tcpServer->errorString()));
        QCoreApplication::exit(EXIT_FAILURE);
        return;
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &GameServer::handleNewConnection);
    qInfo() << "服务器已在端口" << PORT << "启动，等待" << desiredPlayers << "位玩家连接...";
    QMessageBox::information(m_parentWidget,
                             tr("服务器已启动"),
                             tr("正在监听端口 %1\n等待%2位玩家连接...").arg(PORT).arg(desiredPlayers));
}

void GameServer::handleNewConnection()
{
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
        if (!clientSocket) {
            continue;
        }

        qInfo() << "GameServer: 客户端" << clientIdCounter << "尝试连接:"
                << clientSocket->peerAddress().toString() << "描述符:" << clientSocket->socketDescriptor();

        // Pass the QTcpSocket pointer directly
        serverController->addClient(clientSocket, clientIdCounter);
        clientIdCounter++;
    }
}



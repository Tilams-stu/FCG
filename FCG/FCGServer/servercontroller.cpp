#include "servercontroller.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QThread>
#include <QDebug>
#include <QEventLoop>

ServerController::ServerController(QObject *parent) : QObject(parent)
{
    executor.setMaxThreadCount(10);
    qRegisterMetaType<QByteArray>("QByteArray");
}

ServerController::~ServerController()
{
    QThreadPool::globalInstance()->waitForDone();
    qDeleteAll(clients);
}

// 客户端管理
void ServerController::addClient(qintptr socketDescriptor, int clientId)
{

}

void ServerController::removeClient(int clientId)
{

}

void ServerController::sendToClient(int clientId, const QByteArray &message)
{

}

// ClientHandler 实现





// 游戏逻辑处理
void ServerController::handleClientAction(int clientId, const QByteArray &data)
{

}

/*void ServerController::nextTurn()
{
    currentPlayerId = (currentPlayerId % desiredPlayers) + 1;
    broadcastMessage(QString("TURN:%1").arg(currentPlayerId));
}
*/

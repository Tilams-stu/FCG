#include "gamecontroller.h"

GameController::GameController(GameModel* model ,const QString& host,int port,QObject* parent)
    : QObject(parent),model(model),view(nullptr),socket(nullptr),isConnected(false)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &GameController::handleConnected);
    connect(socket, &QTcpSocket::readyRead, this, &GameController::handleReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &GameController::handleError);
    connect(socket, &QTcpSocket::disconnected, this, &GameController::handleDisconnected);

    socket->connectToHost(host,port);
    inStream.setDevice(socket);
}

void GameController::setView(MainView *view)
{
    this->view = view;
}

void GameController::sendReady()
{
    if(!isConnected) return;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << QString("READY");
    socket->write(data);
}

void GameController::sendPlaneOperation(int dice, int planeId)
{
    if(!isConnected) return;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << QString("PLANE_OP")<< dice <<planeId;
    socket->write(data);
}

void GameController::sendFlyOverChoice(bool isYes)
{
    if (!isConnected) return;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << QString("FLY_OVER") << isYes;
    socket->write(data);
}

void GameController::handleConnected()
{
    isConnected = true;
    inStream.setDevice(socket);
    emit connectionStatusChanged(true);
}

void GameController::handleReadyRead()
{
    while(socket->bytesAvailable() > 0){
        QByteArray message;
        inStream.startTransaction();
        inStream >> message;

        if(!inStream.commitTransaction()) return;

        processServerMessage(message);
    }
}

void GameController::handleError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    closeConnection();
    emit serverMessageReceived(tr("网络错误: %1").arg(socket->errorString()));
}

void GameController::handleDisconnected()
{
    closeConnection();
    emit serverMessageReceived(tr("连接已断开"));
}

void GameController::processServerMessage(const QByteArray &message)
{
    QDataStream stream(message);
    QString msgType;
    stream >> msgType;

    if(msgType == "GAME_STATE"){
        QMap<int, QList<int>> tileStates;
        stream >> tileStates;
        emit gameStateUpdated(tileStates);
    }
    else if(msgType == "TEXT_MSG"){
        QString content;
        stream >> content;
        handleTextMessage(content);
    }
}

void GameController::handleTextMessage(const QString &message)
{
    qDebug() << "Server message:" << message;

    if (message.startsWith("YOUR_TURN_ROLL_AND_CHOOSE_PLANE")) {
        QMetaObject::invokeMethod(this, [this](){
            emit serverMessageReceived(tr("轮到你了，请进行操作！"));
            emit updateGamePhase(ControlPanel::ROLL_AND_CHOOSE_PLANE,
                                 tr("现在轮到你了，请先投骰子，然后选择飞机！"));
        }, Qt::QueuedConnection);
    }
    else if (message.startsWith("YOUR_TURN_CHOOSE_FLY")) {
        QMetaObject::invokeMethod(this, [this](){
            emit serverMessageReceived(tr("请选择是否飞跃！"));
            emit updateGamePhase(ControlPanel::CHOOSE_FLY_OVER,
                                 tr("请选择是否飞跃！"));
        }, Qt::QueuedConnection);
    }
    else if (message.startsWith("等待")) {
        QMetaObject::invokeMethod(this, [this, message](){
            emit updateGamePhase(ControlPanel::WAITING, message);
        }, Qt::QueuedConnection);
    }
    else if (message.startsWith("ERROR")) {
        emit serverMessageReceived(tr("服务器错误: %1").arg(message));
    }
    else if (message.startsWith("WELCOME")) {
        emit serverMessageReceived(message);
    }
    else if (message.startsWith("玩家")) {
        emit serverMessageReceived(message);
    }
}

void GameController::closeConnection()
{
    isConnected = false;
    if(socket->state() != QAbstractSocket::UnconnectedState){
        socket->disconnectFromHost();
    }
    emit connectionStatusChanged(false);
}





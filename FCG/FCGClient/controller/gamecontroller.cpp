#include "gamecontroller.h"
#include <QTimer>
#include <QDebug>
#include "../model/gamestate.h"

GameController::GameController(GameModel* gameModel, const QString& h, int p, QObject* parent)
    : QObject(parent), model(gameModel), view(nullptr),
    host(h), port(p), isConnected(false), expectedBytes(0), connectTimer(nullptr)
{
    socket = new QTcpSocket(this);

    inStream.setDevice(socket);
    inStream.setVersion(QDataStream::Qt_6_5);

    connect(socket, &QTcpSocket::connected, this, &GameController::handleConnected);
    connect(socket, &QTcpSocket::readyRead, this, &GameController::handleReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &GameController::handleError);
    connect(socket, &QTcpSocket::disconnected, this, &GameController::handleDisconnected);

    qRegisterMetaType<QMap<int, QList<int>>>("QMap<int,QList<int>>");
    qRegisterMetaType<GameState>("GameState");
    //qRegisterMetaType<ControlPanel::GamePhase>("ControlPanel::GamePhase");
    qDebug() << "GameController created. Attempting to connect to" << host << ":" << port;
}

GameController::~GameController()
{
    closeConnection();
}

void GameController::setView(MainView *v)
{
    this->view = v;
}

void GameController::connectToServer()
{
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        qWarning() << "GameController::connectToServer: Socket not in unconnected state, current state:" << socket->state();
        if (isConnected) {
            qInfo() << "Already connected.";
            emit serverMessageReceived(tr("已连接到服务器"));
            return;
        }
        if (socket->state() == QAbstractSocket::ConnectingState) {
            qInfo() << "Connection attempt already in progress.";
            return;
        }
        socket->abort();
    }

    qDebug() << "GameController: Connecting to server:" << host << ":" << port;
    expectedBytes = 0;
    socket->connectToHost(host, port);

    if (!connectTimer) {
        connectTimer = new QTimer(this);
        connectTimer->setSingleShot(true);
        connect(connectTimer, &QTimer::timeout, this, [this]() {
            if (!isConnected && socket->state() == QAbstractSocket::ConnectingState) {
                socket->abort();
                qCritical() << "GameController: Connection timeout to" << host << ":" << port;
                emit serverMessageReceived(tr("连接服务器超时"));
                emit connectionStatusChanged(false);

                if (view && view->getControlPanel()) {
                    emit updateGamePhase(ControlPanel::GamePhase::WAITING, tr("连接超时，请重试"));
                }
            }
        });
    }
    connectTimer->start(5000);
}

void GameController::sendTypedMessage(const QString& messageType, const QVariant& payload1, const QVariant& payload2) {
    if (!isConnected || !socket || socket->state() != QTcpSocket::ConnectedState) {
        qWarning() << "GameController: Cannot send [" << messageType << "]. Not connected.";
        emit serverMessageReceived(tr("未连接到服务器，无法发送消息。"));
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    out << quint32(0);
    out << messageType;

    if (payload1.isValid()) out << payload1;
    if (payload2.isValid()) out << payload2;

    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));

    qint64 bytesWritten = socket->write(block);
    if (bytesWritten == -1) {
        qWarning() << "GameController: Failed to write to socket for message [" << messageType << "]. Error:" << socket->errorString();
    } else if (bytesWritten < block.size()) {
        qWarning() << "GameController: Not all bytes written for message [" << messageType << "]. Wrote" << bytesWritten << "of" << block.size();
        // Handle partial write, though for TCP this is less common unless buffer issues
    } else {
        socket->flush(); // Ensure data is sent immediately
        qDebug() << "Client sent [" << messageType << "] size:" << block.size();
    }

}


void GameController::sendReady()
{
    qDebug() << "Client sending READY_MSG. Connection status:" << isConnected;
    sendTypedMessage("READY_MSG");
}

void GameController::sendPlaneOperation(int dice, int planeId)
{
    qDebug() << "Client sending PLANE_OP_MSG with dice:" << dice << "plane:" << planeId;
    sendTypedMessage("PLANE_OP_MSG", QVariant(dice), QVariant(planeId));
}

void GameController::sendFlyOverChoice(bool isYes)
{
    qDebug() << "Client sending FLY_OVER_MSG with choice:" << isYes;
    sendTypedMessage("FLY_OVER_MSG", QVariant(isYes));
}


void GameController::handleConnected()
{
    if(connectTimer && connectTimer->isActive()) {
        connectTimer->stop();
    }

    qInfo() << "GameController: Successfully connected to server:" << host << ":" << port;
    isConnected = true;
    emit connectionStatusChanged(true);
    emit serverMessageReceived(tr("已连接到服务器."));
    if (view && view->getControlPanel()) {
        emit updateGamePhase(ControlPanel::GamePhase::WAITING, tr("已连接，等待其他玩家准备..."));
    }
}

void GameController::handleReadyRead()
{
    qDebug() << "Client: handleReadyRead() triggered. Bytes available:" << socket->bytesAvailable();
    inStream.setVersion(QDataStream::Qt_6_5);

    forever {
        qDebug() << "Client: Top of forever loop. expectedBytes:" << expectedBytes << "Bytes available:" << socket->bytesAvailable() << "Stream status:" << inStream.status();
        if (expectedBytes == 0) {
            if (socket->bytesAvailable() < sizeof(quint32)) {
                qDebug() << "Client: Not enough data for size yet. Have" << socket->bytesAvailable() << "need" << sizeof(quint32) << ". Returning.";
                return;
            }
            inStream >> expectedBytes;
            qDebug() << "Client: Expecting" << expectedBytes << "bytes from server.";
        }

        if (socket->bytesAvailable() < expectedBytes) {
            qDebug() << "Client: Not enough data yet. Have" << socket->bytesAvailable() << "need" << expectedBytes;
            return;
        }

        QString messageType;
        inStream >> messageType;
        if (inStream.status() != QDataStream::Ok) {
            qWarning() << "Client: QDataStream error while reading messageType.";
            socket->abort(); expectedBytes = 0; return;
        }
        qDebug() << "Client: Received message type:" << messageType;

        if (messageType == "GAME_STATE_MSG") {
            QVariant gameStatePayload;
            inStream >> gameStatePayload;
            if (inStream.status() != QDataStream::Ok || !gameStatePayload.canConvert<GameState>()) {
                qWarning() << "Client: QDataStream error or type mismatch reading GAME_STATE_MSG payload.";
                socket->abort(); expectedBytes = 0; return;
            }
            GameState receivedState = gameStatePayload.value<GameState>();
            qDebug() << "Client: Received GAME_STATE_MSG, map size:" << receivedState.getTileStates().size();
            if (model) model->setBoardState(receivedState.getTileStates());
            emit gameStateUpdated(receivedState.getTileStates());
        } else if (messageType == "TEXT_MSG") {
            QVariant textPayload;
            inStream >> textPayload;
            if (inStream.status() != QDataStream::Ok) {
                qWarning() << "Client: QDataStream error while reading TEXT_MSG QVariant payload.";
                socket->abort(); expectedBytes = 0; return;
            }
            QString content = textPayload.toString();
            qDebug() << "Client: Received TEXT_MSG content:" << content;

            ControlPanel::GamePhase phase = ControlPanel::GamePhase::WAITING;
            QString uiMessage = content;

            if (content.startsWith("YOUR_TURN_ROLL_AND_CHOOSE_PLANE")) {
                uiMessage = tr("轮到你了, 请投骰子并选择飞机!");
                phase = ControlPanel::GamePhase::ROLL_AND_CHOOSE_PLANE;
            } else if (content.startsWith("YOUR_TURN_CHOOSE_FLY")) {
                uiMessage = tr("请选择是否飞跃!");
                phase = ControlPanel::GamePhase::CHOOSE_FLY_OVER;
            } else if (content.startsWith("ERROR:")) {
                uiMessage = tr("服务器错误: %1").arg(content.mid(6));
            } else if (content.startsWith("WELCOME:")) {
                phase = ControlPanel::GamePhase::WAITING;
            } else {
                if (content.contains("等待") || content.contains("已准备") || content.contains("加入了游戏") || content.contains("游戏开始")) {
                    phase = ControlPanel::GamePhase::WAITING;
                }
                if (content.contains("游戏开始!")) { }
            }
            emit serverMessageReceived(uiMessage);
            if (view && view->getControlPanel()){
                emit updateGamePhase(phase, uiMessage);
            }
        } else {
            qWarning() << "Client: Received unknown message type from server:" << messageType;

            QByteArray dummy = socket->read(expectedBytes); // Attempt to read out the rest of the expected block
            qDebug() << "Client: Discarded" << dummy.size() << "bytes for unknown message type.";
        }
        expectedBytes = 0;

        if(inStream.status() != QDataStream::Ok && socket->state() == QAbstractSocket::ConnectedState){
            qWarning() << "Client: QDataStream not OK after processing message. Aborting.";
            socket->abort();
            return;
        }

        if (socket->bytesAvailable() == 0) break;
    }
}


void GameController::handleError(QAbstractSocket::SocketError socketError)
{
    if(connectTimer && connectTimer->isActive()) {
        connectTimer->stop();
    }

    QString errorMsg;
    if (socketError == QAbstractSocket::RemoteHostClosedError) {
        errorMsg = tr("连接被服务器关闭.");
    } else if (socketError == QAbstractSocket::ConnectionRefusedError) {
        errorMsg = tr("连接被服务器拒绝. 请确认服务器正在运行并且地址正确.");
    } else if (socketError == QAbstractSocket::HostNotFoundError) {
        errorMsg = tr("找不到服务器主机. 请检查主机名或IP地址.");
    } else {
        errorMsg = socket->errorString();
    }

    qCritical() << "GameController: Network error occurred:" << socketError << errorMsg;
    emit serverMessageReceived(tr("网络错误: %1").arg(errorMsg));
    if (isConnected) {
        isConnected = false;
        emit connectionStatusChanged(false);
    }
    if (view && view->getControlPanel()) {
        emit updateGamePhase(ControlPanel::GamePhase::WAITING, tr("网络错误: %1").arg(errorMsg));
    }
}

void GameController::handleDisconnected()
{
    qInfo() << "GameController: Disconnected from server.";
    if(connectTimer && connectTimer->isActive()) {
        connectTimer->stop();
    }
    bool wasConnected = isConnected;
    isConnected = false;
    expectedBytes = 0;

    if (wasConnected) {
        emit serverMessageReceived(tr("已从服务器断开连接."));
        emit connectionStatusChanged(false);
        if (view && view->getControlPanel()) {
            emit updateGamePhase(ControlPanel::GamePhase::WAITING, tr("已断开连接. 请尝试重新连接."));
        }
    } else {
        if (socket->error() == QAbstractSocket::RemoteHostClosedError) {
            qDebug() << "GameController: Disconnected, but wasNotConnected or error already handled. Socket error:" << socket->errorString();
            emit serverMessageReceived(tr("连接被服务器关闭."));
        } else {
            emit serverMessageReceived(tr("连接已关闭."));
        }
        if (view && view->getControlPanel()) { // Safely access controlPanel
            emit updateGamePhase(ControlPanel::GamePhase::WAITING, tr("连接已关闭."));
        }
    }
}

void GameController::closeConnection()
{
    qDebug() << "GameController: closeConnection() called. Current state:" << (socket ? socket->state() : -1) << "isConnected:" << isConnected;
    if (socket && socket->state() != QAbstractSocket::UnconnectedState) {
        socket->abort();
    }

    bool oldStatus = isConnected;
    isConnected = false;
    expectedBytes = 0;

    if (oldStatus) {
        emit connectionStatusChanged(false);
    }
    qDebug() << "GameController: Connection resources cleaned up.";
}

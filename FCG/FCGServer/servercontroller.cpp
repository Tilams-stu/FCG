#include "servercontroller.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QThread>
#include <QDebug>
#include <QVariant>
#include <QThreadPool>

ServerController::ServerController(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QMap<int, QList<int>>>("QMap<int,QList<int>>");
    qRegisterMetaType<GameState>("GameState");

    qDebug() << "ServerController created in thread" << QThread::currentThreadId();
}

ServerController::~ServerController()
{
    qDebug() << "ServerController shutting down...";
    fflush(stdout);
    // Ensure all client handlers are deleted before clients map is cleared.
    // qDeleteAll will call destructors.
    qDeleteAll(clients.values()); // Pass the values (ClientHandler*) to qDeleteAll
    clients.clear();
}

void ServerController::setDesiredPlayers(int desiredPlayers)
{
    QMutexLocker lock(&gameLogicMutex);
    this->desiredPlayers = desiredPlayers;
    qDebug() << "ServerController::setDesiredPlayers - Attempting to init model for" << desiredPlayers << "players.";
    fflush(stdout);
    model.initGame(desiredPlayers);
    qInfo() << "Server desired players set to:" << desiredPlayers;
    fflush(stdout);
}

// 客户端管理
void ServerController::addClient(QTcpSocket* clientSocket, int clientId)
{
    qDebug() << "ServerController::addClient for client" << clientId << "in thread" << QThread::currentThreadId();
    fflush(stdout);

    ClientHandler* handler = nullptr;
    QString newClientColor;
    int currentClientCount = 0;
    int currentDesiredPlayers = 0; // Local copy to use outside lock

    { // Scope for QMutexLocker to ensure it's released before calling other locking functions
        QMutexLocker locker(&clientsMutex);
        currentDesiredPlayers = this->desiredPlayers; // Copy within lock

        if(currentDesiredPlayers > 0 && clients.size() >= currentDesiredPlayers){
            qWarning() << "Server is full. Rejecting new client connection for ID" << clientId;
            fflush(stdout);
            // Locker unlocks automatically
            clientSocket->disconnectFromHost();
            clientSocket->deleteLater();
            return;
        }

        handler = new ClientHandler(clientSocket, clientId, this);
        handler->setParent(this);

        connect(handler, &ClientHandler::parsedMessage, this, &ServerController::handleClientAction);
        connect(handler, &ClientHandler::clientDisconnected, this, &ServerController::removeClientSlot);

        clients.insert(clientId, handler);
        newClientColor = getPlayerColor(clientId);
        playerColors[clientId] = newClientColor;
        currentClientCount = clients.size();

        qDebug() << "ServerController::addClient - Client" << clientId << "added to map. Map size:" << currentClientCount;
        fflush(stdout);
        // Mutex is released here when 'locker' goes out of scope
    }

    // Operations after releasing clientsMutex
    if (handler) {
        qInfo() << "Client" << clientId << "(" << newClientColor << ") connected. Total clients:" << currentClientCount;
        fflush(stdout);

        // broadcastMessage will acquire its own lock on clientsMutex
        broadcastMessage(QString("玩家 %1 (%2) 加入了游戏. (%3/%4)")
                             .arg(clientId).arg(newClientColor).arg(currentClientCount).arg(currentDesiredPlayers));

        // handler->sendMessage does not lock clientsMutex itself
        handler->sendMessage(QString("WELCOME:Connected as player %1 (%2). Waiting for game to start...")
                                 .arg(clientId).arg(newClientColor));
    } else {
        qWarning() << "ServerController::addClient - Handler was not created for client" << clientId << "(should have been rejected if server full)";
        fflush(stdout);
    }
}

void ServerController::removeClientSlot(int clientId)
{
    qDebug() << "ServerController::removeClientSlot - Attempting to remove client" << clientId;
    fflush(stdout);
    QMutexLocker locker(&clientsMutex);
    qDebug() << "ServerController::removeClientSlot - Mutex acquired for client" << clientId;
    fflush(stdout);

    if (clients.contains(clientId)) {
        ClientHandler* handler = clients.take(clientId);
        qDebug() << "ServerController::removeClientSlot - Client" << clientId << "taken from map. Handler ptr:" << handler;
        fflush(stdout);

        if (handler) {
            // Disconnect signals to prevent further interaction with a dying object
            disconnect(handler, &ClientHandler::parsedMessage, this, &ServerController::handleClientAction);
            disconnect(handler, &ClientHandler::clientDisconnected, this, &ServerController::removeClientSlot);
            delete handler; // Deletes the handler
            qDebug() << "ServerController::removeClientSlot - ClientHandler for" << clientId << "deleted.";
            fflush(stdout);
        } else {
            qWarning() << "ServerController::removeClientSlot - Handler for client" << clientId << "was null in map!";
            fflush(stdout);
        }

        QString color = playerColors.take(clientId);
        qInfo() << "Client" << clientId << "(" << color << ") disconnected and removed. Total clients:" << clients.size();

        gameLogicMutex.lock();
        if (playerReadyStatus.remove(clientId)) {
            readyPlayers--;
        }

        if (clients.isEmpty() && (readyPlayers > 0 || currentPlayerId != 0)) {
            qInfo() << "Last player disconnected. Resetting game.";
            readyPlayers = 0;
            currentPlayerId = 0;
            playerReadyStatus.clear();
            model.initGame(desiredPlayers); // Or some other reset logic
        } else if (!clients.isEmpty() && currentPlayerId != 0) { // Game in progress
            broadcastMessage(QString("玩家 %1 (%2) 离开了游戏.").arg(clientId).arg(color));
            if (clientId == currentPlayerId) {
                gameLogicMutex.unlock();
                nextTurn();
                return;
            }
        } else if (!clients.isEmpty() && readyPlayers < desiredPlayers) { // In ready phase
            broadcastMessage(QString("玩家 %1 (%2) 离开了. 等待 %3 位玩家.")
                                 .arg(clientId)
                                 .arg(color)
                                 .arg(desiredPlayers - clients.size()));
        }
        gameLogicMutex.unlock();
    }
    else {
        qWarning() << "ServerController::removeClientSlot - Client" << clientId << "not found in map.";
        fflush(stdout);
    }
    qDebug() << "ServerController::removeClientSlot - Exiting for client" << clientId;
    fflush(stdout);
}

void ServerController::sendToClient(int clientId, const QString &messageType, const QVariant &payload1, const QVariant &payload2)
{
    //QMutexLocker locker(&clientsMutex);
    if (clients.contains(clientId)) {
        ClientHandler* handler = clients.value(clientId);
        if(handler) {
            qDebug() << "ServerController: Sending message" << messageType << "to client" << clientId;
            handler->sendTypedMessage(messageType, payload1, payload2);
        } else {
            qWarning() << "ServerController: Attempted to send message to null handler for client" << clientId;
        }
    } else {
        qWarning() << "ServerController: Client" << clientId << "not found for sending message" << messageType;
    }
}

QString ServerController::getPlayerColor(int clientId)
{
    switch(clientId) {
    case 1: return "黄";
    case 2: return "蓝";
    case 3: return "绿";
    case 4: return "红";
    default: return "未知";
    }
}

// ClientHandler 实现
ClientHandler::ClientHandler(QTcpSocket* clientSock, int cId, ServerController *ctrl, QObject* parent)
    : QObject(parent),
    clientId(cId),
    controller(ctrl),
    socket(clientSock),
    inStream(nullptr),
    expectedBytes(0)
{
    qDebug() << "ClientHandler for client" << clientId << "created in thread" << QThread::currentThreadId();
    if (socket) {
        socket->setParent(this);
        inStream = new QDataStream(socket);
        inStream->setVersion(QDataStream::Qt_6_5);

        connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::readData);
        connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::handleDisconnected);
    } else {
        qCritical() << "ClientHandler for client" << clientId << "received a null socket!";
    }
}
ClientHandler::~ClientHandler()
{
    qDebug() << "ClientHandler for client" << clientId << "destroying...";
    fflush(stdout);
    // Socket is parented, will be deleted.
    // QDataStream should be deleted if new'd.
    delete inStream; // Delete the QDataStream object
    inStream = nullptr;
    qDebug() << "ClientHandler for client" << clientId << "destroyed.";
    fflush(stdout);
}

void ClientHandler::sendTypedMessage(const QString &messageType, const QVariant &payload1, const QVariant &payload2)
{
    if (!socket || socket->state() != QTcpSocket::ConnectedState) {
        qWarning() << "Client" << clientId << ": Socket not connected. Cannot send" << messageType;
        return;
    }

    qDebug() << "ClientHandler" << clientId << ": Preparing to send" << messageType << ". Payload1 valid:" << payload1.isValid() << "UserType:" << payload1.userType() << payload1.typeName() << "Payload2 valid:" << payload2.isValid();
    fflush(stdout);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);


    if (messageType == "GAME_STATE_MSG" && payload1.isValid()) {
        qDebug() << "ClientHandler" << clientId << ": GAME_STATE_MSG payload typeName:" << payload1.typeName() << "userType:" << payload1.userType();
        if (!payload1.canConvert<GameState>()) {
            qWarning() << "ClientHandler" << clientId << ": FATAL - GAME_STATE_MSG payload QVariant CANNOT be converted to GameState! Check Q_DECLARE_METATYPE and qRegisterMetaType.";
            return; // Don't attempt to send if conversion is impossible
        } else {
            // Test actual conversion and access
            GameState tempStateForTest = payload1.value<GameState>();
            qDebug() << "ClientHandler" << clientId << ": GAME_STATE_MSG payload QVariant successfully TEST-converted to GameState. Tile count:" << tempStateForTest.getTileStates().size();
        }
    }

    out << quint32(0);
    out << messageType;
    if (out.status() != QDataStream::Ok) {
        qWarning() << "ClientHandler" << clientId << ": QDataStream error after writing messageType for" << messageType << "Status:" << out.status();
        socket->reset(); return;
    }


    if (payload1.isValid()) {
        qDebug() << "ClientHandler" << clientId << ": Attempting to write payload1 for" << messageType;
        out << payload1; // Serialize QVariant
        if (out.status() != QDataStream::Ok) {
            qWarning() << "ClientHandler" << clientId << ": QDataStream error after writing payload1 for" << messageType << "Status:" << out.status();
            socket->reset();
            return;
        }
        qDebug() << "ClientHandler" << clientId << ": Successfully wrote payload1 for" << messageType;
    }

    if (payload2.isValid()) {
        qDebug() << "ClientHandler" << clientId << ": Attempting to write payload2 for" << messageType;
        out << payload2;
        if (out.status() != QDataStream::Ok) {
            qWarning() << "ClientHandler" << clientId << ": QDataStream error after writing payload2 for" << messageType << "Status:" << out.status();
            socket->reset();
            return;
        }
        qDebug() << "ClientHandler" << clientId << ": Successfully wrote payload2 for" << messageType;
    }

    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32)); // Write actual size

    qDebug() << "ClientHandler" << clientId << ": Final block size for" << messageType << "is" << block.size();
    fflush(stdout);

    qDebug() << "ClientHandler" << clientId << ": Attempting socket->write() for" << messageType << "block size:" << block.size();
    fflush(stdout);

    qint64 written = socket->write(block);
    if (written == -1) {
        qWarning() << "ClientHandler" << clientId << "socket->write() failed for" << messageType << ". Error:" << socket->errorString();
        fflush(stdout);
    } else if (written < block.size()) {
        qWarning() << "ClientHandler" << clientId << "failed to write complete message for" << messageType << ". Wrote" << written << "of" << block.size() << "Error:" << socket->errorString();
        fflush(stdout);
    } else {
        bool flushed = socket->flush();
        qDebug() << "ClientHandler: Server sent [" << messageType << "] to client" << clientId << "size:" << block.size() << "(flushed:" << flushed << ")";
        fflush(stdout);
    }
}

void ClientHandler::sendMessage(const QString &message)
{
    sendTypedMessage("TEXT_MSG", QVariant(message));
}

void ClientHandler::sendGameState(const GameState& gameState)
{
    qCritical() << "ClientHandler" << getClientId() << ": sendGameState - ENTERED. GameState tile count:" << gameState.getTileStates().size();
    fflush(stdout);

    QVariant gameStateVariant;
    qCritical() << "ClientHandler" << getClientId() << ": sendGameState - Attempting QVariant::fromValue(gameState)...";
    fflush(stdout);

    gameStateVariant = QVariant::fromValue(gameState);

    if (!gameStateVariant.isValid()) {
        qWarning() << "ClientHandler" << getClientId() << ": sendGameState - FAILED to create a valid QVariant from GameState! TypeName after attempt:" << gameStateVariant.typeName();
        fflush(stdout);
        return;
    }

    qCritical() << "ClientHandler" << getClientId() << ": sendGameState - QVariant CREATED successfully. TypeName:" << gameStateVariant.typeName() << "UserType:" << gameStateVariant.userType();
    fflush(stdout);

    sendTypedMessage("GAME_STATE_MSG", gameStateVariant);

    qCritical() << "ClientHandler" << getClientId() << ": sendGameState - EXITED (called sendTypedMessage).";
    fflush(stdout);
}

int ClientHandler::getClientId()
{
    return clientId;
}

void ClientHandler::readData()
{
    if (!socket || !inStream) return;
    inStream->setVersion(QDataStream::Qt_6_5);

    forever{
        if (expectedBytes == 0) {
            if (socket->bytesAvailable() < sizeof(quint32)) {
                return;
            }
            *inStream >> expectedBytes;
            //qDebug() << "Server: Client" << clientId << "expecting" << expectedBytes << "bytes for next message.";
        }
        if (socket->bytesAvailable() < expectedBytes) {
            //qDebug() << "Server: Client" << clientId << "not enough data yet. Have" << socket->bytesAvailable() << "need" << expectedBytes;
            return;
        }

        QString messageType;
        *inStream >> messageType;

        if (inStream->status() != QDataStream::Ok) {
            qWarning() << "Server: Client" << clientId << "stream error reading messageType.";
            socket->abort();
            expectedBytes = 0;
            return;
        }
        //qDebug() << "Server: Client" << clientId << "received message type:" << messageType;

        QVariant payload1, payload2;

        if (messageType == "READY_MSG") {
            // No payload
        }
        else if (messageType == "PLANE_OP_MSG") {
            *inStream >> payload1 >> payload2; // Read as QVariants
            if (inStream->status() != QDataStream::Ok) {
                qWarning() << "Server: Client" << clientId << "stream error reading PLANE_OP_MSG payload.";
                socket->abort();
                expectedBytes = 0;
                return;
            }
        }
        else if (messageType == "FLY_OVER_MSG") {
            *inStream >> payload1;
            if (inStream->status() != QDataStream::Ok) {
                qWarning() << "Server: Client" << clientId << "stream error reading FLY_OVER_MSG payload.";
                socket->abort();
                expectedBytes = 0;
                return;
            }
        }
        else {
            qWarning() << "Server: Client" << clientId << "received unknown message type:" << messageType;
            QByteArray dummyLoad;
            dummyLoad.resize(expectedBytes - (sizeof(quint32) + messageType.toUtf8().size())); // Approximate
            if (socket->bytesAvailable() >= dummyLoad.size()) {
                inStream->readRawData(dummyLoad.data(), dummyLoad.size());
                qDebug() << "Server: Discarded approx" << dummyLoad.size() << "bytes of unknown payload from client" << clientId;
            } else {
                qWarning() << "Server: Not enough data to fully discard unknown payload from client" << clientId << ". Stream may be corrupted.";
                socket->abort();
            }
            expectedBytes = 0;
            return;
        }
        emit parsedMessage(clientId, messageType, payload1, payload2);
        expectedBytes = 0;

        if (inStream->status() != QDataStream::Ok && socket->state() == QAbstractSocket::ConnectedState) {
            qWarning() << "Server: Client" << clientId << "QDataStream status not OK after processing message. Aborting.";
            socket->abort();
            return;
        }
        if (socket->bytesAvailable() == 0) break;
    }

}

void ClientHandler::handleDisconnected()
{
    qInfo() << "Client" << clientId << "socket disconnected signal received by ClientHandler.";
    emit clientDisconnected(clientId);
}

QString ClientHandler::getPlayerColor(int cId)
{
    switch(cId) {
    case 1: return "黄";
    case 2: return "蓝";
    case 3: return "绿";
    case 4: return "红";
    default: return "未知";
    }
}

// 游戏逻辑处理
void ServerController::handleClientAction(int clientId, const QString &messageType, const QVariant &payload1, const QVariant &payload2)
{
    QMutexLocker locker(&gameLogicMutex); // Protect game logic and state
    qDebug() << "Server received action from client" << clientId << "(" << getPlayerColor(clientId) << "):" << messageType;
    fflush(stdout);

    if (messageType == "READY_MSG") {
        if (!playerReadyStatus.value(clientId, false)) {
            playerReadyStatus[clientId] = true;
            readyPlayers++;
            QString msg = QString("玩家 %1 (%2) 已准备. (%3/%4)")
                              .arg(clientId).arg(getPlayerColor(clientId))
                              .arg(readyPlayers).arg(desiredPlayers);
            broadcastMessage(msg);
            qInfo() << msg;

            if (readyPlayers == desiredPlayers && desiredPlayers > 0) { // Check clients.size() as well?
                QMutexLocker clientListLocker(&clientsMutex);
                if (clients.size() == desiredPlayers) {
                    initGameAndStart();
                } else {
                    broadcastMessage(QString("所有已连接玩家已准备，但等待 %1 位玩家加入...")
                                         .arg(desiredPlayers - clients.size()));
                }
            } else if (clients.size() < desiredPlayers) {
                QMutexLocker clientListLocker(&clientsMutex); // Accessing clients.size()
                broadcastMessage(QString("等待其他 %1 位玩家加入...").arg(desiredPlayers - clients.size()));
            }
        } else {
            qDebug() << "Player" << clientId << "sent READY_MSG again.";
        }
        return;
    }

    if (readyPlayers < desiredPlayers || currentPlayerId == 0) {
        sendToClient(clientId, "TEXT_MSG", "ERROR:游戏尚未开始或未集齐玩家.");
        return;
    }
    if (clientId != currentPlayerId) {
        sendToClient(clientId, "TEXT_MSG", "ERROR:不是你的回合!");
        return;
    }

    try {
        if (messageType == "PLANE_OP_MSG") {
            bool diceOk, planeIdOk;
            int dice = payload1.toInt(&diceOk);
            int planeId = payload2.toInt(&planeIdOk);

            if (!diceOk || !planeIdOk) {
                qWarning() << "Server: Invalid payload for PLANE_OP_MSG from client" << clientId;
                sendToClient(clientId, "TEXT_MSG", QVariant("ERROR:无效的飞机操作参数."));
                return;
            }
            qInfo() << "玩家" << getPlayerColor(clientId) << "选择了飞机" << planeId << "，骰子点数" << dice;

            GameState state(model.getBoardState());
            int result = do_plan_OP(clientId,dice,planeId,state);
            lastDice = dice;
            lastPlaneId = planeId;

            broadcastGameState(state);
            if(result == 1){
                sendToClient(clientId, "TEXT_MSG", QVariant("YOUR_TURN_CHOOSE_FLY"));
            }
            else{
                check_is_win(state);
                nextTurn();
            }
        }
        else if (messageType == "FLY_OVER_MSG") {
            bool flyYes = payload1.toBool();
            QString choiceStr = flyYes ? "YES" : "NO";
            qInfo() << "玩家" <<clientId << "选择飞跃？"<< choiceStr;

            GameState state(model.getBoardState());
            do_fly(lastPlaneId, clientId, choiceStr, state);
            model.setBoardState(state.getTileStates());

            broadcastGameState(state);
            check_is_win(state);
            nextTurn();
        }
        else {
            qWarning() << "Client" << clientId << "sent unknown or unhandled message type:" << messageType;
            sendToClient(clientId, "TEXT_MSG", QVariant(QString("ERROR:未知操作! %1").arg(messageType)));
        }
    } catch (const std::exception& e) {
        qCritical() << "Exception during client action:" << e.what();
        sendToClient(clientId, "TEXT_MSG", QVariant(QString("ERROR:处理操作时发生错误: %1").arg(e.what())));
    } catch (...) {
        qCritical() << "Unknown exception during client action for client" << clientId;
        sendToClient(clientId, "TEXT_MSG", QVariant("ERROR:处理操作时发生未知错误."));
    }
}

void ServerController::broadcastMessage(const QString &msg)
{
    //QMutexLocker locker(&clientsMutex);
    qDebug() << "Server broadcasting TEXT_MSG:" << msg;
    for(ClientHandler* handler : qAsConst(clients)){
        if (handler) {
            handler->sendMessage(msg);
        }
    }
}

void ServerController::broadcastGameState(const GameState& state)
{
    qCritical() << "[BGS_ENTER] broadcastGameState - ENTERED.";
    fflush(stdout);

    qCritical() << "[BGS_TEST_THIS] 'this->desiredPlayers' (pre-access):" << this->desiredPlayers;
    fflush(stdout);

    qCritical() << "[BGS_TEST_STATE_PARAM] 'state.getTileStates().size()' (pre-access):" << state.getTileStates().size();
    fflush(stdout);

    qCritical() << "[BGS_TEST_CLIENTS_SIZE] 'clients.size()' (pre-access):" << this->clients.size();
    fflush(stdout);

    qCritical() << "[BGS_BEFORE_LOCKER_ATTEMPT] All initial accesses OK. About to TRY to lock clientsMutex.";
    fflush(stdout);

    try {
        qCritical() << "[BGS_TRY_LOCK] Attempting QMutexLocker locker(&clientsMutex)...";
        fflush(stdout);
        //QMutexLocker locker(&clientsMutex); // This is the critical point
        qCritical() << "[BGS_LOCKER_ACQUIRED] clientsMutex Acquired by QMutexLocker. Client map size (again):" << this->clients.size();
        fflush(stdout);

        if (clients.isEmpty()) {
            qWarning() << "[BGS_WARN] No clients to broadcast GameState to!"; fflush(stdout);
            // Locker releases when function returns
            return;
        }

        qDebug() << "[BGS_INFO] Broadcasting GameState object. Actual state tile count:" << state.getTileStates().size(); fflush(stdout);
        int handler_count = 0;
        for (ClientHandler* handler : clients.values()) {
            handler_count++;
            if (handler) {
                qCritical() << "[BGS_LOOP] Loop" << handler_count << ": PREP to call sendGameState for client" << handler->getClientId(); fflush(stdout);
                handler->sendGameState(state);
                qCritical() << "[BGS_LOOP] Loop" << handler_count << ": RET from sendGameState for client" << handler->getClientId(); fflush(stdout);
            } else {
                qWarning() << "[BGS_WARN_LOOP] Loop" << handler_count << ": Found null handler."; fflush(stdout);
            }
        }

        qCritical() << "[BGS_LOOP_FINISHED] Loop finished. QMutexLocker will release."; fflush(stdout);

    } catch (const std::exception& e) {
        qCritical() << "[BGS_EXCEPTION_LOCK_OR_LOOP] std::exception caught: " << e.what();
        fflush(stdout);
    } catch (...) {
        qCritical() << "[BGS_EXCEPTION_LOCK_OR_LOOP] Unknown exception caught around mutex lock or loop.";
        fflush(stdout);
    }

    qCritical() << "[BGS_EXIT] broadcastGameState EXITED.";
    fflush(stdout);
}

void ServerController::initGameAndStart()
{
    qInfo() << "所有玩家已准备，初始化游戏..."; // This is the last log seen from server

    qDebug() << "[Debug] initGameAndStart: Step 1 - Initializing model.initGame(" << desiredPlayers << ")";
    model.initGame(desiredPlayers);
    qDebug() << "[Debug] initGameAndStart: Step 2 - Model initialized.";

    currentPlayerId = 1; // Start with player 1
    qDebug() << "[Debug] initGameAndStart: Step 3 - CurrentPlayerId set to" << currentPlayerId;

    qDebug() << "[Debug] initGameAndStart: Step 4 - Attempting to get board state from model.";
    GameState initialState(model.getBoardState());
    qDebug() << "[Debug] initGameAndStart: Step 5 - Initial GameState created. Tile count (example):" << initialState.getTileStates().size();

    qDebug() << "[Debug] initGameAndStart: Step 6 - Broadcasting initial game state.";
    broadcastGameState(initialState);
    qDebug() << "[Debug] initGameAndStart: Step 7 - Initial game state broadcasted.";

    QString gameStartMsg = QString("游戏开始! 轮到玩家 %1 (%2).")
                               .arg(currentPlayerId).arg(getPlayerColor(currentPlayerId));
    qDebug() << "[Debug] initGameAndStart: Step 8 - Broadcasting game start message:" << gameStartMsg;
    broadcastMessage(gameStartMsg);
    qDebug() << "[Debug] initGameAndStart: Step 9 - Game start message broadcasted.";

    QString turnMsg = "YOUR_TURN_ROLL_AND_CHOOSE_PLANE";
    qDebug() << "[Debug] initGameAndStart: Step 10 - Sending turn message to player" << currentPlayerId << ":" << turnMsg;
    sendToClient(currentPlayerId, "TEXT_MSG", QVariant(turnMsg));
    qDebug() << "[Debug] initGameAndStart: Step 11 - Turn message sent. initGameAndStart complete.";
}

void ServerController::do_fly(int lastPlaneId, int currentPlayerId, const QString &choice, GameState& state)
{
    qDebug() << "[Debug] do_fly called for plane" << lastPlaneId << "client" << currentPlayerId << "choice" << choice;

    if (choice.toUpper() != "YES") {
        qDebug() << "Player" << currentPlayerId << "chose not to fly.";
        // model.setBoardState(state.getTileStates()); // Ensure model is up-to-date if state was modified before this call
        return;
    }

    //计算全局飞机编号
    int globalPlaneId = (currentPlayerId - 1) * 4 + lastPlaneId;

    //获取棋盘映射
    QMap<int , QList<int>> tileStates = state.getTileStates();
    if(tileStates.isEmpty()){
        qWarning() << "tileStates 为空，无法执行飞跃操作";
        return;
    }

    //找到该飞机当前所在的格子
    int currentTile = findPlaneCurrentTile(globalPlaneId,tileStates);
    if(currentTile == -1){
        qWarning() << "未能找到飞机 globalPlaneId=" << globalPlaneId << "所在的格子";
        return;
    }

    //从当前所在的格子移除该飞机
    removePlaneFromTile(globalPlaneId ,currentTile ,tileStates);

    //检查是否在特殊跳跃位置
    int specialJumpTarget = getSpecialJumpTarget(currentPlayerId , currentTile);
    if(specialJumpTarget != -1){
        //执行特殊跳跃
        addPlaneToTile(globalPlaneId,specialJumpTarget,tileStates);

        //处理飞跃时是否发生碰撞
        collisionDuringFly(currentPlayerId,tileStates,state);

        //处理可能的碰撞
        handleCollision(globalPlaneId,specialJumpTarget,tileStates,state);

        //广播更新后的游戏状态
        broadcastGameState(state);
        return ;
    }

    //执行常规飞跃
    int steps = 4;
    int currentPos = currentTile;
    for(int i=0 ;i<steps; i++){
        if(isExitRingPosition(currentPlayerId,currentPos)){
            currentPos = getNextOnExitPath(currentPlayerId,currentPos);
        }else{
            currentPos = getNextPosition(currentPlayerId ,currentPos);
        }
    }

    // 将飞机放到最终位置
    addPlaneToTile(globalPlaneId ,currentPos,tileStates);

    //处理撞机
    handleCollision(globalPlaneId ,currentPos,tileStates,state);

    state.setTileStates(tileStates);
    broadcastGameState(state);
}

void ServerController::check_is_win(GameState& state)
{
    qDebug() << "[Debug] check_is_win called.";
    QMap<int, QList<int>> playerAirports = {
        {1, {1, 2, 3, 4}},    // 黄
        {2, {5, 6, 7, 8}},    // 蓝
        {3, {9, 10, 11, 12}}, // 绿
        {4, {13, 14, 15, 16}} // 红
    };

    for (auto it = playerAirports.begin(); it != playerAirports.end(); ++it) {
        int playerId = it.key();
        QList<int> airportTiles = it.value();

        bool hasWon = true;
        for (int tileId : airportTiles) {
            QList<int> planes = state.getTileStates().value(tileId);
            if (!planes.contains(100)) {
                hasWon = false;
                break;
            }
        }

        if (hasWon) {
            QString playerColor = getPlayerColor(playerId);
            QString winMessage = QString("玩家 %1 已赢得游戏！").arg(playerColor);
            broadcastMessage(winMessage);
            qDebug() << winMessage;
            return;
        }
    }
}

int ServerController::getSpecialJumpTarget(int clientId, int currentPos) {
    switch (clientId) {
    case 1: if (currentPos == 38) return 50; break; // Yellow
    case 2: if (currentPos == 51) return 63; break; // Blue
    case 3: if (currentPos == 25) return 37; break; // Green
    case 4: if (currentPos == 64) return 24; break; // Red
    }
    return -1;
}

int ServerController::do_plan_OP(int clientId, int dice, int planeId, GameState& state)
{
    qDebug() << "[Debug] do_plan_OP called for client" << clientId << "dice" << dice << "plane" << planeId;
    QMutexLocker locker(&clientsMutex);

    const int globalPlaneId = (clientId - 1)*4 + planeId;
    bool backwardFlag = false;

    QMap<int ,QList<int>> tileStates = state.getTileStates();
    if(tileStates.isEmpty()){
        qCritical() << "tileStates 为空，无法执行操作";
        return 0;
    }

    //查找当前飞机位置
    int currentTile = findPlaneCurrentTile(globalPlaneId,tileStates);
    if(currentTile == -1){
        qCritical() << "未能找到飞机 globalPlaneId=" << globalPlaneId;
        return 0;
    }

    //机场处理逻辑
    if(isInAirport(currentTile)){
        if(dice == 5 || dice == 6){
            removePlaneFromTile(globalPlaneId,currentTile,tileStates);
            const int startTile = getStartTile(clientId);
            addPlaneToTile(globalPlaneId,startTile,tileStates);
            broadcastGameState(state);
        }
        return 0;
    }

    //移动逻辑
    int steps = dice;
    int currentPosition = currentTile;
    while(steps-- >0){
        QThread::msleep(100);

        removePlaneFromTile(globalPlaneId,currentPosition,tileStates);
        int nextPos = isExitRingPosition(clientId ,currentPosition)
                    ? getNextOnExitPath(clientId,currentPosition)
                          : getNextPosition(clientId,currentPosition);
        if(backwardFlag) nextPos = currentPosition-1;

        addPlaneToTile(globalPlaneId ,nextPos,tileStates);
        broadcastGameState(state);
        currentPosition = nextPos;

        //终点检测
        if(isFinalEnd(clientId , currentPosition)){
            if(steps > 0){
                backwardFlag = true;
            }else{
                removePlaneFromTile(globalPlaneId,currentPosition,tileStates);
                const int airporTile = getAirportTile(clientId ,planeId);
                addPlaneToTile(100,airporTile,tileStates);
                broadcastGameState(state);
            }
        }
    }
    model.setBoardState(tileStates);
    state.setTileStates(tileStates);

    //碰撞处理
    handleCollision(globalPlaneId ,currentPosition,tileStates,state);

    return isTileColorMatchesClient(clientId , currentPosition)
                   && !isExitRingPosition(clientId , currentPosition) ? 1 : 0;

}

int ServerController::findPlaneCurrentTile(int globalPlaneId, QMap<int, QList<int> > &tileStates)
{
    for(auto it = tileStates.begin();it != tileStates.end();it++){
        if(it.value().contains(globalPlaneId)){
            return it.key();
        }
    }
    qWarning() << "findPlaneCurrentTile: Plane" << globalPlaneId << "not found on any tile.";
    return -1;
}

void ServerController::removePlaneFromTile(int planeId, int tileId, QMap<int, QList<int> > &tileStates)
{
    if(!tileStates.contains(tileId)) {
        qWarning() << "removePlaneFromTile: Tile" << tileId << "not found in board state.";
        return;
    }
    bool removed = tileStates[tileId].removeAll(planeId);
    if (!removed) {
        qWarning() << "removePlaneFromTile: Plane" << planeId << "was not found on tile" << tileId << "to remove.";
    }
}

void ServerController::addPlaneToTile(int planeId, int tileId, QMap<int, QList<int> > &tileStates)
{
    if (!tileStates.contains(tileId)) {
        qWarning() << "addPlaneToTile: Attempting to add plane" << planeId << "to non-existent tile" << tileId << ". Creating tile entry.";
    }
    tileStates[tileId].append(planeId);
}

bool ServerController::isInAirport(int tileId)
{
    return tileId >= 1 && tileId <= 16;
}

int ServerController::getStartTile(int clientId)
{
    switch(clientId) {
    case 1: return 17;
    case 2: return 18;
    case 3: return 20;
    case 4: return 19;
    default: throw std::invalid_argument("无效的clientId");
    }
}

int ServerController::getAirportTile(int clientId, int planeId)
{
    return (clientId - 1) * 4 + planeId;
}

int ServerController::getNextPosition(int clientId, int currentPos)
{
    const int RING_START = 21;
    const int RING_END = 72;

    if (currentPos >= RING_START && currentPos < RING_END)
        return currentPos + 1;
    if (currentPos == RING_END)
        return RING_START;

    // 起点特殊处理
    switch(clientId) {
    case 1: return (currentPos == 17) ? 21 : currentPos+1;
    case 2: return (currentPos == 18) ? 34 : currentPos+1;
    case 3: return (currentPos == 20) ? 60 : currentPos+1;
    case 4: return (currentPos == 19) ? 47 : currentPos+1;
    default: return currentPos+1;
    }
}

bool ServerController::isExitRingPosition(int clientId, int pos)
{
    static QMap<int, int> exitPositions{{1,70}, {2,31}, {3,57}, {4,44}};
    return pos == exitPositions.value(clientId, -1);
}

int ServerController::getNextOnExitPath(int clientId, int currentPos)
{
    static QMap<int, int> exitPaths{{1,73}, {2,79}, {3,91}, {4,85}};
    return exitPaths.value(clientId, currentPos);
}

bool ServerController::isFinalEnd(int clientId, int pos)
{
    static QMap<int, int> endPositions{{1,78}, {2,84}, {3,96}, {4,90}};
    return pos == endPositions.value(clientId, -1);
}

void ServerController::handleCollision(int selfPlaneId, int tileId, QMap<int, QList<int> > &tileStates, GameState& state)
{
    QList<int> &planes = tileStates[tileId];
    if(planes.size() <= 1)return ;

    QList<int> toRemove;
    foreach (int pid, planes) {
        if(pid != selfPlaneId && !isSameColor(pid , selfPlaneId)){
            toRemove.append(pid);
        }
    }
    foreach (int pid, toRemove) {
        planes.removeAll(pid);
        const int otherClient = (pid - 1)/4 + 1;
        const int otherPlane = (pid - 1)%4 + 1;
        const int airportTile = getAirportTile(otherClient,otherPlane);
        addPlaneToTile(pid ,airportTile,tileStates);
    }
    broadcastGameState(state);
}

bool ServerController::isSameColor(int planeId1, int planeId2)
{
    return ((planeId1 - 1)/4) == ((planeId2 - 1)/4);
}

bool ServerController::isTileColorMatchesClient(int clientId, int tileId)
{
    if (tileId < 21 || tileId > 72) return false;
    const int offset = (tileId - 21) % 4;
    const QMap<int, int> colorMap{{0,3}, {1,1}, {2,2}, {3,4}};
    return colorMap.value(offset, 0) == clientId;
}

void ServerController::collisionDuringFly(int clientId, QMap<int, QList<int> > &tileStates, GameState& state)
{
    static QMap<int, int> flyTiles{{1,87}, {2,93}, {3,81}, {4,75}};
    const int tileId = flyTiles.value(clientId, 0);

    QList<int> &planes = tileStates[tileId];
    if (planes.size() <= 1) return;

    QList<int> toRemove;
    foreach (int pid, planes) {
        if (pid != clientId && !isSameColor(pid, clientId)) {
            toRemove.append(pid);
        }
    }

    foreach (int pid, toRemove) {
        planes.removeAll(pid);
        const int otherClient = (pid - 1)/4 + 1;
        const int otherPlane = (pid - 1)%4 + 1;
        const int airportTile = getAirportTile(otherClient, otherPlane);
        addPlaneToTile(pid, airportTile, tileStates);
    }

    broadcastGameState(state);
}

void ServerController::nextTurn()
{
    qDebug() << "[Debug] nextTurn: Entered nextTurn method.";

    if (clients.isEmpty()) {
        qDebug() << "[Debug] nextTurn: No clients connected, resetting currentPlayerId and returning.";
        currentPlayerId = 0;
        return;
    }

    bool currentPlayerStillConnected = false;
    {
        QMutexLocker clientListLocker(&clientsMutex);
        if(clients.contains(currentPlayerId)){
            currentPlayerStillConnected = true;
        }
    }

    qDebug() << "[Debug] nextTurn: Last dice roll was" << lastDice << ". Current player" << currentPlayerId << "is" << (currentPlayerStillConnected ? "connected." : "not connected.");

    if(lastDice != 6 || !currentPlayerStillConnected){
        qDebug() << "[Debug] nextTurn: Advancing to next player.";
        QMutexLocker clientListLocker(&clientsMutex);
        if (clients.isEmpty()) {
            qDebug() << "[Debug] nextTurn: No clients connected after trying to advance. Resetting and returning.";
            currentPlayerId = 0;
            return;
        }
        int initialPlayerId = currentPlayerId;
        if (desiredPlayers > 0) {
            int attempts = 0;
            do {
                currentPlayerId = (currentPlayerId % desiredPlayers) + 1;
                attempts++;
                if (attempts > desiredPlayers + 1) {
                    qWarning() << "[Debug] nextTurn: Could not find a connected next player after multiple attempts. Breaking loop.";
                    currentPlayerId = 0;
                    broadcastMessage("错误：无法找到有效的下一位玩家。");
                    return;
                }
            } while (!clients.contains(currentPlayerId) && currentPlayerId != initialPlayerId);
        } else {
            qWarning() << "[Debug] nextTurn: desiredPlayers is 0, cannot advance turn.";
            currentPlayerId = 0;
            return;
        }

        if (!clients.contains(currentPlayerId)) {
            qInfo() << "No valid next player found. Game might be over or waiting.";
            currentPlayerId = 0;
            broadcastMessage("没有有效的下一位玩家，游戏可能已结束或等待中。");
            return;
        }
    }
    qInfo() << "Next turn: Player" << currentPlayerId << "(" << getPlayerColor(currentPlayerId) << ")";

    const QString currentColor = getPlayerColor(currentPlayerId);
    qDebug() << "[Debug] nextTurn: Sending YOUR_TURN_ROLL_AND_CHOOSE_PLANE to player" << currentPlayerId;
    sendToClient(currentPlayerId, "TEXT_MSG", QVariant("YOUR_TURN_ROLL_AND_CHOOSE_PLANE"));

    qDebug() << "[Debug] nextTurn: Notifying other players about current turn.";
    for(auto it = clients.begin();it !=clients.end();it++){
        const int clientId = it.key();
        if(clientId != currentPlayerId){
            sendToClient(clientId, "TEXT_MSG", QVariant(QString("轮到玩家 %1 (%2) 操作").arg(currentPlayerId).arg(currentColor)));
        }
    }
    qDebug() << "[Debug] nextTurn: nextTurn method complete.";
}





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
    QMutexLocker locker(&clientsMutex);
    try{
        ClientHandler* handler = new ClientHandler(socketDescriptor,clientId,this);
        connect(handler , &ClientHandler::messageReceived, this , &ServerController::handleClientAction);
        connect(handler , &ClientHandler::disconnected , this , &ServerController::removeClient);
        clients.insert(clientId,handler);
        QThreadPool::globalInstance()->start(handler);
        qInfo() << "Client" << clientId <<"connected";
    }catch(const std::exception& e){
        qCritical() << "Add client failed:" <<e.what();
    }
}

void ServerController::removeClient(int clientId)
{
    QMutexLocker locker(&clientsMutex);
    if (clients.contains(clientId)) {
        ClientHandler* handler = clients.take(clientId);
        handler->deleteLater();
        qInfo() << "Client" << clientId << "disconnected";
    }
}

void ServerController::sendToClient(int clientId, const QString &message)
{
    QMutexLocker locker(&clientsMutex);
    if (clients.contains(clientId)) {
        QMetaObject::invokeMethod(clients[clientId], [=](){
            clients[clientId]->sendMessage(message);
        }, Qt::QueuedConnection);
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
ClientHandler::ClientHandler(qintptr descriptor, int clientId, ServerController *controller)
    : socketDescriptor(descriptor) , clientId(clientId),controller(controller) , socket(nullptr) , in(nullptr)
{
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor)){
        throw std::runtime_error(socket->errorString().toStdString());
    }
    in = new QDataStream(socket);
    in->setVersion(QDataStream::Qt_6_5);
}

ClientHandler::~ClientHandler()
{
    socket->deleteLater();
    delete in;
}

void ClientHandler::run()
{
    // 发送欢迎消息
    QByteArray welcomeMsg;
    QDataStream out(&welcomeMsg, QIODevice::WriteOnly);
    out << QString("WELCOME:Connected as player %1 . Please send 'READY' to start.").arg(clientId);
    sendMessage(welcomeMsg);

    // 进入事件循环
    QEventLoop loop;
    connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::readData);
    connect(socket, &QTcpSocket::disconnected, &loop, &QEventLoop::quit);
    loop.exec();
}
void ClientHandler::sendMessage(const QString &message)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        QByteArray packet;
        QDataStream stream(&packet, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_6_5);
        stream << quint32(0) << message;
        stream.device()->seek(0);
        stream << quint32(packet.size() - sizeof(quint32));
        socket->write(packet);
    }
}

void ClientHandler::readData()
{
    while (socket->bytesAvailable() >= sizeof(quint32)) {
        if (expectedBytes == 0) {
            *in >> expectedBytes;
        }
        if (socket->bytesAvailable() < expectedBytes) return;

        QString data;
        *in >> data;
        expectedBytes = 0;
        emit messageReceived(clientId, data);
    }
}

// 游戏逻辑处理
void ServerController::handleClientAction(int clientId, const QString &action)
{
    QMutexLocker locker(&clientsMutex);

    if(action.startsWith("READY")){
        if(clientId <= desiredPlayers){
            readyPlayers++;
            QString msg = QString("玩家 %1 已准备(%2/%3)")
                              .arg(clientId).arg(readyPlayers).arg(desiredPlayers);
            broadcastMessage(msg);
            qInfo() <<msg;

            if(readyPlayers == desiredPlayers){
                initGameAndStart();
            }
        }
        else{
            sendToClient(clientId, "ERROR:You are not in the desired players list!");
        }
            return;
    }
    if(readyPlayers < desiredPlayers){
        sendToClient(clientId,"ERROR:Game not started yet. Please wait.");
        return ;
    }
    if(clientId != currentPlayerId){
        sendToClient(clientId,"ERROR:Not your turn!");
        return ;
    }
    try{
        if(action.startsWith("PLANE_OP:")){
            QString data = action.mid(9);
            QStringList parts = data.split(";");
            if(parts.size() != 2) throw std::invalid_argument("Invalid number format");

            bool ok1,ok2;
            int dice = parts[0].toInt(&ok1);
            int planeId = parts[1].toInt(&ok2);

            if(!ok1 || !ok2 ) throw std::invalid_argument("Invalid number format");

            qInfo() << "玩家" << clientId << "投了" << "操作飞机" <<planeId;

            GameState state(model.getBoardState());
            int result = do_plan_OP(clientId,dice,planeId,state);
            lastDice = dice;
            lastPlaneId = planeId;

        if(result == 1){
                sendToClient(clientId,"YOUR_TURN_CHOOSE_FLY");
            }else{
                check_is_win(state);
                nextTurn();
            }
        }
        else if(action.startsWith("FLY_OVER:")){
            QString choice = action.mid(9);
            qInfo() << "玩家" <<clientId << "选择飞跃？"<< choice;

            GameState state(model.getBoardState());
            do_fly(lastPlaneId,currentPlayerId,choice,state);
            nextTurn();
        }
        else {
            qInfo() << "客户端" << clientId << "执行动作:" << action;
            nextTurn();
        }
    }catch(const std::exception& e){
        sendToClient(clientId,QString("ERROR:Invalid action! %1").arg(e.what()));
    }
}

void ServerController::broadcastMessage(const QString &msg)
{
    QMutexLocker locker(&clientsMutex);
    QByteArray messageData;
    QDataStream stream(&messageData , QIODevice::WriteOnly);
    stream << msg;

    for(ClientHandler* handler : clients.values()){
        QMetaObject::invokeMethod(handler,[=](){
            handler->sendMessage(messageData);
        },Qt::QueuedConnection);
    }
}

void ServerController::broadcastGameState(const GameState state)
{
    QByteArray stateData;
    QDataStream stream(&stateData, QIODevice::WriteOnly);
    stream << state;  // 需要 GameState 支持 QDataStream 序列化

    for (ClientHandler* handler : clients.values()) {
        QMetaObject::invokeMethod(handler, [=]() {
            handler->sendMessage(stateData);
        }, Qt::QueuedConnection);
    }
}

void ServerController::initGameAndStart()
{
    QMutexLocker locker(&clientsMutex);

    qInfo() << "所有玩家已准备，初始化游戏...";

    model.initGame(desiredPlayers);

    GameState initialState(model.getBoardState());
    broadcastGameState(initialState);

    currentPlayerId = 1;

    sendToClient(currentPlayerId, "YOUR_TURN_ROLL_AND_CHOOSE_PLANE");
}

void ServerController::do_fly(int lastPlaneId, int currentPlayerId, const QString &choice, GameState state)
{
    if(choice.toUpper() != "YES") return ;

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

void ServerController::check_is_win(GameState state)
{
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
    case 3: // 绿
        if (currentPos == 25) return 37;
        break;
    case 1: // 黄
        if (currentPos == 38) return 50;
        break;
    case 2: // 蓝
        if (currentPos == 51) return 63;
        break;
    case 4: // 红
        if (currentPos == 64) return 24;
        break;
    }
    return -1;
}

int ServerController::do_plan_OP(int clientId, int dice, int planeId, GameState state)
{
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
    else return 0;

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

    //碰撞处理
    handleCollision(globalPlaneId ,currentPosition,tileStates,state);

    return isTileColorMatchesClient(clientId , currentPosition)
                   && !isExitRingPosition(clientId , currentPosition) ? 1 : 0;

}

int ServerController::findPlaneCurrentTile(int globalPlaneId, QMap<int, QList<int> > tileStates)
{
    for(auto it = tileStates.begin();it != tileStates.end();it++){
        if(it.value().contains(globalPlaneId)){
            return it.key();
        }
    }
    return -1;
}

void ServerController::removePlaneFromTile(int planeId, int tileId, QMap<int, QList<int> > tileStates)
{
    if(!tileStates.contains(tileId)) return ;
    QList<int> &planes = tileStates[tileId];
    planes.removeAll(planeId);
}

void ServerController::addPlaneToTile(int planeId, int tileId, QMap<int, QList<int> > tileStates)
{
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
    if (currentPos >= 21 && currentPos < 72) return currentPos + 1;
    if (currentPos == 72) return 21;

    switch(currentPos) {
    case 17: return 21;
    case 18: return 34;
    case 19: return 47;
    case 20: return 60;
    default: return currentPos + 1;
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

void ServerController::handleCollision(int selfPlaneId, int tileId, QMap<int, QList<int> > tileStates, GameState state)
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

void ServerController::collisionDuringFly(int clientId, QMap<int, QList<int> > tileStates, GameState state)
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
    QMutexLocker locker(&clientsMutex);

    if(!clients.isEmpty()){
        if(lastDice != 6){
            currentPlayerId = (currentPlayerId % desiredPlayers) + 1;
        }

        const QString currentColor = getPlayerColor(currentPlayerId);

        sendToClient(currentPlayerId , "YOUR_TURN_ROLL_AND_CHOOSE_PLANE");

        for(auto it = clients.begin();it !=clients.end();it++){
            const int clientId = it.key();
            if(clientId != currentPlayerId){
                sendToClient(clientId,QString("等待%1色玩家操作").arg(currentColor));
            }
        }
    }
}





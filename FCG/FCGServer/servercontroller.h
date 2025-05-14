#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QThreadPool>
#include <QTcpSocket>
#include <QDataStream>
#include <QMutex>
#include <../FCGClient/model/gamemodel.h>
#include <../FCGClient/model/gamestate.h>
#include <QVariant>

class ClientHandler;

class ServerController : public QObject
{
    Q_OBJECT
public:
    explicit ServerController(QObject *parent = nullptr);
    ~ServerController();

    //客户端信息处理
    void setDesiredPlayers(int desiredPlayers);
    void addClient(QTcpSocket* clientSocket, int clientId);
public slots:
    void removeClientSlot(int clientId);
    void handleClientAction(int clientId, const QString &messageType, const QVariant &payload1, const QVariant &payload2);

private:


    //成员变量
    QMap<int , ClientHandler*> clients;
    QMutex clientsMutex;
    QMutex gameLogicMutex;
    GameModel model;

    bool gameHasEnded = false;
    int currentPlayerId = 0;
    int desiredPlayers = 0;
    int readyPlayers = 0;
    int lastDice = 0;
    int lastPlaneId = -1;
    QMap<int, bool> playerReadyStatus;
    QMap<int ,QString> playerColors;

    //客户端信息处理
    void sendToClient(int clientId, const QString &messageType, const QVariant &payload1 = QVariant()
                      , const QVariant &payload2 = QVariant());
    void broadcastMessage(const QString &msg);
    void broadcastGameState(const GameState& state);

    void initGameAndStart();
    void do_fly(int lastPlaneId,int currentPlayerId,const QString &choice ,QMap<int, QList<int>>& tileStates);
    void check_is_win(GameState &state);
    int getSpecialJumpTarget(int clientId,int currentPos);
    int do_plan_OP(int clientId,int dice,int planeId, QMap<int, QList<int>>& tileStates);
    int findPlaneCurrentTile(int globalPlaneId,QMap<int ,QList<int>> &tileStates);
    void removePlaneFromTile(int planeId,int tileId,QMap<int ,QList<int>> &tileStates);
    void addPlaneToTile(int planeId,int tileId,QMap<int ,QList<int>> &tileStates);
    bool isInAirport(int tileId);
    int getStartTile(int clientId);
    int getAirportTile(int clientId,int planeId);
    int getNextPosition(int clientId,int currentPos);
    bool isExitRingPosition(int clientId,int pos);
    int getNextOnExitPath(int clientId,int currentPos);
    bool isFinalEnd(int clientId,int pos);
    void handleCollision(int selfPlaneId,int tileId,QMap<int ,QList<int>> &tileStates);
    bool isSameColor(int planeId1,int planeId2);
    bool isTileColorMatchesClient(int clientId,int tileId);
    void collisionDuringFly(int clientId,QMap<int ,QList<int >> &tileStates);
    void nextTurn();

    QString getPlayerColor(int clientId);

};
class ClientHandler : public QObject
{
    Q_OBJECT

public:
    ClientHandler(QTcpSocket* socket, int clientId, ServerController* controller, QObject* parent = nullptr);
    ~ClientHandler();

    Q_INVOKABLE void sendTypedMessage(const QString& messageType, const QVariant& payload1 = QVariant(), const QVariant& payload2 = QVariant());
    Q_INVOKABLE void sendMessage(const QString & message);
    Q_INVOKABLE void sendGameState(const GameState &state);
    int getClientId();

signals:
    void parsedMessage(int clientId, const QString& messageType, const QVariant& payload1, const QVariant& payload2);
    void clientDisconnected(int clientId);

private slots:
    void readData();
    void handleDisconnected();

private:

    int clientId;
    ServerController* controller;
    QTcpSocket* socket;
    QDataStream* inStream;
    quint32 expectedBytes = 0;

    QString getPlayerColor(int cId);
};
#endif // SERVERCONTROLLER_H

#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QThreadPool>
#include <QMutex>
#include <../FCGClient/model/gamemodel.h>
#include <../FCGClient/model/gamestate.h>

class QTcpSocket;
class QDataStream;
class ClientHandler;

class ServerController : public QObject
{
    Q_OBJECT
public:
    explicit ServerController(QObject *parent = nullptr);
    ~ServerController();

    //客户端信息处理
    void setDesiredPlayers(int desiredPlayers);
    void addClient(qintptr socket , int clientId);
    void removeClient(int clientId);

private:


    //成员变量
    QThreadPool executor;
    QMap<int , ClientHandler*> clients;
    QMutex clientsMutex;
    GameModel model;

    int lastUsers;
    int lastNameId = 0;
    int currentPlayerId = 1;
    int desiredPlayers = 1;
    int readyPlayers = 0;
    int lastDice = 0;
    int lastPlaneId = 0;

    //客户端信息处理
    void handleClientAction(int clientId, const QString &action);
    void broadcastMessage(const QString &msg);
    void broadcastGameState(const GameState state);

    void initGameAndStart();
    void do_fly(int lastPlaneId,int currentPlayerId,const QString &choice ,GameState state);
    void check_is_win(GameState state);
    int getSpecialJumpTarget(int clientId,int currentPos);
    int do_plan_OP(int clientId,int dice,int planeId,GameState state);
    int findPlaneCurrentTile(int globalPlaneId,QMap<int ,QList<int>> tileStates);
    void removePlaneFromTile(int planeId,int tileId,QMap<int ,QList<int>> tileStates);
    void addPlaneToTile(int planeId,int tileId,QMap<int ,QList<int>> tileStates);
    bool isInAirport(int tileId);
    int getStartTile(int clientId);
    int getAirportTile(int clientId,int planeId);
    int getNextPosition(int clientId,int currentPos);
    bool isExitRingPosition(int clientId,int pos);
    int getNextOnExitPath(int clientId,int currentPos);
    bool isFinalEnd(int clientId,int pos);
    void handleCollision(int selfPlaneId,int tileId,QMap<int ,QList<int>> tileStates,GameState state);
    bool isSameColor(int planeId1,int planeId2);
    bool isTileColorMatchesClient(int clientId,int tileId);
    void collisionDuringFly(int clientId,QMap<int ,QList<int >> tileStates,GameState state);
    void nextTurn();

    void sendToClient(int clientId, const QString &message);
    QString getPlayerColor(int clientId);

};
class ClientHandler : public QObject , public QRunnable
{
    Q_OBJECT

public:
    ClientHandler(qintptr descriptor , int clientId, ServerController* controller);
    ~ClientHandler();

    void sendMessage(const QString & message);

signals:
    void messageReceived(int clientId , QString data);
    void disconnected(int clientId);

protected:
    void run() override;

private slots:
    void readData();
    //void handleDicconnected();

private:
    //void processMessage(const QString & data);

    qintptr socketDescriptor;
    int clientId;
    ServerController* controller;
    QTcpSocket* socket;
    QDataStream* in;
    quint32 expectedBytes = 0;
};
#endif // SERVERCONTROLLER_H

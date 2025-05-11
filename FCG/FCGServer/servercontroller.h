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

    class ClientHandler : public QObject , public QRunnable
    {
        Q_OBJECT

    public:
        ClientHandler(qintptr socketDescriptor , int clientId, ServerController* controller);
        ~ClientHandler();

        void sendMessage(const QByteArray & message);

    signals:
        void messageReceived(int clientId , QByteArray data);
        void disconnected(int clientId);

    protected:
        void run() override;

    private slots:
        void readData();
        void handleDicconnected();

    private:
        void processMessage(const QByteArray & data);

        qintptr socketDescriptor;
        int clientId;
        ServerController* controller;
        QTcpSocket* socket;
        QDataStream* in;
        quint32 expectedBytes = 0;
    };
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
    void handleClientAction(int clientId, const QByteArray &data);

    void sendToClient(int clientId, const QByteArray &message);

};

#endif // SERVERCONTROLLER_H

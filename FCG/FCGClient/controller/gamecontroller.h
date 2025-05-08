#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QTcpSocket>
#include <mainview.h>
#include <model/gamemodel.h>
#include <QObject>

//class MainView;

class GameController : public QObject
{
    Q_OBJECT

public:
    GameController(GameModel* model ,const QString& host,int port,QObject* parent = nullptr);
    void setView(MainView* view);

public slots:
    void sendReady();
    void sendPlaneOperation(int dice ,int planeId);
    void sendFlyOverChoice(bool isYes);

signals:
    void gameStateUpdated(const QMap<int,QList<int>> & tileStates);
    void serverMessageReceived(const QString& message);
    void connectionStatusChanged(bool connected);
    void updateGamePhase(ControlPanel::GamePhase phase , const QString& message);

private slots:
    void handleConnected();
    void handleReadyRead();
    void handleError(QAbstractSocket::SocketError error);
    void handleDisconnected();

private:
    GameModel* model;
    MainView* view;
    QTcpSocket* socket;
    QDataStream inStream;
    bool isConnected;

    //void connectToServer(const QString& host,int port);
    void processServerMessage(const QByteArray& message);
    void handleTextMessage(const QString& message);
    void closeConnection();

};

#endif // GAMECONTROLLER_H

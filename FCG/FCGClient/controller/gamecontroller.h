#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QTcpSocket>
#include <QDataStream>
#include "mainview.h"
//#include <view/controlpanel.h>
#include <model/gamemodel.h>
#include <QObject>
#include <QTimer>
#include <QVariant>

class ControlPanel;
class MainView;

class GameController : public QObject
{
    Q_OBJECT

public:
    explicit GameController(GameModel* model ,const QString& host,int port,QObject* parent = nullptr);
    ~GameController();
    void setView(MainView* view);
    void connectToServer();


public slots:
    void sendReady();
    void sendPlaneOperation(int dice ,int planeId);
    void sendFlyOverChoice(bool isYes);
    void closeConnection();

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
    QString host;
    int port;
    bool isConnected;
    quint32 expectedBytes = 0;
    QTimer* connectTimer = nullptr;

    void sendTypedMessage(const QString& messageType, const QVariant& payload1 = QVariant(), const QVariant& payload2 = QVariant());
};

#endif // GAMECONTROLLER_H

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QTcpSocket>
#include <QObject>
#include <mainview.h>
#include <model/gamemodel.h>

//class MainView;

class GameController : public QObject
{
public:
    GameController(GameModel* model ,const QString& host,int port);
    void setView(MainView* view);

public slots:
    void sendReady();
    void sendPlaneOperation(int dice ,int planeId);
    void sendFlyOverChoice(bool isYes);

private:
    GameModel* model;
    MainView* view;
    QTcpSocket* socket;
    bool isConnected = false;

    void connectToServer(const QString& host,int port);
    void processServerMessage(const QByteArray& message);
};

#endif // GAMECONTROLLER_H

#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QInputDialog>
#include "servercontroller.h"

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(QWidget *parentWidget = nullptr, QObject *parent = nullptr);
    void startServer();

private slots:
    void handleNewConnection();

private:
    QTcpServer *tcpServer;
    ServerController *serverController;
    QWidget *m_parentWidget;  // 用于显示对话框的父窗口
    int clientIdCounter = 1;
    int desiredPlayers = 0;
};

#endif // GAMESERVER_H

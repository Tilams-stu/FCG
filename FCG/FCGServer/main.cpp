#include "gameserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameServer server;
    server.startServer();

    return a.exec();
}

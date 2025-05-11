#include "gamemodel.h"
#include <QDebug>
GameModel::GameModel() {}

void GameModel::initializeBoard(int playerCount)
{
    if(playerCount < 1 || playerCount > 4){
        qCritical() << "Invalid player count:"<< playerCount;
    }

    boardState.clear();
    const int tilesPerPlayer = 4;
    const int totalTiles = playerCount * tilesPerPlayer;

    for(int tileId = 1; tileId <= totalTiles; tileId++){
        boardState.insert(tileId, QList<int>{tileId});
    }
}

void GameModel::initGame(int playerCount)
{
    initializeBoard(playerCount);
}

QMap<int, QList<int> > GameModel::getBoardState() const
{
    return boardState;
}


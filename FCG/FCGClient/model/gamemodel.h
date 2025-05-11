#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QMap>
#include <QList>

class GameModel
{
public:
    GameModel();

    void initializeBoard(int playerCount);
    void initGame(int playerCount);
    QMap<int ,QList<int >> getBoardState() const;

private:
    QMap<int , QList<int>> boardState;
};

#endif // GAMEMODEL_H

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QMap>
#include <QList>
#include <QDataStream>

class GameState
{
public:
    GameState() = default;
    explicit GameState(const QMap<int, QList<int>>& tileStates);

    QMap<int, QList<int>> getTileStates() const;
    void setTileStates(const QMap<int, QList<int>>& tileStates);

    friend QDataStream& operator<<(QDataStream& out, const GameState& state);
    friend QDataStream& operator>>(QDataStream& in, GameState& state);

private:
    QMap<int, QList<int>> tileStates;

};

#endif // GAMESTATE_H

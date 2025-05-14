#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QMap>
#include <QList>
#include <QDataStream>
#include <QVariant>

class GameState
{
public:
    GameState(const QMap<int, QList<int>>& initialTileStates = {});

    QMap<int, QList<int>> getTileStates() const;
    void setTileStates(const QMap<int, QList<int>>& tileStates);

    friend QDataStream& operator<<(QDataStream& out, const GameState& state);
    friend QDataStream& operator>>(QDataStream& in, GameState& state);

private:
    QMap<int, QList<int>> tileStates;

};
Q_DECLARE_METATYPE(GameState)

#endif // GAMESTATE_H

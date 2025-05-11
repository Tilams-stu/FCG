#include "gamestate.h"

GameState::GameState(const QMap<int, QList<int>>& tileStates)
    : tileStates(tileStates) {}

QMap<int, QList<int>> GameState::getTileStates() const
{
    return tileStates;
}

void GameState::setTileStates(const QMap<int, QList<int>>& tileStates)
{
    this->tileStates = tileStates;
}

QDataStream& operator<<(QDataStream& out, const GameState& state)
{
    out << state.tileStates;
    return out;
}

QDataStream& operator>>(QDataStream& in, GameState& state)
{
    in >> state.tileStates;
    return in;
}

#include "gamemodel.h"
#include <QDebug>

const int YELLOW_AIRPORT_START_ID = 1;
const int BLUE_AIRPORT_START_ID = 5;
const int GREEN_AIRPORT_START_ID = 9;
const int RED_AIRPORT_START_ID = 13;
const int PLANES_PER_PLAYER = 4;
const int TOTAL_BOARD_TILES = 96;

GameModel::GameModel(QObject *parent) : QObject(parent){}

void GameModel::initGame(int playerCount)
{
    if (playerCount < 1 || playerCount > 4) {
        qCritical() << "GameModel::initGame - Invalid player count:" << playerCount;
        return;
    }

    qDebug() << "GameModel::initGame - Initializing for" << playerCount << "players.";
    boardState.clear();

    for (int i = 1; i <= TOTAL_BOARD_TILES; ++i) {
        boardState.insert(i, QList<int>()); // Empty list of planes
    }

    for (int p = 0; p < playerCount; ++p) { // Iterate through active players
        int playerID = p + 1; // Player ID 1 to 4
        int airport_start_tile_id = 0;

        switch (playerID) {
        case 1: // Yellow
            airport_start_tile_id = YELLOW_AIRPORT_START_ID;
            break;
        case 2: // Blue
            airport_start_tile_id = BLUE_AIRPORT_START_ID;
            break;
        case 3: // Green
            airport_start_tile_id = GREEN_AIRPORT_START_ID;
            break;
        case 4: // Red
            airport_start_tile_id = RED_AIRPORT_START_ID;
            break;
        default:
            continue; // Should not happen with playerCount check
        }
        for (int i = 0; i < PLANES_PER_PLAYER; ++i) {
            int planeGlobalID = (playerID - 1) * PLANES_PER_PLAYER + (i + 1);
            int airportTileForThisPlane = airport_start_tile_id + i;

            if (boardState.contains(airportTileForThisPlane)) {
                boardState[airportTileForThisPlane].append(planeGlobalID);
                qDebug() << "GameModel::initGame - Placed plane" << planeGlobalID
                         << "for player" << playerID
                         << "on airport tile" << airportTileForThisPlane;
            } else {
                qWarning() << "GameModel::initGame - Airport tile" << airportTileForThisPlane
                           << "does not exist in boardState map for player" << playerID;
            }
        }
    }
    qDebug() << "GameModel::initGame - Board initialized. Total tiles in map:" << boardState.size();
    if (boardState.contains(YELLOW_AIRPORT_START_ID)) {
        qDebug() << "GameModel::initGame - Planes on Yellow Airport Tile 1:" << boardState[YELLOW_AIRPORT_START_ID];
    }
}

QMap<int, QList<int> > GameModel::getBoardState() const
{
    return boardState;
}
void GameModel::setBoardState(const QMap<int, QList<int>>& newState)
{
    boardState = newState;
    qDebug() << "GameModel: Board state updated. Number of tiles with planes:" << boardState.size();

}


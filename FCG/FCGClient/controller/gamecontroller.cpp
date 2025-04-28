#include "gamecontroller.h"

GameController::GameController(GameModel* model ,const QString& host,int port) {}

void GameController::setView(MainView *view)
{
    this->view = view;
}

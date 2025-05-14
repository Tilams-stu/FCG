#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QMap>
#include <QList>



class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = nullptr);

    void initGame(int playerCount);

    QMap<int, QList<int>> getBoardState() const;

    void setBoardState(const QMap<int, QList<int>>& newState);

private:
    QMap<int, QList<int>> boardState;
};

#endif // GAMEMODEL_H

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTextEdit>
class GameController;
class MainView;


class ControlPanel : public QWidget
{
    Q_OBJECT

public:

    enum GamePhase {
        ROLL_AND_CHOOSE_PLANE,
        CHOOSE_FLY_OVER,
        WAITING,
        GAME_ENDED
    };
    Q_ENUM(GamePhase)

    explicit ControlPanel(MainView* gameView,QWidget *parent = nullptr);
    void setGamePhase(GamePhase phase, const QString& message);
    //void setDiceResult(int value);

signals:
    //void readyClicked();
    //void rollDiceClicked();
    void planeSelected(int planeId);
    void flyOverChoice(bool accepted);

private slots:
    void handleReady();
    void handleRollDice();
    void handlePlaneButton(int id);
    void handleFlyOver(bool yes);

private:
    void setupUI();
    void setupConnections();
    void setAllControlsEnabled(bool enabled);

    MainView* gameView;
    GameController* controller;

    // UI Components
    QLabel* serverMessage;
    QPushButton* readyButton;
    QPushButton* rollDiceButton;
    QPushButton* planeButton1;
    QPushButton* planeButton2;
    QPushButton* planeButton3;
    QPushButton* planeButton4;
    QPushButton* flyYesButton;
    QPushButton* flyNoButton;
    // State
    int currentDice = 0;
};
#endif // CONTROLPANEL_H

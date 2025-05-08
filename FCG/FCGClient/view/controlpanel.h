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



class ControlPanel : public QWidget
{
    Q_OBJECT

public:

    enum GamePhase {
        ROLL_AND_CHOOSE_PLANE,
        CHOOSE_FLY_OVER,
        WAITING
    };
    Q_ENUM(GamePhase)

    explicit ControlPanel( QWidget *parent = nullptr);
    void setGamePhase(GamePhase phase, const QString& message);
    void setDiceResult(int value);

signals:
    void readyClicked();
    void rollDiceClicked();
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

    // UI Components
    QLabel* serverMessage;
    QPushButton* readyButton;
    QPushButton* rollDiceButton;
    QButtonGroup* planeButtons;
    QPushButton* flyYesButton;
    QPushButton* flyNoButton;
    QLabel* diceResultLabel;

    // State
    int currentDice = 0;
};
#endif // CONTROLPANEL_H

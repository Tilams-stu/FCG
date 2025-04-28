#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QLabel>
#include <QWidget>

#include <view/boardpanel.h>
#include <view/controlpanel.h>

class GameController;
class BoardPanel;
class ControlPanel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainView;
}
QT_END_NAMESPACE

class MainView : public QWidget
{
    Q_OBJECT

public:
    explicit MainView(GameController* controller,const QString& username,QWidget *parent = nullptr);
    ~MainView();
    void updateBoardState(const QMap<int,QList<int>>& tileStates);
    void showMessage(const QString& message);


private:
    Ui::MainView *ui;
    BoardPanel* boardPanel;
    ControlPanel* controlPanel;
    GameController* controller;
    QLabel* statusLabel;

    void setupUI();
    void setupConnections();
};
#endif // MAINVIEW_H

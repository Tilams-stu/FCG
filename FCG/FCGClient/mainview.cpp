#include "mainview.h"
//#include "ui_mainview.h"
#include <view/boardpanel.h>
#include <view/controlpanel.h>
#include <controller/gamecontroller.h>
#include <QLabel>
#include <QBoxLayout>

MainView::MainView(GameController* controller,
                   const QString& username,
                   QWidget *parent)
    : QWidget(parent)
    , controller(controller)
    , boardPanel(nullptr)
    , controlPanel(nullptr)
    , username(username)
{
//    ui->setupUi(this);

    setupUI();
    setupConnections();

    setWindowTitle(tr("FCG - %1").arg(username));
    setMinimumSize(1200,800);
}

MainView::~MainView(){}

void MainView::showMessage(const QString &message)
{
    QMessageBox::information(this, "提示", message);
}

ControlPanel* MainView::getControlPanel()
{
    return controlPanel;
}

GameController *MainView::getController()
{
    return controller;
}
void MainView::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    qDebug() << "Creating boardPanel...";
    boardPanel = new BoardPanel(this);
    qDebug() << "Creating controlPanel...";
    controlPanel = new ControlPanel(this);

    mainLayout->addWidget(boardPanel,7);
    mainLayout->addWidget(controlPanel,3);

    setLayout(mainLayout);
}

void MainView::setupConnections()
{
    //ControlPanel* panel = new ControlPanel(this);

    //connect(controller, &GameController::gameStateUpdated,
    //        boardPanel, &BoardPanel::updateBoardState);
    if(controlPanel && controller){
        connect(controller, &GameController::serverMessageReceived,
                this, &MainView::showMessage);
        connect(controller, &GameController::updateGamePhase,
                controlPanel,&ControlPanel::setGamePhase);
    }
}

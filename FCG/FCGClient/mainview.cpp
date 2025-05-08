#include "mainview.h"
#include "ui_mainview.h"
#include <view/boardpanel.h>
#include <view/controlpanel.h>
#include <controller/gamecontroller.h>
#include <QLabel>
#include <QBoxLayout>

MainView::MainView(GameController* controller, const QString& username,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
    , controller(controller)
{
    ui->setupUi(this);

    setupUI();

    setupConnections();

    setWindowTitle(tr("FCG - %1").arg(username));
    setMinimumSize(1200,800);
}

MainView::~MainView()
{
    delete ui;
}

ControlPanel* MainView::getControlPanel()
{
    return controlPanel;
}
void MainView::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    BoardPanel* boardPanel= new BoardPanel(this);
    ControlPanel* controlPanel = new ControlPanel(this);

    mainLayout->addWidget(boardPanel,7);
    mainLayout->addWidget(controlPanel,3);
}

void MainView::setupConnections()
{
    //ControlPanel* panel = new ControlPanel(this);

    //connect(controller, &GameController::gameStateUpdated,
    //        boardPanel, &BoardPanel::updateBoardState);
    connect(controller, &GameController::serverMessageReceived,
            this, &MainView::showMessage);
    connect(controller, &GameController::updateGamePhase,
            controlPanel,&ControlPanel::setGamePhase);
}

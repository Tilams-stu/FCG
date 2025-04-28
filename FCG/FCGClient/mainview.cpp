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

    //setupConnections();

    setWindowTitle(tr("FCG - %1").arg(username));
    setMinimumSize(1200,800);
}

MainView::~MainView()
{
    delete ui;
}
void MainView::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    boardPanel = new BoardPanel(this);
    mainLayout->addWidget(boardPanel, 7);

    //controlPanel = new ControlPanel(this);
    //mainLayout->addWidget(controlPanel, 3);

}

void MainView::setupConnections()
{
    //connect(controller, &GameController::gameStateUpdated,
    //       qobject_cast<BoardPanel*>(ui->boardPanel), &BoardPanel::updateBoardState);

    //connect(ui->controlPanel, &ControlPanel::readyClicked,
    //        controller, &GameController::sendReady);
}

#include "controlpanel.h"
#include "controller/gamecontroller.h"
#include "mainview.h"

ControlPanel::ControlPanel(MainView* gameview,QWidget *parent)
    : QWidget(parent)
{
    this->gameView = gameview;
    this->controller = gameview->getController();
    setupUI();
    setupConnections();
    setAllControlsEnabled(false);

}

void ControlPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    serverMessage = new QLabel(tr("等待服务器连接..."),this);
    serverMessage->setWordWrap(true);
    mainLayout->addWidget(serverMessage);
    mainLayout->addSpacing(10);

    //准备按钮
    QGroupBox* readyGroup = new QGroupBox(tr("游戏准备"),this);
    QVBoxLayout* readyLayout = new QVBoxLayout(readyGroup);
    readyButton = new QPushButton(tr("准备"),readyGroup);
    readyLayout->addWidget(readyButton);
    mainLayout->addWidget(readyGroup);
    mainLayout->addSpacing(10);

    //骰子按钮
    QGroupBox* diceGroup = new QGroupBox(tr("骰子操作"),this);
    QVBoxLayout* diceLayout = new QVBoxLayout(diceGroup);
    rollDiceButton = new QPushButton(tr("投掷骰子"),diceGroup);
    diceLayout->addWidget(rollDiceButton);
    mainLayout->addWidget(diceGroup);
    mainLayout->addSpacing(10);

    //飞机按钮
    QHBoxLayout* planesLayout = new QHBoxLayout;
    planeButton1 = new QPushButton("飞机1");
    planeButton2 = new QPushButton("飞机2");
    planeButton3 = new QPushButton("飞机3");
    planeButton4 = new QPushButton("飞机4");

    planesLayout->addWidget(planeButton1);
    planesLayout->addWidget(planeButton2);
    planesLayout->addWidget(planeButton3);
    planesLayout->addWidget(planeButton4);
    mainLayout->addLayout(planesLayout);
    mainLayout->addSpacing(10);

    //飞跃按钮
    QGroupBox* flyGroup = new QGroupBox(tr("飞跃操作"),this);
    QHBoxLayout* flyLayout = new QHBoxLayout(flyGroup);
    flyYesButton = new QPushButton(tr("飞跃(Yes)"),flyGroup);
    flyNoButton = new QPushButton(tr("不飞跃(No)"),flyGroup);
    flyLayout->addWidget(flyYesButton);
    flyLayout->addWidget(flyNoButton);
    mainLayout->addWidget(flyGroup);
    mainLayout->addSpacing(10);

    //帮助按钮
    QPushButton* helpButton = new QPushButton(tr("游戏帮助"),this);
    mainLayout->addWidget(helpButton);
    connect(helpButton,&QPushButton::clicked,this,[this](){
        QDialog *helpDialog = new QDialog(this);
        helpDialog->setWindowTitle("游戏帮助");
        helpDialog->setMinimumSize(150, 100);
        helpDialog->resize(600, 400);
        helpDialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        helpDialog->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

        QVBoxLayout *lmainLayout = new QVBoxLayout(helpDialog);

        QTextEdit *textEdit = new QTextEdit(helpDialog);
        textEdit->setReadOnly(true);
        textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        textEdit->setStyleSheet(
            "QTextEdit {"
            "  font-family: '微软雅黑';"
            "  font-size: 14px;"
            "  line-height: 1.5;"
            "  color: #34495e;"
            "}"
            "h2 { margin: 15px 0 10px 0; }"
            "h3 { margin: 12px 0 8px 0; }"
            "ul { margin-left: 20px; }"
            "li { margin: 5px 0; }"
            );
        textEdit->setHtml(
            "<h2>游戏规则</h2>"
            "<h3>起飞</h3>"
            "<p>掷得5点以上的点数后，方可将一枚棋子由“基地”起飞至起飞点，掷得5点时不能再掷,掷得6点可以再掷骰子一次，确定棋子的前进步数。</p>"
            "<h3>连投奖励</h3>"
            "<p>在游戏进行过程中，掷得6点的游戏者可以连续投掷骰子，直至显示点数不是6点或游戏结束。</p>"
            "<h3>迭子</h3>"
            "<p>己方的棋子走至同一格内，可迭在一起，这类情况称为“迭子”。敌方的棋子不能在迭子上面飞过，经过时需要后退剩余的步数； 当敌方的棋子正好停留在“迭子”上方时，敌方棋子与2架迭子棋子同时返回停机坪。</p>"
            "<h3>撞子</h3>"
            "<p>棋子在行进过程中走至一格时，若已有敌方棋子停留，可将敌方的棋子逐回基地。</p>"
            "<h3>跳子</h3>"
            "<p>棋子在地图行走时，如果停留在和自己颜色相同格子，可以向前一个相同颜色格子作跳跃。</p>"
            "<h3>飞棋</h3>"
            "<p>棋子若行进到颜色相同而有虚线连接的一格，可照虚线箭头指示的路线，通过虚线到前方颜色相同的的一格后，再跳至下一个与棋子颜色相同的格内。</p>"
            "<h3>终点</h3>"
            "<p>“终点”就是游戏棋子的目的地。当玩家有棋子到达本格时候，表示到达终点，不能再控制该棋子。</p>"
            "<p>关于终点，还有以下几种规则：</p>"
            "<ul>"
            "<li>如果玩家扔出的骰子点数无法刚好走到终点处，多出来的点数，向后走相应步数。</li>"
            "<li>在终点区域可以迭子和撞子；在己方终点处撞子后，必须至少再兜一圈才可以到达终点。</li>"
            "</ul>"
            );
        lmainLayout->addWidget(textEdit, 1);

        helpDialog->setAttribute(Qt::WA_DeleteOnClose);
        helpDialog->show();
    });
    mainLayout->addStretch();
    setAllControlsEnabled(false);
}

void ControlPanel::setupConnections()
{
    connect(readyButton,&QPushButton::clicked,this,&ControlPanel::handleReady);

    connect(rollDiceButton,&QPushButton::clicked,this,&ControlPanel::handleRollDice);

    connect(planeButton1, &QPushButton::clicked, this,[this](){ handlePlaneButton(1); });
    connect(planeButton2, &QPushButton::clicked, this,[this](){ handlePlaneButton(2); });
    connect(planeButton3, &QPushButton::clicked, this,[this](){ handlePlaneButton(3); });
    connect(planeButton4, &QPushButton::clicked, this,[this](){ handlePlaneButton(4); });

    connect(flyYesButton, &QPushButton::clicked, this,[this](){ this->handleFlyOver(true); });
    connect(flyNoButton, &QPushButton::clicked, this,[this](){ this->handleFlyOver(false); });
}


void ControlPanel::setGamePhase(GamePhase phase, const QString &message)
{
    serverMessage->setText(message);
    setAllControlsEnabled(false);

    if (phase == WAITING) {
        if (message.contains(tr("等待")) || message.contains(tr("连接到服务器"))) {
            readyButton->setEnabled(true);
        }

    } else if (phase == ROLL_AND_CHOOSE_PLANE) {
        rollDiceButton->setEnabled(true);
        readyButton->setEnabled(false);
        planeButton1->setEnabled(true);
        planeButton2->setEnabled(true);
        planeButton3->setEnabled(true);
        planeButton4->setEnabled(true);
    } else if (phase == CHOOSE_FLY_OVER) {
        flyYesButton->setEnabled(true);
        flyNoButton->setEnabled(true);
        readyButton->setEnabled(false);
    }
    else if (phase == GAME_ENDED) {
        setAllControlsEnabled(false);
        readyButton->setEnabled(false);
    }
}


void ControlPanel::handleReady()
{
    controller->sendReady();
    gameView->showMessage(tr("已发送准备请求，请等待其他玩家..."));
    readyButton->setEnabled(false);
}

void ControlPanel::handleRollDice()
{
    currentDice = QRandomGenerator::global()->bounded(6) + 1;
    gameView->showMessage(tr("你投出的点数是: %1").arg(currentDice));
    rollDiceButton->setEnabled(false);
}

void ControlPanel::handlePlaneButton(int id)
{
    if(currentDice == 0){
        gameView->showMessage(tr("请先投骰子，再选择飞机!"));
        return ;
    }
    controller->sendPlaneOperation(currentDice,id);
    currentDice = 0;
    setAllControlsEnabled(false);
}

void ControlPanel::handleFlyOver(bool yes)
{
    controller->sendFlyOverChoice(yes);
    setAllControlsEnabled(false);

}

void ControlPanel::setAllControlsEnabled(bool enabled)
{
    rollDiceButton->setEnabled(enabled);
    planeButton1->setEnabled(enabled);
    planeButton2->setEnabled(enabled);
    planeButton3->setEnabled(enabled);
    planeButton4->setEnabled(enabled);
    flyYesButton->setEnabled(enabled);
    flyNoButton->setEnabled(enabled);
}




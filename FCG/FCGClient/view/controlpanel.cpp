#include "controlpanel.h"


ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();

}

void ControlPanel::setGamePhase(GamePhase phase, const QString &message)
{
    serverMessage->setText(message);

    if(phase == WAITING) setAllControlsEnabled(false);
    else{
        const bool rollEnabled = (phase == ROLL_AND_CHOOSE_PLANE);
        const bool flyEnabled = (phase == CHOOSE_FLY_OVER);

        rollDiceButton->setEnabled(rollEnabled);
        planeButtons->setExclusive(rollEnabled);
        QList<QAbstractButton*> planeBtns = planeButtons->buttons();
        for(auto btn : planeBtns) btn->setEnabled(rollEnabled);

        flyYesButton->setEnabled(flyEnabled);
        flyNoButton->setEnabled(flyEnabled);
    }
}

void ControlPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    serverMessage = new QLabel(tr("等待服务器连接..."),this);
    serverMessage->setWordWrap(true);
    mainLayout->addWidget(serverMessage);

    //准备按钮
    QGroupBox* readyGroup = new QGroupBox(tr("游戏准备"),this);
    QVBoxLayout* readyLayout = new QVBoxLayout(readyGroup);
    readyLayout->addWidget(readyButton);
    mainLayout->addWidget(readyGroup);

    //骰子按钮
    QGroupBox* diceGroup = new QGroupBox(tr("骰子操作"),this);
    QVBoxLayout* diceLayout = new QVBoxLayout(diceGroup);
    rollDiceButton = new QPushButton(tr("投掷骰子"),diceGroup);
    diceResultLabel = new QLabel("0",diceGroup);
    diceResultLabel->setAlignment(Qt::AlignCenter);
    diceResultLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    diceLayout->addWidget(rollDiceButton);
    diceLayout->addWidget(diceResultLabel);
    mainLayout->addWidget(diceGroup);

    //飞机按钮
    QGroupBox* planeGroup = new QGroupBox(tr("选择飞机"),this);
    QHBoxLayout* planeLayout = new QHBoxLayout(planeGroup);
    planeButtons = new QButtonGroup(planeGroup);
    for(int i=1;i<=4;i++){
        QPushButton* btn = new QPushButton(QString::number(i),planeGroup);
        btn->setCheckable(true);
        planeButtons->addButton(btn,i);
        planeLayout->addWidget(btn);
    }
    mainLayout->addWidget(planeGroup);

    //飞跃按钮
    QGroupBox* flyGroup = new QGroupBox(tr("飞跃操作"),this);
    QHBoxLayout* flyLayout = new QHBoxLayout(flyGroup);
    flyYesButton = new QPushButton(tr("飞跃(Yes)"),flyGroup);
    flyNoButton = new QPushButton(tr("不飞跃(No)"),flyGroup);
    flyLayout->addWidget(flyYesButton);
    flyLayout->addWidget(flyNoButton);

    //帮助按钮
    QPushButton* helpButton = new QPushButton(tr("游戏帮助"),this);
    mainLayout->addWidget(helpButton);
    connect(helpButton,&QPushButton::clicked,this,[this](){
        QMessageBox *messageBox = new QMessageBox(this);
        messageBox->setWindowTitle("游戏帮助");
        messageBox->setTextFormat(Qt::RichText);
        messageBox->setStandardButtons(QMessageBox::NoButton);
        messageBox->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

        QTextEdit* textEdit = new QTextEdit(messageBox);
        textEdit->setReadOnly(true);
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
        QGridLayout* layout = qobject_cast<QGridLayout*>(messageBox->layout());
        layout->addWidget(textEdit,0,0,1,layout->columnCount());

        messageBox->resize(300,200);
        messageBox->setAttribute(Qt::WA_DeleteOnClose);
        messageBox->show();
    });
    mainLayout->addStretch();
    setAllControlsEnabled(false);
}

void ControlPanel::setupConnections()
{
    connect(readyButton,&QPushButton::clicked,this,&ControlPanel::readyClicked);

    connect(rollDiceButton,&QPushButton::clicked,this,[this](){
        currentDice = QRandomGenerator::global()->bounded(6) + 1;
        diceResultLabel->setText(QString::number(currentDice));
        emit rollDiceClicked();
    });

    connect(planeButtons, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),this,
            [this](QAbstractButton *button) {
                if (!button) return;
                int id = planeButtons->id(button);
                if (id == -1) {
                    qWarning() << "按钮未分配有效 ID";
                    return;
                }
                emit planeSelected(id);
    });

    connect(flyYesButton, &QPushButton::clicked, this,[this](){ emit flyOverChoice(true); });
    connect(flyNoButton, &QPushButton::clicked, this,[this](){ emit flyOverChoice(false); });
}

void ControlPanel::setAllControlsEnabled(bool enabled)
{
    rollDiceButton->setEnabled(enabled);
    planeButtons->setExclusive(enabled);
    QList<QAbstractButton*> planeBtns = planeButtons->buttons();
    for(auto btn : planeBtns) btn->setEnabled(enabled);

    flyYesButton->setEnabled(enabled);
    flyNoButton->setEnabled(enabled);
}




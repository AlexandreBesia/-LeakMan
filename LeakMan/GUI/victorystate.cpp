#include "victorystate.h"

// Public constructor(s)
VictoryState::VictoryState(StateStack* stateStack, QMainWindow* window)
    : State(stateStack, window)
{
    QLabel *lbl1 = new QLabel("Bravo, vous avez enfin pu atteindre les toilettes");
    addNbDeath(lbl1);
    QPushButton *btnNextLevel = new QPushButton("Niveau suivant");
    QPushButton *btnLeave = new QPushButton("Quitter");

    // big title
    lbl1->setGeometry(0, height() * 0.15, width(), height() * 0.5);
    lbl1->setFont(QFont("Arial", 40));
    lbl1->setAlignment(Qt::AlignHCenter);

    // buttons
    btnNextLevel->setGeometry(width() * 0.4, height() * 0.4, width() * 0.2, height() * 0.08);
    btnLeave->setGeometry(width() * 0.4, height() * 0.85, width() * 0.2, height() * 0.08);

    // add them to the scene
    mScene->addWidget(lbl1);
    mScene->addWidget(btnNextLevel);
    mScene->addWidget(btnLeave);

    // connect the buttons
    connect(btnNextLevel, &QPushButton::clicked, this, &VictoryState::nextLevel);
    connect(btnLeave, &QPushButton::clicked, this, &VictoryState::leave);
}

// Public virtual method(s)
bool VictoryState::update(qreal dt)
{
    return false;
}

void VictoryState::nextLevel()
{
    requestStateRemove(StateID::Victory);
    requestStatePush(StateID::Calibration);
}

void VictoryState::leave()
{
    requestStateRemove(StateID::Victory);
    requestStatePush(StateID::Menu);
}

/**
 * @brief VictoryState::addNbDeath Adds number of player's death to text
 * @param lbl
 */
void VictoryState::addNbDeath(QLabel *lbl)
{
    LevelState* levelState= dynamic_cast<LevelState*>(requestState(StateID::LevelWindow));
    if (levelState)
    {
        int levelNbDeaths = levelState->getDeath() - 1;
        lbl->setText(lbl->text() + "\naprès " + QString::number(levelNbDeaths) + " morts");

        requestStateRemove(StateID::LevelWindow);
    }
}

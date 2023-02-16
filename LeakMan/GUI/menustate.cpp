#include "menustate.h"

// Public constructor(s)
MenuState::MenuState(StateStack* stateStack, QMainWindow* window)
    : State(stateStack, window)
{
    QLabel *lbl1 = new QLabel("");
    QPixmap image("://img//LeakMan_LOGO.png");

    QPushButton *btnStory = new QPushButton("Mode histoire");
    QPushButton *btnLevel = new QPushButton("Choisir un niveau");
    QPushButton *btnEditor = new QPushButton("Éditeur de niveau");
    QPushButton *btnCredits = new QPushButton("Crédits");
    QPushButton *btnLeave = new QPushButton("Quitter");

    // big title
    lbl1->setPixmap(image);
    lbl1->setGeometry(0, height() * 0.15, width(), height() * 0.5);
    lbl1->setAlignment(Qt::AlignHCenter);

    // buttons
    btnStory->setGeometry(width() * 0.4, height() * 0.4, width() * 0.2, height() * 0.08);
    btnLevel->setGeometry(width() * 0.4, height() * 0.5, width() * 0.2, height() * 0.08);
    btnEditor->setGeometry(width() * 0.4, height() * 0.6, width() * 0.2, height() * 0.08);
    btnCredits->setGeometry(width() * 0.4, height() * 0.7, width() * 0.2, height() * 0.08);
    btnLeave->setGeometry(width() * 0.4, height() * 0.85, width() * 0.2, height() * 0.08);


    // add them to the scene
    mScene->addWidget(lbl1);
    mScene->addWidget(btnStory);
    mScene->addWidget(btnLevel);
    mScene->addWidget(btnEditor);
    mScene->addWidget(btnCredits);
    mScene->addWidget(btnLeave);

    // Disable unused button
    //btnLevel->setEnabled(false);

    // connect the buttons
    connect(btnStory, &QPushButton::clicked, this, &MenuState::story);
    connect(btnLevel, &QPushButton::clicked, this, &MenuState::level);
    connect(btnEditor, &QPushButton::clicked, this, &MenuState::editor);
    connect(btnCredits, &QPushButton::clicked, this, &MenuState::credits);
    connect(btnLeave, &QPushButton::clicked, this, &MenuState::leave);

    }

// Public virtual method(s)
bool MenuState::update(qreal dt)
{
    return false;
}

void MenuState::story()
{
    requestStateRemove(StateID::Menu);
    requestStatePush(StateID::Calibration);
}

void MenuState::level()
{
    requestStateRemove(StateID::Menu);
    requestStatePush(StateID::Selector);
}

void MenuState::editor()
{
    requestStateRemove(StateID::Menu);
    requestStatePush(StateID::Editor);
}

void MenuState::credits()
{
    requestStateRemove(StateID::Menu);
    requestStatePush(StateID::Credits);
}

void MenuState::leave()
{
    requestStateClear();
}

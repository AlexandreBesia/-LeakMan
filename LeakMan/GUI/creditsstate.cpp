#include "creditsstate.h"

CreditsState::CreditsState(StateStack* stateStack, QMainWindow* window)
    : State(stateStack, window)
{

    lblArcLogo = new QLabel();
    QImage arcLogo;
    QPixmap pmArcLogo;
    arcLogo.load("://img//hearc_logo.png");
    QPixmap pmTemp = QPixmap::fromImage(arcLogo);
    pmArcLogo = pmTemp;
    lblArcLogo->setPixmap(pmArcLogo.scaled(300, 300, Qt::KeepAspectRatio));
    lblArcLogo->adjustSize();
    lblArcLogo->setGeometry(width() * 0.1, height() * 0.05, lblArcLogo->width(), lblArcLogo->height());

    lblTitre = new QLabel("Haute-École ARC (All rights reserved)");
    lblTitre->setGeometry(0, height() * 0.2, width(), height() * 0.4);
    lblTitre->setFont(QFont("Arial", 30));
    lblTitre->setAlignment(Qt::AlignHCenter);

    lblDevs = new QLabel("Développeurs :\nMayer Sarah\nBesia Alexandre\nGleglakos Filippos\nGuyaz Loïc");
    lblDevs->setGeometry(0, height() * 0.4, width(), height() * 0.4);
    lblDevs->setFont(QFont("Arial", 30));
    lblDevs->setAlignment(Qt::AlignHCenter);

    // buttons
    btnLeave = new QPushButton("Retour");
    btnLeave->setGeometry(width() * 0.35, height() * 0.8, width() * 0.3, height() * 0.12);
    btnLeave->setFont(QFont("Arial", 30));

    // add them to the scene
    mScene->addWidget(lblArcLogo);
    mScene->addWidget(lblTitre);
    mScene->addWidget(lblDevs);
    mScene->addWidget(btnLeave);

    // connect the buttons
    connect(btnLeave, &QPushButton::clicked, this, &CreditsState::leave);
}

void CreditsState::leave()
{
    requestStateRemove(StateID::Credits);
    requestStatePush(StateID::Menu);
}

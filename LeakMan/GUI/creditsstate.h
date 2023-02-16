#pragma once

#include <QtWidgets>
#include <QGraphicsItem>

#include "state.h"
#include "stateid.h"

class CreditsState : public State
{
public:
    CreditsState(StateStack* stateStack, QMainWindow* window);

private:
    void leave();

    QLabel *lblTitre, *lblArcLogo, *lblDevs;
    QPushButton *btnLeave;

};

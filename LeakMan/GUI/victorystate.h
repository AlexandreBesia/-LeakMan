#pragma once

#include "state.h"
#include "stateid.h"
#include "levelstate.h"
#include <QtWidgets>
#include <QGraphicsItem>

class VictoryState : public State
{
public:
    // Public constructor(s)
    VictoryState(StateStack* stateStack, QMainWindow* window);

    // Public virtual method(s)
    virtual bool update(qreal dt) override final;

    void nextLevel();
    void leave();

private:
    void addNbDeath(QLabel *);
};

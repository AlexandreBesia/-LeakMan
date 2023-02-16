#pragma once

#include "state.h"
#include "stateid.h"
#include <QtWidgets>
#include <QGraphicsItem>

class MenuState : public State
{
public:
    // Public constructor(s)
    MenuState(StateStack* stateStack, QMainWindow* window);

    // Public virtual method(s)
    virtual bool update(qreal dt) override final;

    void story();
    void level();
    void editor();
    void credits();
    void leave();
};

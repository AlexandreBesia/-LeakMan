#include "state.h"

#include <QScreen>

#include "statestack.h"

// Public virtual method(s)
bool State::update(qreal dt)
{
    return true;
}

// Protected constructor(s)
State::State(StateStack* stateStack, QMainWindow* window)
    : QGraphicsView(stateStack)
    , mScene(new QGraphicsScene(this))
    , mWindow(window)
    , mStateStack(stateStack)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
    setStyleSheet("background: transparent"); // permits multi-state drawing

    QSizeF availableSize = screen()->availableSize();
    mWindow->showMaximized();
    setFixedSize(availableSize.width(), availableSize.height());

    setScene(mScene);
    mScene->setSceneRect(0, 0, availableSize.width(), availableSize.height());
}

// Protected method(s)
void State::requestStatePush(uint32_t stateId)
{
    mStateStack->push(stateId);
}

void State::requestStateRemove(uint32_t stateId)
{
    mStateStack->remove(stateId);
}

void State::requestStateClear()
{
    mStateStack->clear();
}

State* State::requestState(uint32_t stateId)
{
    return mStateStack->get(stateId);
}

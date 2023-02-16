#include <statestack.h>

#include <QApplication>
#include <QTime>

// Public constructor(s)
StateStack::StateStack(QApplication* app, QMainWindow* parent)
    : QWidget(parent)
    , mStates()
    , mStateFactories()
    , mPendingQueue()
    , mApplication(app)
    , mWindow(parent)
    , mElapsedTimer()
{
    startTimer(1.f / 120.f * 1000.f);

    mElapsedTimer.start();
}

// Public method(s)
State* StateStack::get(uint32_t stateId) const
{
    auto stateItr = mStates.find(stateId);
    return (stateItr == mStates.end()) ? nullptr : stateItr->second;
}

// Private method(s)
State* StateStack::createState(uint32_t stateId) const
{
    auto factoryItr = mStateFactories.find(stateId);
    Q_ASSERT_X(factoryItr != mStateFactories.end(), "StateStack::createState", "Attempt to create unregistered state. No match found for the state ID provided.");

    return factoryItr->second();
}

void StateStack::removeState(uint32_t stateId)
{
    auto state = mStates.find(stateId);
    if (state != mStates.end()) {
        state->second->deleteLater();
        mStates.erase(state);
    }
}

void StateStack::applyPendingChanges()
{
    while (!mPendingQueue.empty()) {
        const PendingChange& change = mPendingQueue.front();
        switch (change.action)
        {
        case Action::Push:
            mStates.try_emplace(change.stateId, createState(change.stateId));
            mStates[change.stateId]->show();
            break;
        case Action::Remove:
            removeState(change.stateId);
            break;
        case Action::Clear:
            mStates.clear();
            mApplication->quit();
        };

        mPendingQueue.pop();
    }
}

// Private virtual method(s)
void StateStack::timerEvent(QTimerEvent*)
{
    qreal dt = mElapsedTimer.restart() / 1000.0;
    for (auto& state : mStates) {
        if (!state.second->update(dt)) {
            break;
        }
    }

    applyPendingChanges();
}

// PendingChange
    // Constructor(s)
StateStack::PendingChange::PendingChange(Action action, uint32_t stateId)
    : action(action)
    , stateId(stateId)
{
}

#pragma once

#include <map>
#include <unordered_map>
#include <functional>
#include <queue>

#include <QMainWindow>
#include <QGraphicsView>
#include <QElapsedTimer>

#include "state.h"

/**
 * \brief The class used to manage the State instances.
 */
class StateStack : public QWidget
{
    Q_OBJECT

public:
    // Public enum(s)
    /**
     * \brief The enumeration indicating the action to apply to a registered state.
     */
    enum class Action {
        Push,
        Remove,
        Clear
    };

public:
    // Public constructor(s)
    StateStack(QApplication* app, QMainWindow* parent);
    StateStack(const StateStack&) = delete;
    StateStack(StateStack&&) = delete;

    // Public operator(s)
    StateStack& operator=(const StateStack&) = delete;
    StateStack& operator=(StateStack&&) = delete;

    // Public method(s)
    /*!
     * \brief Creates the association between the type and identifier provided.
     * \param[in] stateId The unique identifier to associate with the type.
     */
    template <class T>
    void registerState(uint32_t stateId);

    /*!
     * \brief Retrieves the base pointer to the State associated to the identifier provided.
     * \param stateId The unique identifier associated to the state to retrieve.
     * \return The State requested or nullptr if it wasn't found.
     */
    [[nodiscard]] State* get(uint32_t stateId) const;
    /*!
     * \brief Enqueues a push command for the state associated to the identifier provided.
     * \param stateId The unique identifier of the state to push.
     */
    inline void push(uint32_t stateId) { mPendingQueue.emplace(Action::Push, stateId); }
    /*!
     * \brief Enqueues a remove command for the state associated to the identifier provided.
     * \param stateId The unique identifier of the state to remove.
     */
    inline void remove(uint32_t stateId) { mPendingQueue.emplace(Action::Remove, stateId); }
    /*!
     * \brief Enqueues a clear command to remove all pushed states.
     */
    inline void clear() { mPendingQueue.emplace(Action::Clear); }
private:
    // Private method(s)
    /*!
     * \brief Uses the factory to instantiate the state of the identifier provided.
     * \param stateId The unique identifier of the state to create.
     * \return The newly created State instance.
     */
    [[nodiscard]] State* createState(uint32_t stateId) const;
    /*!
     * \brief Attempts to remove the pushed state assoiciated to the identifier provided.
     * \param stateId The unique identifier of the state to remove.
     */
    void removeState(uint32_t stateId);
    /*!
     * \brief Applies all requests enqueued by the State instances.
     */
    void applyPendingChanges();

    // Private virtual method(s)
    /*!
     * \brief Commands the State instances to update their elements.
     */
    virtual void timerEvent(QTimerEvent*) override;

private:
    // Private struct(s)
    /**
     * \brief The struct representing a pending action to apply to a registered State.
     * These actions aren't instantly applied as a State instance may want to remove itself.
     */
    struct PendingChange
    {
        // Member(s)
        Action   action; //!< The action to apply to a registered state.
        uint32_t stateId;

        // Constructor(s)
        explicit PendingChange(Action action, uint32_t stateId = 0);
    };

private:
    // Private member(s)
    std::map<uint32_t, State*>                            mStates;         //!< The pushed states
    std::unordered_map<uint32_t, std::function<State*()>> mStateFactories; //!< The registered states which are created once a push request is made
    std::queue<PendingChange>                             mPendingQueue;   //!< The enqueued requests made by the state instances
    QApplication*                                         mApplication;
    QMainWindow*                                          mWindow;
    QElapsedTimer                                         mElapsedTimer;   //!< The timer used to calculate the delta time
};

// Public method(s)
template <class T>
void StateStack::registerState(uint32_t stateId)
{
    static_assert(std::is_base_of_v<State, T>, "StateStack::registerState - T provided has to inherit from State.");
    Q_ASSERT_X(mStateFactories.find(stateId) == mStateFactories.end(), "StateStack::registerState", "The state identifier provided is already in use.");

    // Associate the identifier with the state instantiation
    mStateFactories[stateId] = [this]() {
        return new T(this, dynamic_cast<QMainWindow*>(parentWidget()));
    };
}

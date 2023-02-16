#pragma once

#include <QGraphicsView>
#include <QMainWindow>

// Forward declaration(s)
class StateStack;

/*!
 * \brief The class used to separate different views within the game and holds its respective elements.
 */
class State : public QGraphicsView
{
    Q_OBJECT

public:
    // Public constructor(s)
    State(const State&) = delete;
    State(State&&) = delete;
    virtual ~State() = default;

    // Public operator(s)
    State& operator=(const State&) = delete;
    State& operator=(State&&) = delete;

    // Public virtual method(s)
    /*!
     * \brief Updates the state's elements every frame.
     * \param dt The time difference between this frame and the previous one.
     * \return True to allow the other states to update their elements, false otherwise.
     */
    virtual bool update(qreal dt);

protected:
    // Protected constructor(s)
    State(StateStack* stateStack, QMainWindow* window);

    // Protected method(s)
    /*!
     * \brief Requests the StateStack to push a new state.
     * \details The request is placed into a queue which will be polled on the next frame.
     * \param stateId The identifier associated to a state.
     */
    void requestStatePush(uint32_t stateId);
    /*!
     * \brief Requests the StateStack to remove a speficic state.
     * \details The request is placed into a queue which will be polled on the next frame.
     * \param stateId The identifier associated to a state.
     */
    void requestStateRemove(uint32_t stateId);
    /*!
     * \brief Requests the StateStack to remove all states.
     * \details The request is placed into a queue which will be polled on the next frame.
     */
    void requestStateClear();
    /*!
     * \brief Requests the StateStack to retrieve a specific state.
     * \note The state requested must be pushed onto the stack prior to calling this method.
     * \param stateId The identifier associated to a state.
     * \return The pointer to the State requested or nullptr if the state hasn't been pushed.
     */
    [[nodiscard]] State* requestState(uint32_t stateId);

protected:
    // Protected member(s)
    QGraphicsScene* mScene;
    QMainWindow*    mWindow;
private:
    // Private member(s)
    StateStack*     mStateStack;
};

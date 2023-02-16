#pragma once

#include "state.h"

// Forward declaration(s)
class Level;

/*!
 * \brief The class used in the Level Selection menu.
 */
class LevelSelectionState : public State
{
    Q_OBJECT

public:
    // Public constructor(s)
    LevelSelectionState(StateStack* stateStack, QMainWindow* window);

    // Public static method(s)
    /*!
     * \brief Retrieves the level filename chosen by the user in the selection menu.
     * \note The stored filename will be cleared once this method is called.
     * \return The chosen level's filename.
     */
    [[nodiscard]] static QString acceptLevelFilename() noexcept;
private:
    // Private method(s)
    /*!
     * \brief Retrieves the levels, paints them onto an icon and setups them up in a grid layout.
     */
    void setupLevelGrid();
    /*!
     * \brief Paints the level into a pixmap and converts it into an icon used to set to a button.
     * \param level The level to paint.
     * \return The icon containing the level.
     */
    QIcon levelToIcon(Level& level);

private slots:
    void backToMenu();

private:
    // Private static member(s)
    static QString selectedLevelFilename;
};

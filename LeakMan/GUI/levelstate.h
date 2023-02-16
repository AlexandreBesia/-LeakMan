#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QTimer>

#include "state.h"
#include "stateid.h"
#include "../notGUI/level.h"
#include "stickman.h"

class LevelState : public State
{
public:
    // Public constructor(s)
    LevelState(StateStack* stateStack, QMainWindow* window);

    // Public method(s)
    QRectF stickmanGeometry();
    QPointF levelSize();
    inline Stickman *getStickman() const noexcept { return mStickman; }
    inline Level* getLevel() const noexcept { return mLevel; }
    inline int getDeath() const noexcept { return nbDeath; }

    // Public virtual method(s)
    virtual bool update(qreal dt) override final;

private:
    // Private method(s)
    void resetStickman();
    /*!
     * \brief Attempts to load either the level manually selected in the level selector
     * or the next level from the levels found in the "levels/" directory.
     */
    void loadLevel();

private:
    // Private static member(s)
    static QStringList levelsAvailable; //!< The static level filenames retrieved

    // Private member(s)
    QString   mLevelFilename; //!< The filename of the level to load
    Level*    mLevel;         //!< The level loaded
    Stickman* mStickman;

    int nbDeath;
};

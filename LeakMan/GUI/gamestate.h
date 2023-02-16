#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QColor>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "state.h"
#include "stateid.h"
#include "notGUI/block.h"
#include "gamewindow.h"
#include "levelstate.h"
#include "calibrationstate.h"
#include "notGUI/block.h"

class GameState : public State
{
public:
    // Public constructor(s)
    GameState(StateStack* stateStack, QMainWindow* window);

    // Public virtual method(s)
    virtual bool update(qreal dt) override final;

    void quit();

private:
    bool isQuit;
    int cptLastColor;
    std::vector<QPointF> invalidPositions;

    GameWindow w;

    LevelState *levelState;

    void colorColliding();
    void colorCollidingDebug();
    QLabel* lblDeath;
};

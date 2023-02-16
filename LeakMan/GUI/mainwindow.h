#pragma once

#include <QMainWindow>
#include <qdebug.h>
#include <QKeyEvent>
#include <QLabel>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "statestack.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Public constructor(s)
    explicit MainWindow(QApplication* app);

    // Public method(s)
    template <class T>
    void registerState(uint32_t stateId);
    void pushState(uint32_t stateId);

private:
    // Private member(s)
    StateStack* mStateStack;
};

// Public method(s)
template <class T>
void MainWindow::registerState(uint32_t stateId)
{
    static_assert(std::is_base_of_v<State, T>, "MainWindow::registerState - T provided has to inherit from State.");
    mStateStack->registerState<T>(stateId);
}

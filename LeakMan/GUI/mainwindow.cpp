#include "mainwindow.h"

// Public constructor(s)

/**
 * @brief MainWindow::MainWindow Displays Leakman
 * @param app
 */
MainWindow::MainWindow(QApplication* app)
    : QMainWindow()
    , mStateStack(new StateStack(app, this))
{
    setCentralWidget(mStateStack);
    setWindowTitle("LeakMan");

    setWindowIcon(QIcon("://img//Stickman_Unique.ico"));
}

// Public method(s)

/**
 * @brief MainWindow::pushState Push the state wanted
 * @param stateId
 */
void MainWindow::pushState(uint32_t stateId)
{
    Q_ASSERT_X(mStateStack, "MainWindow::pushState", "State stack is null.");
    mStateStack->push(stateId);
}


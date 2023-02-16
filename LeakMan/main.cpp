#include "GUI/calibrationwindow.h"
#include "GUI/calibrationstate.h"
#include "GUI/mainwindow.h"
#include "GUI/stateid.h"
#include "GUI/menustate.h"
#include "GUI/editorstate.h"
#include "GUI/gamestate.h"
#include "GUI/levelselectionstate.h"
#include "GUI/creditsstate.h"
#include "GUI/levelstate.h"
#include "GUI/actor.h"
#include "GUI/victorystate.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window(&app);

    // Register all the states
    window.registerState<MenuState>(StateID::Menu);
    window.registerState<EditorState>(StateID::Editor);
    window.registerState<GameState>(StateID::Game);
    window.registerState<LevelSelectionState>(StateID::Selector);
    window.registerState<CreditsState>(StateID::Credits);
    window.registerState<CalibrationState>(StateID::Calibration);
    window.registerState<LevelState>(StateID::LevelWindow);
    window.registerState<VictoryState>(StateID::Victory);

    window.pushState(StateID::Menu);
    //window.pushState(StateID::LevelWindow);

    window.show();
    return app.exec();
}

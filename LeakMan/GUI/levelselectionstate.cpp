#include "levelselectionstate.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyleOptionGraphicsItem>

#include "notGUI/level.h"
#include "notGUI/block.h"
#include "stateid.h"

// Static member(s)
QString LevelSelectionState::selectedLevelFilename = "";

// Public constructor(s)
LevelSelectionState::LevelSelectionState(StateStack* stateStack, QMainWindow* window)
    : State(stateStack, window)
{
    setupLevelGrid();
}

// Public static method(s)
QString LevelSelectionState::acceptLevelFilename() noexcept
{
    QString tmp = selectedLevelFilename;
    selectedLevelFilename.clear();

    return tmp;
}

// Private method(s)
void LevelSelectionState::setupLevelGrid()
{
    const int LEVELS_PER_ROW = 10;

    // 80% of the width is for the levels, 15% for borders
    int blockWidth = width() * 0.8 / LEVELS_PER_ROW;
    int widthSpacing = width() * 0.15 / (LEVELS_PER_ROW + 1);

    // head layout
    QHBoxLayout *hLayout = new QHBoxLayout();
    QPushButton* btnLeave = new QPushButton("Back to menu");
    hLayout->addWidget(btnLeave);
    hLayout->addWidget(new QLabel("Select a level"));
    connect(btnLeave, &QPushButton::clicked, this, &LevelSelectionState::backToMenu);

    // layout containing the levels
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(widthSpacing);

    // Deserialize the levels and paint them onto an icon
    QStringList levelFilenames = Level::getLevelFilenames();
    for(qsizetype i = 0; i < levelFilenames.size(); ++i)
    {
        Level* level = Level::deserializeFrom("levels/" + levelFilenames[i]);
        if (level) {
            QPushButton* btnLevel = new QPushButton();
            btnLevel->setIcon(levelToIcon(*level));
            btnLevel->setIconSize(QSize(blockWidth, blockWidth));
            connect(btnLevel, &QPushButton::clicked, this, [=]() {
                selectedLevelFilename = levelFilenames[i];
                requestStateRemove(StateID::Selector);
                requestStatePush(StateID::Calibration);
            });

            gridLayout->addWidget(btnLevel, floor(i / LEVELS_PER_ROW), i % LEVELS_PER_ROW);
        }
    }

    // scroll area, does not work
    QScrollArea *scroll = new QScrollArea();
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // main layout
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout);
    vLayout->addLayout(gridLayout);
    vLayout->addStretch();

    scroll->setLayout(vLayout); // add the scrollarea, but does not work
    mScene->addWidget(scroll);
}

// Private method(s)
QIcon LevelSelectionState::levelToIcon(Level& level)
{
    // Prepare pixmap
    QPixmap pixmap(level.boundingRect().size().toSize());
    pixmap.fill(Qt::transparent);

    // Paint grid and blocks onto the pixmap
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QStyleOptionGraphicsItem opt;
    level.getGrid()->paint(&painter, &opt, nullptr);
    for (Block* block : level.getBlocks()) {
        block->paintForSelection(&painter, &opt);
    }

    return QIcon(pixmap);
}

// Slot(s)
void LevelSelectionState::backToMenu()
{
    requestStateRemove(StateID::Selector);
    requestStatePush(StateID::Menu);
}

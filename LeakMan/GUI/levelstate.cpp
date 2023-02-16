#include "levelstate.h"

#include <QElapsedTimer>

#include "notGUI/block.h"
#include "levelselectionstate.h"
#include "stateid.h"

// Static member(s)
QStringList LevelState::levelsAvailable = QStringList();

// Public constructor(s)
LevelState::LevelState(StateStack* stateStack, QMainWindow* window)
    : State(stateStack, window)
    , mLevelFilename("")
    , mLevel(nullptr)
    , mStickman()
    , nbDeath(0)
{
    loadLevel();

    mStickman = new Stickman();
    mScene->addItem(mStickman);

    resetStickman();
}

// Public virtual method(s)
bool LevelState::update(qreal dt)
{
    QVector<Block*> blocks = mLevel->getBlocks();

    mStickman->tickUpdate(dt);
    for (Block* block : blocks) {
        block->tickUpdate(dt);
    }

    resetStickman();

    for (size_t i = 0; i < blocks.size(); ++i) {
        if (blocks[i]->isMarkedForRemoval()) {
            mLevel->removeBlock(blocks[i]);
        }
    }

    return true;
}

// Private method(s)
void LevelState::resetStickman()
{
    if (mStickman->isMarkedForReset() || !mStickman->mapRectToScene(mStickman->boundingRect()).intersects(mLevel->mapRectToScene(mLevel->boundingRect()))) {
        if (mLevel) {
            mScene->removeItem(mLevel);
            delete mLevel;
        }
        qint32 tileSize = qMin(window()->size().width(), window()->size().height());
        mLevel = Level::deserializeFrom("levels/" + mLevelFilename, QSize(tileSize, tileSize));
        mScene->addItem(mLevel);

        QVector<Block*> startBlocks = mLevel->getBlocks(Block::TypeBlock::StartBlock);
        if (!startBlocks.empty()) {
            mStickman->setPos(startBlocks[0]->pos() + QPointF(10.f, -10.f));
        }
        mStickman->markForReset(false);
        mStickman->reset();
        nbDeath++;
    }

    if(mStickman->isEnded())
    {
        //requestStateRemove(StateID::LevelWindow); // uncomment it only if not used anymore in VictoryState
        requestStateRemove(StateID::Game);
        requestStatePush(StateID::Victory);
    }
}

QRectF LevelState::stickmanGeometry()
{
    return (mStickman) ? mStickman->mapRectFromScene(mStickman->boundingRect()) : QRectF();
}

QPointF LevelState::levelSize()
{
    QRectF tmp = mLevel->mapRectToScene(mLevel->boundingRect());
    return QPointF(tmp.width(), tmp.height());
}

void LevelState::loadLevel()
{
    mLevelFilename = LevelSelectionState::acceptLevelFilename();

    // Load next random level if a level wasn't manually chosen
    if (mLevelFilename.isEmpty()) {
        if (levelsAvailable.empty()) {
            levelsAvailable = Level::getLevelFilenames();
        }
        qsizetype rndIndex = rand() % levelsAvailable.size();
        mLevelFilename = levelsAvailable[rndIndex];
        levelsAvailable.remove(rndIndex);
    }
}

#include "levelstate.h"

#include <QElapsedTimer>

#include "block.h"

// Public constructor(s)
LevelState::LevelState(StateStack* stateStack, QMainWindow* window)
    : State(stateStack, window)
    , mLevel(Level::deserializeFrom("levels/texturelevel.lml"))
    , mStickman()
{
    QPointF tilePos = mLevel->getGrid()->getTilePosition(5, 4);
    mLevel->addBlock(tilePos.x(), tilePos.y(), Block::TypeBlock::Jump);
    mScene->addItem(mLevel);

    QVector<Block*>& blocks = mLevel->getBlocks();

    auto startBlock = std::find_if(blocks.begin(), blocks.end(), [](Block* block) {
        return block->getType() == Block::TypeBlock::StartBlock;
    });

    QPointF stickmanStartPosition;
    if (startBlock != blocks.end()) {
        stickmanStartPosition = (*startBlock)->pos();
    }
    mStickman = new Stickman(stickmanStartPosition + QPointF(10.f, -10.f));
    mScene->addItem(mStickman);

    mStickman->setPixmap(QPixmap("://img/spikes_64x64.png").scaledToWidth(64));
}

// Public virtual method(s)
bool LevelState::update(qreal dt)
{
    static qreal timer;
    if ((timer += dt) > 3.0) {
        mStickman->tickUpdate(dt);
    }
    return true;
}

QRectF LevelState::stickmanGeometry()
{
    if(mStickman != nullptr)
    {
        qDebug() << mLevel->mapRectToScene(mLevel->boundingRect());
        return mStickman->mapRectFromScene(mStickman->boundingRect());
    }
}

QPointF LevelState::levelSize()
{
    QRectF tmp = mLevel->mapRectToScene(mLevel->boundingRect());
    return QPointF(tmp.width(), tmp.height());
}

Stickman* LevelState::getStickman()
{
    return mStickman;
}

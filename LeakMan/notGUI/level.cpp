#include "level.h"
#include "block.h"

#include <QGraphicsScene>
#include <QFile>
#include <QDir>
#include <QMessageBox>

// Public constructor(s)
Level::Level(const QSize& tileSize, qint32 columns, qint32 rows, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , mBlocks()
    , mGrid(new LevelGrid(tileSize, columns, rows, this))
{
}

// Public method(s)
void Level::addBlock(qreal x, qreal y, quint32 identifier)
{
    Block* block = createBlock(identifier);
    block->setPos(x, y);
}

void Level::removeBlock(Block* block)
{
    auto blockItr = std::find(mBlocks.begin(), mBlocks.end(), block);
    if (blockItr != mBlocks.end()) {
        (*blockItr)->setParentItem(nullptr);
        delete *blockItr;
        mBlocks.erase(blockItr);
    }
}

void Level::removeBlockAt(qreal x, qreal y)
{
    auto block = std::find_if(mBlocks.begin(), mBlocks.end(), [=](const Block* block) {
        const QPointF DISTANCE_VEC = QPointF(x, y) - block->pos();
        return qAbs(qHypot(DISTANCE_VEC.x(), DISTANCE_VEC.y())) < 0.01f;
    });

    if (block != mBlocks.end()) {
        (*block)->setParentItem(nullptr);
        delete *block;
        mBlocks.erase(block);
    }
}

QVector<Block*> Level::getBlocks(uint32_t type)
{
    QVector<Block*> blocksFound;
    for (Block* block : mBlocks) {
        if (block->getType() & type) {
            blocksFound.push_back(block);
        }
    }

    return (blocksFound.empty()) ? mBlocks : blocksFound;
}

// Public virtual method(s)
QRectF Level::boundingRect() const
{
    return mGrid->boundingRect();
}

void Level::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

// Public static method(s)
Level* Level::deserializeFrom(const QString& filename, const QSize& gridSize)
{
    Level* level = nullptr;

    QFile data(filename);
    if (data.open(QFile::ReadOnly)) {
        QDataStream in(&data);

        quint32 magicNumber;
        in >> magicNumber;
        if (magicNumber == 0xA4F955EF) {
            in.setVersion(QDataStream::Qt_6_1);

            level = new Level();
            level->mGrid = LevelGrid::deserialize(in, level);
            if (gridSize.width() != 0 && gridSize.height() != 0) {
                level->mGrid->setSize(gridSize);
            }
            level->deserializeBlocks(in);
        }

        data.close();
    }

    return level;
}

QStringList Level::getLevelFilenames()
{
    QStringList filenames;

    QDir directory("levels");
    if (directory.exists()) {
        filenames = directory.entryList(QStringList() << "*.lml", QDir::Files);
    }

    return filenames;
}

// Friend operator(s)
QDataStream& operator<<(QDataStream& out, Level& level)
{
    out << static_cast<quint32>(0xA4F955EF);
    out.setVersion(QDataStream::Qt_6_1);

    out << *level.mGrid;

    quint32 blockCount = level.mBlocks.size();
    out << blockCount;

    for (Block* block : level.mBlocks) {
        quint32 identifier = block->getType();
        out << identifier;

        Tile closestTile = level.mGrid->getClosestTile(block->pos());
        out << closestTile.column << closestTile.row;
    }
    return out;
}

// Private constructor(s)
Level::Level()
    : mGrid(nullptr)
{
}

// Private method(s)
void Level::deserializeBlocks(QDataStream& in)
{
    quint32 actorCount;
    in >> actorCount;

    for (quint32 i = 0; i < actorCount; ++i) {
        quint32 identifier;
        in >> identifier;

        quint32 columnIdx, rowIdx;
        in >> columnIdx >> rowIdx;

        Block* block = createBlock(identifier);
        block->setPos(mGrid->getTilePosition(columnIdx, rowIdx));
    }
}

Block* Level::createBlock(uint32_t identifier)
{
    const QSize& tileSize = mGrid->getTileSize();
    Block::TypeInfo& typeInfo = Block::getTypeInfo(identifier);

    // Scale the pixmap if it doesn't conform to the grid's tile size
    if (typeInfo.pixmap.size() != tileSize) {
        typeInfo.pixmap = typeInfo.pixmap.scaled(tileSize.width(), tileSize.height());
    }

    Block* block = new Block(identifier, this);
    block->setPixmap(typeInfo.pixmap);
    mBlocks.push_back(block);

    return block;
}

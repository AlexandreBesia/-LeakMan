#include "levelgrid.h"

#include <QPainter>

// Public constructor(s)
LevelGrid::LevelGrid(const QSize& tileSize, qint32 columns, qint32 rows, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , mTileSize(tileSize)
    , mColumnCount(columns)
    , mRowCount(rows)
{
    Q_ASSERT_X(tileSize.width() > 0 && tileSize.height() > 0, "LevelGrid::LevelGrid", "The tile dimensions need to be greater than zero.");
    Q_ASSERT_X(columns > 0 && rows > 0, "LevelGrid::LevelGrid", "Number of columns and rows need to be greater than zero.");

    createLines();
}

// Public method(s)
void LevelGrid::setSize(const QSize& gridSize)
{
    Q_ASSERT_X(gridSize.width() > 0 && gridSize.height() > 0, "LevelGrid::setSize", "The grid dimensions need to be greater than zero.");

    mTileSize.setWidth(gridSize.width() / mColumnCount);
    mTileSize.setHeight(gridSize.height() / mRowCount);
    createLines();
}

QPointF LevelGrid::getTilePosition(qint32 column, qint32 row) const
{    
    Q_ASSERT_X(column >= 0 && row >= 0 && column <= mColumnCount && row <= mRowCount, "LevelGrid::getTilePosition", "Out of bounds column/row indices.");

    return QPointF(column * mTileSize.width(), row * mTileSize.height());
}

Tile LevelGrid::getClosestTile(const QPointF& pos) const
{
    auto[posX, column] = findClosestTileAlongAxis(mColumnCount, pos.x(), mTileSize.width());
    auto[posY, row] = findClosestTileAlongAxis(mRowCount, pos.y(), mTileSize.height());

    return Tile({ QPointF(posX, posY), mTileSize, column, row });
}

// Public virtual method(s)
QRectF LevelGrid::boundingRect() const
{
    return QRectF(getTilePosition(0, 0), getTilePosition(mColumnCount, mRowCount));
}

void LevelGrid::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    for (const QLine& line : mLines) {
        painter->drawLine(line);
    }
}

// Public static method(s)
LevelGrid* LevelGrid::deserialize(QDataStream& in, QGraphicsItem* parent)
{
    qint32 tileWidth, tileHeight;
    qint32 columns, rows;
    in >> tileWidth >> tileHeight >> columns >> rows;

    LevelGrid* levelGrid = new LevelGrid(QSize(tileWidth, tileHeight), columns, rows, parent);
    return levelGrid;
}

// Friend operator(s)
QDataStream& operator<<(QDataStream& out, LevelGrid& levelGrid)
{
    const qint32 TILE_WIDTH = static_cast<qint32>(levelGrid.mTileSize.width());
    const qint32 TILE_HEIGHT = static_cast<qint32>(levelGrid.mTileSize.height());

    return out << TILE_WIDTH << TILE_HEIGHT << levelGrid.mColumnCount << levelGrid.mRowCount;
}

// Private method(s)
QPair<qreal, qint32> LevelGrid::findClosestTileAlongAxis(qint32 indexCount, qreal pos, qint32 size) const
{
    qreal tilePos = 0;
    qint32 tileIndex = -1;

    // Find the closest distance and store its corresponding tile index
    qreal distance = std::numeric_limits<qreal>().max();
    for (qint32 i = 0; i <= indexCount; ++i) {
        const qreal NEW_DISTANCE = qAbs(i * size + size / 2.f - pos);
        if (NEW_DISTANCE <= distance) {
            distance = NEW_DISTANCE;
        }
        else {
            tilePos = (i - 1) * size;
            tileIndex = i - 1;
            break;
        }
    }

    return QPair<qreal, qint32>(tilePos, tileIndex);
}

void LevelGrid::createLines()
{
    const qint32 MAX_WIDTH = mColumnCount * mTileSize.width();
    const qint32 MAX_HEIGHT = mRowCount * mTileSize.height();

    mLines.clear();
    mLines.reserve(mColumnCount + mRowCount);
    for (qint32 i = 0; i <= mColumnCount; ++i) {
        mLines.emplace_back(i * mTileSize.width(), 0, i * mTileSize.width(), MAX_HEIGHT);
    }
    for (qint32 i = 0; i <= mRowCount; ++i) {
        mLines.emplaceBack(0, i * mTileSize.height(), MAX_WIDTH, i * mTileSize.height());
    }
}

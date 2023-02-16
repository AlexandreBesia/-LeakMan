#pragma once

#include <QGraphicsItem>

/*!
 * \brief The struct containing the properties of a tile within the LevelGrid.
 */
struct Tile
{
    QPointF position; //!< The fixed position
    QSize   size;     //!< The dimensions
    qint32  column;   //!< The tile's index along the X axis
    qint32  row;      //!< The tile's index along the Y axis
};

/*!
 * \brief The class permitting the querying of tiles' size and fixed position within the grid.
 */
class LevelGrid : public QGraphicsItem
{
public:
    // Public constructor(s)
    /*!
     * \brief Custom constructor used for the level editor.
     * \param[in] tileSize The dimensions of each tile.
     * \param[in] columns The number of horizontal tiles.
     * \param[in] rows The number of vertical tiles.
     * \param[in] parent The element responsible for the LevelGrid's destruction.
     */
    LevelGrid(const QSize& tileSize, qint32 columns, qint32 rows, QGraphicsItem* parent);

    // Public method(s)
    /*!
     * \brief Sets the size of each tile by providing the total grid size.
     * \param[in] gridSize The size of the entire grid.
     */
    void setSize(const QSize& gridSize);

    /*!
     * \brief Retrieves the fixed position of the tile at the indices provided.
     * \param[in] column The index along the X axis.
     * \param[in] row The index along the Y axis.
     * \return The fixed position of the tile situated at the \a column and \a row specified.
     * \sa getClosestTile()
     */
    QPointF getTilePosition(qint32 column, qint32 row) const;
    /*!
     * \brief Retrieves the tile closest to the \a pos provided.
     * \param[in] pos The position in scene-coordinates.
     * \return The closest tile with all of its properties (fixed position, size, column and row indices).
     * \sa getTilePosition()
     */
    Tile getClosestTile(const QPointF& pos) const;

    /*!
     * \brief Retrieves the dimensions of the tiles.
     * \return A QSize containing the tiles' dimensions.
     */
    [[nodiscard]] inline const QSize& getTileSize() const noexcept { return mTileSize; }

    // Public virtual method(s)
    /*!
     * \brief Retrieves the rect formed by the first and last tile positions.
     * \return A QRectF containing the grid's local bounding rect.
     */
    virtual QRectF boundingRect() const override;
    /*!
     * \brief Draws the lines forming the grid.
     */
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    // Public static method(s)
    /*!
     * \brief Creates a LevelGrid from a datastream.
     * \param[in] in The datastream.
     * \param[in] parent The grid's parent, usually a Level instance.
     * \return The LevelGrid instance.
     */
    [[nodiscard]] static LevelGrid* deserialize(QDataStream& in, QGraphicsItem* parent);

    // Friend operator(s)
    /*!
     * \brief Serialization operator.
     * \param[in, out] out The datastream.
     * \param[in] levelGrid The LevelGrid instance to serialize.
     * \return The datastream.
     */
    friend QDataStream& operator<<(QDataStream& out, LevelGrid& levelGrid);
private:
    // Private method(s)
    /*!
     * \brief Helper method used to find the closest tile along a certain axis specified by the parameters.
     * \param[in] indexCount Either the number of columns or the number of rows.
     * \param[in] pos The relative position used to find the closest tile.
     * \param[in] size Either the tile width or the tile height.
     * \return The position and index of the tile closest to \a pos along the specified axis.
     */
    QPair<qreal, qint32> findClosestTileAlongAxis(qint32 indexCount, qreal pos, qint32 size) const;
    /*!
     * \brief Creates the lines which will be used to draw the grid.
     */
    void createLines();

private:
    // Private member(s)
    QVector<QLine> mLines;       //!< The lines that form the grid
    QSize          mTileSize;    //!< The size of each tile
    qint32         mColumnCount; //!< The number of horizontal tiles
    qint32         mRowCount;    //!< The number of vertical tiles
};

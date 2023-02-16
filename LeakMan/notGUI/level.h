#pragma once

#include <QGraphicsItem>
#include <QString>

#include "../GUI/actor.h"
#include "levelgrid.h"

// Forward declaration(s)
class Block;

/*!
 * \brief The class containing the actors/blocks of a level.
 */
class Level : public QGraphicsItem
{
public:
    // Public constructor(s)
    /*!
     * \brief Constucts the level by providing the tiles's size and the number of columns and rows.
     * \details This constructor should primarily be used by the editor as levels can be deserialized using the \see deserializeFrom() static method.
     * \param tileSize The dimensions of the level's tiles.
     * \param columns The number of columns.
     * \param rows The number of rows.
     * \param parent The item responsible for the level's destruction.
     */
    Level(const QSize& tileSize, qint32 columns, qint32 rows, QGraphicsItem* parent = nullptr);

    // Public method(s)
    /*!
     * \brief Adds a block at the coordinates and of the type provided.
     * \note This method always creates a block even if a block is already occupying the same position.
     * \param x The coordinate along the horizontal axis.
     * \param y The coordinate along the vertical axis.
     * \param identifier The Block::TypeBlock bit flags.
     */
    void addBlock(qreal x, qreal y, quint32 identifier);
    /*!
     * \brief Attemps to remove the \a block provided, no effect is the block isn't found.
     * \param block The block to remove
     */
    void removeBlock(Block* block);
    /*!
     * \brief Attempts to remove a block situated at the \a x and \a y coordinates provided.
     * \param x The coordinate along the horizontal axis.
     * \param y The coordinate along the vertical axis.
     */
    void removeBlockAt(qreal x, qreal y);
    /*!
     * \brief Retrieve the blocks associated to the \a type provided.
     * \param[in] type The Block::TypeBlock bit flags.
     * \return List of blocks of the \a type provided or all the blocks if no blocks of that type were found.
     */
    [[nodiscard]] QVector<Block*> getBlocks(uint32_t type = 0);
    /*!
     * \brief Retrieves the LevelGrid which contains the individual tiles' positions and indices.
     * \return The LevelGrid which can be used to find the tiles' fixed positions.
     */
    [[nodiscard]] inline LevelGrid* getGrid() noexcept { return mGrid; }

    // Public virtual method(s)
    /*!
     * \brief Retrieves the rect formed by the level grid.
     * \return A QRectF containing the grid's local bounding rect.
     */
    virtual QRectF boundingRect() const override final;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override final;

    // Public static method(s)
    /*!
     * \brief Constructs a new Level by deserializing the \a filename and an optional grid size.
     * \param[in] filename The file containing the level data.
     * \param[in} gridSize The optional new grid size, (0, 0) by default which is the base grid size.
     * \return The level containing the deserialized data.
     */
    [[nodiscard]] static Level* deserializeFrom(const QString& filename, const QSize& gridSize = QSize(0, 0));
    /*!
     * \brief Retrieves all the filenames of levels in the 'levels/' directory.
     * \return A list of the level filenames found.
     */
    [[nodiscard]] static QStringList getLevelFilenames();

    // Friend operator(s)
    /*!
     * \brief Serialization operator.
     * \param[in, out] out The datastream.
     * \param[in] level The Level instance to serialize.
     * \return The datastream.
     */
    friend QDataStream& operator<<(QDataStream& out, Level& level);
private:
    // Private constructor(s)
    Level();

    // Private method(s)
    /*!
     * \brief Creates the blocks during the deserialization process.
     * \param[in] in The datastream.
     */
    void deserializeBlocks(QDataStream& in);
    /*!
     * \brief Constructs a block based on the \a identifier provided.
     * \param[in] identifier The Block::TypeBlock bit flags specifying the type.
     * \return The block of type \a identifier created.
     */
    Block* createBlock(uint32_t identifier);

private:
    // Private member(s)
    QVector<Block*> mBlocks; //!< The blocks placed within the level
    LevelGrid*      mGrid;   //!< The level grid
};

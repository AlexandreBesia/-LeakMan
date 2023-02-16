#pragma once

#include <QGraphicsPixmapItem>

#include "../GUI/actor.h"
#include "./notGUI/physicscomponent.h"
#include "GUI/stickman.h"

/*!
 * \brief The class that represents a block which can be placed onto the grid.
 */
class Block : public QGraphicsPixmapItem, public Actor
{
public:
    // Public enum(s)
    /*!
     * \brief The enumeration containing the different blocks that can be placed.
     * \details A block can have a type of one or more of the types available using the OR bit flag.
     */
    enum TypeBlock
    {
        Jump        = 1 << 1,   // 0010             = 2
        Reverse     = 1 << 2,   // 0100             = 4
        Run         = 1 << 3,   // 1000             = 8
        Climb       = 1 << 4,   // 0001 0000        = 16
        Ground      = 1 << 5,   // 0010 0000        = 32
        SpikesTrap  = 1 << 6,   // 0100 0000        = 64
        FallingTrap = 1 << 7,   // 1000 0000        = 128
        EndBlock    = 1 << 8,   // 0001 0000 0000   = 256
        StartBlock  = 1 << 9    // 0010 0000 0000   = 512
    };
    /*!
     * \brief The struct containing the information of a block of a certain type.
     */
    struct TypeInfo {
        QString name; //!< The name of the block used in the editor
        QPixmap pixmap; //!< The corresponding pixmap
        bool collidable; //!< Whether the block is collidable with other blocks or the stickman
    };

public:
    // Public constructor(s)
    /*!
     * \brief Constructs a Block by providing a \a type and \a parent.
     * \param[in] type The Block::TypeBlock to assign to the newly created block.
     * \param parent The item responsible for the Block's destruction.
     */
    explicit Block(uint32_t type, QGraphicsItem* parent = nullptr);

    // Public method(s)
    /*!
     * \brief Applies the block's effect to the actor (which is usually the stickman).
     * \param actor The actor to which its PhysicsComponent will be modified.
     */
    void applyEffect(Actor& actor);
    /*!
     * \brief Updates the PhysicsComponent (if one has been added).
     * \param dt The time difference between this frame and the previous one.
     */
    void tickUpdate(qreal dt) override;
    [[nodiscard]] inline uint32_t getType() const noexcept { return mType; }

    /*!
     * \brief Paints the block's pixmap at the block's position.
     * \details Used solely for the level selection.
     */
    void paintForSelection(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr);

    // Public static method(s)
    /*!
     * \brief Retrieves the predefined block information associated to the \a type provided.
     * \param type The Block::TypeBlock using the OR bit flag.
     * \return The associated type information.
     */
    static TypeInfo& getTypeInfo(uint32_t type);
    /*!
     * \brief Retrieves the map of all predefined block information.
     * \details This map will be created when this static method is first called.
     * \return The map of the predefined blocks.
     */
    static std::unordered_map<uint32_t, TypeInfo>& getBlockInfo();

private:
    // Private static member(s)
    static std::unordered_map<uint32_t, TypeInfo> blockInfo;

    // Private member(s)
    uint32_t mType;
};

#include "block.h"

#include <QTimer>
#include <QPainter>

#include "./notGUI/physicscomponent.h"
#include "GUI/stickman.h"

#include <QTimer>

std::unordered_map<uint32_t, Block::TypeInfo> Block::blockInfo = {};

Block::Block(uint32_t type, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , mType(type)
{
    if (type & TypeBlock::FallingTrap) {
        addComponent<PhysicsComponent>();
    }
}

void Block::applyEffect(Actor& actor)
{
    PhysicsComponent* actorPhysics = actor.getComponent<PhysicsComponent>();
    if (actorPhysics) {
        if (mType & TypeBlock::Jump) {
            actorPhysics->applyForce(QVector2D(1.5f, -10.25f));
        }
        if (mType & TypeBlock::Reverse) {
            const QVector2D CURRENT_ACCELERATION = actorPhysics->getAcceleration();
            actorPhysics->setAcceleration(QVector2D(-CURRENT_ACCELERATION.x(), CURRENT_ACCELERATION.y()));
        }
        if (mType & TypeBlock::Run) {
            actorPhysics->applyForce(QVector2D(8.5f, 0.f));
        }
    }

    Stickman* stickman = dynamic_cast<Stickman*>(&actor);
    if (stickman) {
        if (mType & TypeBlock::SpikesTrap) {
            stickman->markForReset(true);
        }
        else if (mType & TypeBlock::EndBlock) {
            stickman->end(true);
        }
    }

    PhysicsComponent* blockPhysics = getComponent<PhysicsComponent>();
    if (blockPhysics) {
        if (mType & TypeBlock::FallingTrap) {
            blockPhysics->setAcceleration(QVector2D(0.f, 5.f));
        }
    }
}

void Block::tickUpdate(qreal dt)
{
    PhysicsComponent* physicsComponent = getComponent<PhysicsComponent>();
    if (physicsComponent) {
        physicsComponent->tickUpdate(dt);
    }
}

void Block::paintForSelection(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(pos().x(), pos().y(), pixmap());
}

Block::TypeInfo& Block::getTypeInfo(uint32_t type)
{
    auto info = getBlockInfo().find(type);
    return (info != blockInfo.end()) ? info->second : blockInfo[TypeBlock::Ground];
}

std::unordered_map<uint32_t, Block::TypeInfo>& Block::getBlockInfo()
{
    // Initialized here as QPixmap instances can't be static
    if (blockInfo.empty()) {
        blockInfo = {
            { Block::TypeBlock::Ground, { "Ground", QPixmap("://img/floor_64x64.png"), true } },
            { Block::TypeBlock::SpikesTrap, { "Spikes", QPixmap("://img/Spikes.png"), true } },
            { Block::TypeBlock::FallingTrap | Block::TypeBlock::Ground, { "Trap floor", QPixmap("://img/Cloud.png"), true } },
            { Block::TypeBlock::EndBlock, { "End", QPixmap("://img/Toilet.png"), true } },
            { Block::TypeBlock::StartBlock, { "Start", QPixmap("://img/Claw.png"), false } },
            { Block::TypeBlock::Jump, { "Jump", QPixmap("://img/jump_64x64.png"), true } },
            { Block::TypeBlock::Run, { "Sprint", QPixmap("://img/sprint_64x64.png"), true } },
            { Block::TypeBlock::Reverse | Block::TypeBlock::Ground, { "Reverse", QPixmap("://img/reverse_64x64.png"), true } }
        };
    }

    return blockInfo;
}

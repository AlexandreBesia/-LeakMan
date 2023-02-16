#include "physicscomponent.h"

#include <QTime>

#include "GUI/actor.h"
#include "block.h"

PhysicsComponent::PhysicsComponent(Actor& associatedActor) noexcept
    : Component(associatedActor)
    , mVelocity(0.f, 0.f)
    , mAcceleration(0.f, 0.f)
{
}

void PhysicsComponent::tickUpdate(qreal dt)
{
    QGraphicsItem* actorItem = dynamic_cast<QGraphicsItem*>(&mAssociatedActor);
    if (actorItem) {
        mVelocity += mAcceleration;
        actorItem->moveBy(mVelocity.x() * dt, mVelocity.y() * dt);
        handleCollisions(*actorItem);
    }
}

// Private method(s)
void PhysicsComponent::handleCollisions(QGraphicsItem& actor)
{
    for (QGraphicsItem* otherItem : actor.collidingItems(Qt::ItemSelectionMode::IntersectsItemShape)) {
        Block* block = dynamic_cast<Block*>(otherItem);
        if (block && Block::getTypeInfo(block->getType()).collidable) {
            if (block->getType() & Block::TypeBlock::Ground) {
                Block* actorBlock = dynamic_cast<Block*>(&mAssociatedActor);
                if (actorBlock && actorBlock->getType() & Block::TypeBlock::FallingTrap) {
                    actorBlock->markForRemoval(true);
                }
                else {
                    handleBlockCollision(actor, *block);
                }
            }
            block->applyEffect(mAssociatedActor);
        }
    }
}

void PhysicsComponent::handleBlockCollision(QGraphicsItem& actor, Block& block)
{
    const QRectF RECT = actor.mapRectToScene(actor.boundingRect());
    const QRectF BLOCK_RECT = block.mapRectToScene(block.boundingRect());

    const float RESTITUTION = 0.12f;
    const float STICKY_THRESHOLD = 0.004f;

    // Side of entry
    float dx = (BLOCK_RECT.center().x() - RECT.center().x()) / BLOCK_RECT.size().width();
    float dy = (BLOCK_RECT.center().y() - RECT.center().y()) / BLOCK_RECT.size().height();

    // Absolute change in x and y
    float absDX = qAbs(dx);
    float absDY = qAbs(dy);

    // Check if collision occured on a corner
    if (qAbs(absDX - absDY) < 0.1) {
        // Actor approaching from positive X
        if (dx < 0) {
            actor.setX(BLOCK_RECT.right());
        }
        // Actor approchaing from negative X
        else {
            actor.setX(BLOCK_RECT.left() - RECT.width());
        }

        // Actor approaching from positive Y
        if (dy < 0) {
            actor.setY(BLOCK_RECT.bottom());
        }
        // Actor approaching from negative Y
        else {
            actor.setY(BLOCK_RECT.top() - RECT.height());
        }

        // Randomly select a x/y direction to reflect velocity (since it's a corner collision)
        if (rand() % 1 == 0) {
            mVelocity.setX(-mVelocity.x() * RESTITUTION);
            if (qAbs(mVelocity.x()) < STICKY_THRESHOLD) {
                mVelocity.setX(0.f);
            }
        }
        else {
            mVelocity.setY(-mVelocity.y() * RESTITUTION);
            if (qAbs(mVelocity.y()) < STICKY_THRESHOLD) {
                mVelocity.setY(0.f);
            }
        }
    }
    // If collision on the sides
    else if (absDX > absDY) {
        if (dx < 0) {
            actor.setX(BLOCK_RECT.right());
        }
        else {
            actor.setX(BLOCK_RECT.left() - RECT.width());
        }

        mVelocity.setX(-mVelocity.x() * RESTITUTION);
        if (qAbs(mVelocity.x()) < STICKY_THRESHOLD) {
            mVelocity.setX(0.f);
        }
    }
    // If collision from top or bottom
    else {
        if (dy < 0) {
            actor.setY(BLOCK_RECT.bottom());
        }
        else {
            actor.setY(BLOCK_RECT.top() - RECT.height());
        }

        mVelocity.setY(-mVelocity.y() * RESTITUTION);
        if (qAbs(mVelocity.y()) < STICKY_THRESHOLD) {
            mVelocity.setY(0.f);
        }
    }
}


QVector2D PhysicsComponent::forceMagnitude(const QVector2D& vector, float forcedMagnitude)
{
    return vector.normalized() * forcedMagnitude;
}


QVector2D PhysicsComponent::limitMagnitude(const QVector2D& vector, float limit)
{
    const float CURRENT_MAGNITUDE = vector.length();
    const QVector2D LIMIT_VECTOR = forceMagnitude(vector, limit);

    return (CURRENT_MAGNITUDE > limit) ? LIMIT_VECTOR : vector;
}

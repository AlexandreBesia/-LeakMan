#pragma once

#include <QGraphicsItem>
#include <QVector2D>

#include "GUI/component.h"

// Forward declaration(s)
class Block;

class PhysicsComponent : public Component
{
public:
    explicit PhysicsComponent(Actor& associatedActor) noexcept;

    void tickUpdate(qreal dt);

    inline void setVelocity(const QVector2D& v) noexcept { mVelocity = v; }
    inline void setAcceleration(const QVector2D& a) noexcept { mAcceleration = a; }
    inline void applyForce(const QVector2D& v) { mVelocity += v; }

    [[nodiscard]] inline const QVector2D& getVelocity() const noexcept { return mVelocity; }
    [[nodiscard]] inline const QVector2D& getAcceleration() const noexcept { return mAcceleration; }
private:
    // Private method(s)
    void handleCollisions(QGraphicsItem& actor);
    void handleBlockCollision(QGraphicsItem& actor, Block& block);

    [[nodiscard]] QVector2D forceMagnitude(const QVector2D& vector, float forcedMagnitude);
    [[nodiscard]] QVector2D limitMagnitude(const QVector2D& vector, float limit);

private:
    // Private member(s)
    QVector2D mVelocity;
    QVector2D mAcceleration;
};

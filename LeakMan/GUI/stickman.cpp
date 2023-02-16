#include "stickman.h"

#include "notGUI/physicscomponent.h"
#include "notGUI/block.h"

// Public constructor(s)
Stickman::Stickman(const QPointF& startPosition)
    : QGraphicsPixmapItem(nullptr)
    , mOriginalPixmap("://img/Stickman.png")
    , mIsEnded(false)
{
    addComponent<PhysicsComponent>();

    setPos(startPosition);
    setPixmap(mOriginalPixmap.scaledToWidth(64));
    markForReset(true);
    reset();

    x = 0;
    size = 64;
}

// Public method(s)
void Stickman::tickUpdate(qreal dt)
{
    PhysicsComponent* physicsComponent = getComponent<PhysicsComponent>();
    Q_ASSERT_X(physicsComponent, "Stickman::tickUpdate", "Stickman instances require a PhysicsComponent");

    if ((mAnimationTimer -= dt) <= 0.0) {
        mAnimationTimer = 24.0 / 5.0 / 60.0;
        x = (x + 1) % 4;
        rect.setRect(x * size, 0, size, size);

        updatePixmap((physicsComponent->getVelocity().x() > 0) ? x : -x - 1);
    }

    physicsComponent->tickUpdate(dt);
}

void Stickman::reset()
{
    PhysicsComponent* physicsComponent = getComponent<PhysicsComponent>();
    physicsComponent->setVelocity(QVector2D(0.f, 0.f));
    physicsComponent->setAcceleration(QVector2D(0.35f, 4.f));
}

// Private method(s)
void Stickman::updatePixmap(int32_t key)
{
    auto cachedItr = mAnimationPixmaps.find(key);
    if (cachedItr == mAnimationPixmaps.end()) {
        mAnimationPixmaps[key] = mOriginalPixmap.copy(rect).scaledToWidth(64);
        if (key < 0) {
            mAnimationPixmaps[key] = mAnimationPixmaps[key].transformed(QTransform().scale(-1.f, 1.f));
        }
    }

    setPixmap(mAnimationPixmaps[key]);
}

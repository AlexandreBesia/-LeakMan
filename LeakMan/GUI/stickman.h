#pragma once

#include <QGraphicsPixmapItem>
#include <QTime>

#include "actor.h"

#include "notGUI/physicscomponent.h"
#include "notGUI/block.h"

class Stickman : public QGraphicsPixmapItem, public Actor
{
public:
    // Public constructor(s)
    explicit Stickman(const QPointF& startPosition = QPointF());

    // Public method(s)
    void tickUpdate(qreal dt) override;
    void reset();

    [[nodiscard]] inline const QPixmap& getOriginalPixmap() const noexcept { return mOriginalPixmap; }

    [[nodiscard]] inline bool isEnded() const noexcept { return mIsEnded; }
    inline void end(bool flag) noexcept { mIsEnded = flag; }

private:
    // Private method(s)
    void updatePixmap(int32_t key);

private:
    // Private member(s)
    std::unordered_map<int32_t, QPixmap> mAnimationPixmaps;

    bool mIsEnded;

    QPixmap mOriginalPixmap;
    QPixmap mCroppedPixmap;

    QRect rect;
    int x, size;

    qreal mAnimationTimer;
};

#include "component.h"

// Protected constructor(s)
Component::Component(Actor& associatedActor) noexcept
    : mAssociatedActor(associatedActor)
{
}

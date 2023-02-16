#pragma once

#include <memory>
#include <unordered_map>

#include "componenttypeconverter.h"

/**
 * @brief Class used for a component-based design.
 */
class Actor
{
public:
    // Public constructor(s)
    Actor()
        : mMarkedForReset(false)
        , mMarkedForRemoval(false)
    {
    }
    virtual ~Actor() = default;

    // Public method(s)
    /*!
     * @brief Adds the component specified by the template typename.
     * @details The component is only added if the same type isn't already in the component list.
     *
     * @par Example:
     * @code
     * actor->addComponent<PhysicsComponent>();
     * @endcode
     *
     * @sa getComponent()
     */
    template <class T>
    void addComponent()
    {
        ComponentTypeConverter::Type type = ComponentTypeConverter::getType<T>();
        mComponents.try_emplace(type, std::make_unique<T>(*this));
    }
    /*!
     * @brief Retrieves the component specified by the template typename.
     *
     * @return The component associated or nullptr if it wasn't found
     *
     * @par Example:
     * @code
     * auto physics = actor->getComponent<PhysicsComponent>();
     * @endcode
     *
     * @sa addComponent()
     */
    template <class T>
    T* getComponent()
    {
        auto component = mComponents.find(ComponentTypeConverter::getType<T>());
        return (component != mComponents.end()) ? dynamic_cast<T*>(component->second.get()) : nullptr;
    }
    /*!
     * @brief Retrieves the component specified by the template typename.
     *
     * @return The component associated or nullptr if it wasn't found
     *
     * @par Example:
     * @code
     * auto physics = actor->getComponent<PhysicsComponent>();
     * @endcode
     *
     * @sa addComponent()
     */
    template <class T>
    const T* getComponent() const
    {
        auto component = mComponents.find(ComponentTypeConverter::getType<T>());
        return (component != mComponents.end()) ? dynamic_cast<const T*>(component->second.get()) : nullptr;
    }

    inline void markForReset(bool flag) noexcept { mMarkedForReset = flag; }
    inline void markForRemoval(bool flag) noexcept { mMarkedForRemoval = flag; }
    [[nodiscard]] inline bool isMarkedForReset() const noexcept { return mMarkedForReset; }
    [[nodiscard]] inline bool isMarkedForRemoval() const noexcept { return mMarkedForRemoval; }

protected:
    // Protected virtual method(s)
    virtual void tickUpdate(qreal dt) {}

private:
    // Private member(s)
    std::unordered_map<ComponentTypeConverter::Type, std::unique_ptr<Component>> mComponents; //!< The actor's attached components
    bool mMarkedForReset;
    bool mMarkedForRemoval;
};

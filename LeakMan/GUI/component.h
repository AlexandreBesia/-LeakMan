#pragma once

// Forward declaration(s)
class Actor;

/*!
 * @brief Abstract base class used in component-based design.
 * @note No direct instance of this may be created.
 */
class Component
{
public:
    // Public constructor(s)
    /*!
     * @brief Virtual destructor.
     * @details A virtual destructor is needed as this class will be inherited.
     */
    virtual ~Component() = default;
protected:
    // Protected constructor(s)
    /*!
     * @brief Constructs the component by providing the associated actor.
     * @note No direct instance of this class may be created.
     *
     * @param[in] associatedActor The actor to which the component will be attached
     */
    Component(Actor& associatedActor) noexcept;

protected:
    // Protected member(s)
    Actor& mAssociatedActor; //!< The actor to which the component is attached
};

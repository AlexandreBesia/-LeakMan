#pragma once

#include <type_traits>

#include "../notGUI/physicscomponent.h"

namespace ComponentTypeConverter
{
    /*!
     * \brief The enumeration of all available components.
     */
    enum class Type { Physics };

    /*!
     * \brief Retrieves the appropriate component type based on the typename.
     *
     * \return The ComponentTypeConverter::Type that corresponds to the typename
     */
    template <class T>
    Type getType()
    {
        static_assert(std::is_base_of_v<Component, T>, "ComponentTypeConverter::getType - Typename needs to derive from Component.");

        Type type;
        if constexpr (std::is_same_v<T, PhysicsComponent>) {
            type = Type::Physics;
        }

        return type;
    }
}

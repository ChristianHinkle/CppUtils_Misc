// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc/Concepts.h>


/*
* Trait that extracts element type from a given container type.
*/
namespace CppUtils
{
    // Primary template.
    template <TNonReferenceType TContainer>
    struct ContainerElementType
    {
        // Due to TContainer::value_type being a common std pattern, we treat this as our default behavior to covers many cases.
        static_assert(sizeof(TContainer) != 0 && requires { typename TContainer::value_type; }, "Container type does not follow std value_type typedef pattern. This will require a custom specialization of your container type.");
        using type = TContainer::value_type;
    };

    // Raw array.
    template <class ElementType, std::size_t Capacity>
    struct ContainerElementType<ElementType[Capacity]>
    {
        using type = ElementType;
    };


}

namespace CppUtils
{
    template <TNonReferenceType TContainer>
    using ContainerElementType_t = typename ContainerElementType<TContainer>::type;
}

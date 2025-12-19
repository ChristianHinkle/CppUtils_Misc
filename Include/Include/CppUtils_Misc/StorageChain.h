// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

/**
 * @brief Builds a heterogeneous storage type using variadic recursive inheritance.
 *
 * Each template parameter contributes a data member via a unique base class,
 * forming a deterministic, linear memory layout derived solely from the order
 * of the template parameter pack.
 *
 * The layout is stable and predictable across translation units and compilers,
 * subject to standard C++ object layout rules.
 */

// Single member declaration.
namespace CppUtils::Misc
{
    template <class T>
    struct Storage
    {
        T m_Data;
    };
}

namespace CppUtils::Misc
{
    template <class... Ts>
    struct StorageChain;

    // Recursively declare members.
    template <class TFirst, class... TRest>
    struct StorageChain<TFirst, TRest...>
        : Storage<TFirst>
        , StorageChain<TRest...>
    {};
    
    // End of recursion.
    template <>
    struct StorageChain<>
    {};
}

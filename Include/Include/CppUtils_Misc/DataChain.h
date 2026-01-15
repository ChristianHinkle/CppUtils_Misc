// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc/DataMixin.h>

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

namespace CppUtils
{
    template <class... Ts>
    struct DataChain;

    // Recursively declare members.
    template <class TFirst, class... TRest>
    struct DataChain<TFirst, TRest...>
        : DataMixin<TFirst>
        , DataChain<TRest...>
    {};
    
    // End of recursion.
    template <>
    struct DataChain<>
    {};
}

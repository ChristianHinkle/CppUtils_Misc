// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/StdReimpl/concepts.h>

namespace CppUtils
{
    /**
     * @brief A software implementation of the IEEE 754 standard's divide operation to support special
     *        cases like NaN and inf values. The purpose of this is to support this behavior with non-floating-point types.
     * @todo @Christian TODO: [todo] Convert this code to work with non-floating-point types. Make static interfaces and
     *       type traits for getting special values like inf, NaN, and -0. We also need functions like copysign and stuff.
     * @todo @Christian TODO: [todo][cpp] Convert this to a constexpr function, as it is basic arithmetic. There are
     *       two options for this: either (1) make our own `constexpr` versions of the `std` functions that aren't
     *       constexpr yet, or (2) wait for C++23 which will make many of the `std` functions `constexpr`. I'm just going
     *       to wait for C++23 support, unless we really need to have this for some reason.
     */
    template <StdReimpl::floating_point TFloat>
    TFloat IeeeDivide(TFloat dividend, TFloat divisor);
}

#include <CppUtils/Misc/Math.inl>

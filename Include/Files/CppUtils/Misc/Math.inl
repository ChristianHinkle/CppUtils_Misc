// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/Math.h>

#include <limits>
#include <cmath>

template <StdReimpl::floating_point TFloat>
TFloat CppUtils::IeeeDivide(TFloat dividend, TFloat divisor)
{
    // Note: We make sure that each return case is abiding by the NaNs-always-propagate rule.

    static_assert(-0.f == 0.f, "It is a C standard that positive and negative zeroes compare equal, according to https://cppreference.com/w/c/language/operator_comparison.html. It's also required by the IEEE 754 section 5.11 standard.");

    if (divisor == static_cast<TFloat>(0))
    {
        // If both the divisor and dividend are zero values, return a NaN.
        if (dividend == static_cast<TFloat>(0))
        {
            return std::numeric_limits<TFloat>::quiet_NaN();
        }

        // Explicitly check for NaN in the other operand to comply with the NaNs-always-propagate rule. Note that
        // the `copysign` return case wouldn't propagate NaN on its own since `copysign` and `signbit` just use the sign bit.
        if (std::isnan(dividend))
        {
            return std::numeric_limits<TFloat>::quiet_NaN();
        }

        // If the divisor is +0, return inf with the original sign of the dividend.
        // If the divisor is -0, return inf with the opposite sign of the dividend.
        return std::copysign(
            std::numeric_limits<TFloat>::infinity(),
            dividend * std::copysign(static_cast<TFloat>(1), divisor));
    }

    if (std::isinf(divisor))
    {
        // If both the divisor and dividend are inf values, return a NaN.
        if (std::isinf(dividend))
        {
            return std::numeric_limits<TFloat>::quiet_NaN();
        }

        // Explicitly check for NaN in the other operand to comply with the NaNs-always-propagate rule. Note that
        // the `copysign` return case wouldn't propagate NaN on its own since `copysign` and `signbit` just use the sign bit.
        if (std::isnan(dividend))
        {
            return std::numeric_limits<TFloat>::quiet_NaN();
        }

        // If the divisor is +inf, return 0 with the original sign of the dividend.
        // If the divisor is -inf, return 0 with the opposite sign of the dividend.
        return std::copysign(
            static_cast<TFloat>(0),
            dividend * std::copysign(static_cast<TFloat>(1), divisor));
    }

    // Note: The potential NaN values from our parameters will naturally propagate in this case, so we are NaNs-always-propagate rule compliant.
    return dividend / divisor;
}

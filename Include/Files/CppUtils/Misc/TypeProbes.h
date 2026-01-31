// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/Concepts.h>

/*
* Type probes are used as a tool for probing functions signatures. Very beneficial when hitting the
* language boundary of non-addressable functions (e.g. useful in tandem with std::is_constructible).
*
* NOTE: It may be possible to generalize this as a single struct for any user-definable type constraint,
*       but for now, defining multiple type probes for the constraints will do (arguably more clear too).
*/
namespace CppUtils
{
    /*
    * A type that can only convert to lvalue references of T (T& or const T&).
    */
    template<CppUtils::TNonReferenceType T>
    struct TypeProbe_LValueRef
    {
        // Allow implicit conversions to lvalue refs.
        operator T&();
        operator const T&() const;
        
        // Disallow implicit conversions to T (by value) to avoid copies.
        operator T() = delete;
        operator const T() const = delete;
        
        // Disallow implicit conversions to rvalue refs, as we only want lvalue refs.
        operator T&&() = delete;
        operator const T&&() const = delete;
    };
}

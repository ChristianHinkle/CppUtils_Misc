// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/Concepts.h>

/*
* Type probes are used as a last resort tool for probing functions signatures. Very beneficial when hitting
* language boundary of non-addressable functions (e.g. useful in tandem with std::is_constructible).
*/
namespace CppUtils
{
    /*
    * A type that can only convert to lvalue references of T (T& or const T&).
    * NOTE: No need to specify implicit conversions to const types, since non-const can simply convert to const.
    */
    template<CppUtils::TNonReferenceType T>
    struct TypeProbe_LValueRef
    {        
        // Allow implicit conversions to lvalue refs.
        operator T&();
        
        // Disallow implicit conversions to rvalue refs, as we only want lvalue refs.
        operator T&&() = delete;
    };

    /*
    * A type that can only convert to const references of T (const T& or const T&&).
    */
    template<CppUtils::TNonReferenceType T>
    struct TypeProbe_ConstRef
    {
        operator const T&();
        
        operator const T&&();
    };
}

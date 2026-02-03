// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/FunctionTraits.h>


namespace CppUtils
{
    /*
    * A type which is callable (via operator()).
    * Convenient option over std::invocable, which requires knowlage of the call signature.
    */
    template <auto T>
    concept TCallable = requires { typename FunctionPtrTraits<T>; };

    template <class T>
    concept TReferenceType = std::is_reference_v<T>;

    template <class T>
    concept TNonRefType = !TReferenceType<T>;

    template <class T>
    concept TNonCVRefType = std::is_same_v<T, std::remove_cvref_t<T>>;
}

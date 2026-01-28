// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc/FunctionTraits.h>


namespace CppUtils
{
    /*
    * A type which is callable (via operator()).
    */
    template <auto T>
    concept TCallable = requires { typename FunctionPtrTraits<T>; };

    template <class T>
    concept TReferenceType = std::is_reference_v<T>;

    template <class T>
    concept TNonReferenceType = !TReferenceType<T>;
}

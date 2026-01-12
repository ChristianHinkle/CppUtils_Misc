// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <tuple>

/*
* Function traits, used to extract information about a given function.
* Specializations make use of inheritance to avoid dup code.
*/
namespace CppUtils
{
    // Primary template.
    template <class TFunc>
    struct FunctionTraits
    {
        static_assert(sizeof(TFunc) == 0, "Provided type must be either a function or function pointer.");
    };

    // Free function specialization.
    template <class TReturnType, class... TArgs>
    struct FunctionTraits<TReturnType (TArgs...)>
    {
        using ReturnType = TReturnType;

        using ArgsTuple = std::tuple<TArgs...>;

        using ClassType = void; // No class type for free functions.
    };

    // Pointer to free function specialization.
    template <class TReturnType, class... TArgs>
    struct FunctionTraits<TReturnType (*)(TArgs...)>
        : FunctionTraits<TReturnType (TArgs...)>
    {
    };


    // Pointer to member function specializations.

    // Non-const.
    template <class TReturnType, class TClass, class... TArgs>
    struct FunctionTraits<TReturnType (TClass::*)(TArgs...)>
        : FunctionTraits<TReturnType (*)(TArgs...)>
    {
        using ClassType = TClass;
    };

    // Const.
    template <class TReturnType, class TClass, class... TArgs>
    struct FunctionTraits<TReturnType (TClass::*)(TArgs...) const>
        : FunctionTraits<TReturnType (TClass::*)(TArgs...)>
    {};


    // Non-pointer callable types (e.g. lambda) specialization.
    template <class T>
        requires (requires { &T::operator(); })
    struct FunctionTraits<T>
        : FunctionTraits<decltype(&T::operator())>
    {};
}

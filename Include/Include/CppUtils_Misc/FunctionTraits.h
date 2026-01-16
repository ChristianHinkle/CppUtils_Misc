// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <tuple>

/*
* Function traits, used to extract information about a given function.
* Specializations make use of inheritance to avoid dup code.
*
* NOTE: Non-static member functions have no type because they requires
*       an instance (this ptr). Func ptr should be used in that case.
*       If it's impossible to get a ptr to it (e.g. consteval), calling
*       the function via requires expressions is a viable alternative.
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

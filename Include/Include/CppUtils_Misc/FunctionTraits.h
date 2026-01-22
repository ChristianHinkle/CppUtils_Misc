// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <tuple>

/*
* Function traits, used to extract information about a given function.
* Specializations make use of inheritance to avoid dup code.
*
* NOTE: Non-static member functions have no type because they require
*       an instance (this ptr). Func ptr should be used in that case.
*       If it's impossible to get a ptr to it (e.g. consteval), calling
*       the function via requires expressions is a viable alternative.
*/
namespace CppUtils
{
    // Primary template.
    template <class TFunc>
    struct FunctionTypeTraits
    {
        static_assert(sizeof(TFunc) == 0, "Provided type must be either a function or function pointer.");
    };

    // Free function specialization.
    template <class TReturnType, class... TArgs>
    struct FunctionTypeTraits<TReturnType (TArgs...)>
    {
        using ReturnType = TReturnType;

        using ArgsTuple = std::tuple<TArgs...>;

        using ClassType = void; // No class type for free functions.


        // BEGIN: Helpers for convenience        
        static consteval std::size_t GetArgsCount() { return std::tuple_size_v<ArgsTuple>; }
        // END: Helpers for convenience
    };

    // Pointer to free function specialization.
    template <class TReturnType, class... TArgs>
    struct FunctionTypeTraits<TReturnType (*)(TArgs...)>
        : FunctionTypeTraits<TReturnType (TArgs...)>
    {
    };


    // Pointer to member function specializations.

    // Non-const.
    template <class TReturnType, class TClass, class... TArgs>
    struct FunctionTypeTraits<TReturnType (TClass::*)(TArgs...)>
        : FunctionTypeTraits<TReturnType (*)(TArgs...)>
    {
        using ClassType = TClass;
    };

    // Const.
    template <class TReturnType, class TClass, class... TArgs>
    struct FunctionTypeTraits<TReturnType (TClass::*)(TArgs...) const>
        : FunctionTypeTraits<TReturnType (TClass::*)(TArgs...)>
    {};


    // Non-pointer callable types (e.g. lambda) specialization.
    template <class T>
        requires (requires { &T::operator(); })
    struct FunctionTypeTraits<T>
        : FunctionTypeTraits<decltype(&T::operator())>
    {};    
}

/*
* Convenience type for function ptr values, removing the need for decltype.
*/
namespace CppUtils
{
    template <auto FuncPtr>
    struct FunctionPtrTraits
        : FunctionTypeTraits<decltype(FuncPtr)>
    {};
}

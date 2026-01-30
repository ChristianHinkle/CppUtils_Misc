// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <tuple>

/*
* Function traits, used to extract information about a given function.
* Specializations make use of inheritance to avoid dup code.
*
* USAGE NOTES:
*
* - Non-static member functions have no type because they require
*       an instance (this ptr). Func ptr should be used in that case.
*
* -  Consteval functions: If you can't get traits due to the function being
*       immediate (consteval), use the trait instantiation trick. Since immediate
*       function pointers are only available in consteval contexts, create a
*       consteval function that returns decltype(auto) (an instantiation of
*       FunctionTypeTraits). Get the traits by taking decltype of that function's
*       return value.
*
* -  As a final fallback for complex cases, you may simply use a requires expressions as an alternative to function traits.
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

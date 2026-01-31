// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <type_traits>
#include <CppUtils/Misc/TypeProbes.h>

namespace
{
    struct Foo {};

    /*
    * By values.
    */
    struct Ctor_Value
    {
        Ctor_Value(Foo);
    };
    struct Ctor_ConstValue
    {
        Ctor_ConstValue(const Foo);
    };

    /*
    * By lvalue refs.
    */
    struct Ctor_LvalueRef
    {
        Ctor_LvalueRef(Foo&);
    };
    struct Ctor_ConstLvalueRef
    {
        Ctor_ConstLvalueRef(const Foo&);
    };

    /*
    * By rvalue refs.
    */
    struct Ctor_RvalueRef
    {
        Ctor_RvalueRef(Foo&&);
    };
    struct Ctor_ConstRvalueRef
    {
        Ctor_ConstRvalueRef(const Foo&&);
    };

    /*
    * TypeProbe_LValueRef
    */
    static_assert(!std::is_constructible_v<Ctor_Value,          CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be rejected from `Foo` (value) ctr.");
    static_assert(!std::is_constructible_v<Ctor_ConstValue,     CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be rejected from `const Foo` (const value) ctr.");
    static_assert( std::is_constructible_v<Ctor_LvalueRef,      CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be accepted into `Foo&` (lvalue ref) ctr.");
    static_assert( std::is_constructible_v<Ctor_ConstLvalueRef, CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be accepted into `const Foo&` (const lvalue ref) ctr.");
    static_assert(!std::is_constructible_v<Ctor_RvalueRef,      CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be rejected from `Foo&&` (rvalue ref) ctr.");
    static_assert(!std::is_constructible_v<Ctor_ConstRvalueRef, CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be rejected from `const Foo&&` (const rvalue ref) ctr.");

    /*
    * TypeProbe_ConstRef
    */
    static_assert(!std::is_constructible_v<Ctor_Value,          CppUtils::TypeProbe_ConstRef<Foo>>, "Expected to be rejected from `Foo` (value) ctr.");
    static_assert(!std::is_constructible_v<Ctor_ConstValue,     CppUtils::TypeProbe_ConstRef<Foo>>, "Expected to be rejected from `const Foo` (const value) ctr.");
    static_assert(!std::is_constructible_v<Ctor_LvalueRef,      CppUtils::TypeProbe_ConstRef<Foo>>, "Expected to be rejected into `Foo&` (lvalue ref) ctr.");
    static_assert( std::is_constructible_v<Ctor_ConstLvalueRef, CppUtils::TypeProbe_ConstRef<Foo>>, "Expected to be accepted into `const Foo&` (const lvalue ref) ctr.");
    static_assert(!std::is_constructible_v<Ctor_RvalueRef,      CppUtils::TypeProbe_ConstRef<Foo>>, "Expected to be rejected from `Foo&&` (rvalue ref) ctr.");
    static_assert( std::is_constructible_v<Ctor_ConstRvalueRef, CppUtils::TypeProbe_ConstRef<Foo>>, "Expected to be accepted into `const Foo&&` (const rvalue ref) ctr.");
}

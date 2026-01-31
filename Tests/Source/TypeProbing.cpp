// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <type_traits>
#include <CppUtils/Misc/TypeProbes.h>

namespace
{
    struct Foo {};

    /*
    * By lvalue refs.
    */
    struct Allowed1
    {
        Allowed1(Foo&);
    };
    struct Allowed2
    {
        Allowed2(const Foo&);
    };

    /*
    * By values.
    */
    struct Disallowed1
    {
        Disallowed1(Foo);
    };
    struct Disallowed2
    {
        Disallowed2(const Foo);
    };

    /*
    * By rvalue refs.
    */
    struct Disallowed3
    {
        Disallowed3(Foo&&);
    };
    struct Disallowed4
    {
        Disallowed4(const Foo&&);
    };

    static_assert(std::is_constructible_v<Allowed1, CppUtils::TypeProbe_LValueRef<Foo>>,     "Expected to be accepted into `Foo&` (lvalue) ctr.");
    static_assert(std::is_constructible_v<Allowed2, CppUtils::TypeProbe_LValueRef<Foo>>,     "Expected to be accepted into `const Foo&` (const lvalue) ctr.");
    static_assert(!std::is_constructible_v<Disallowed1, CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be rejected from `Foo` (value) ctr.");
    static_assert(!std::is_constructible_v<Disallowed2, CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be rejected from `const Foo` (const value) ctr.");
    static_assert(!std::is_constructible_v<Disallowed3, CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be rejected from `Foo&&` (rvalue ref) ctr.");
    static_assert(!std::is_constructible_v<Disallowed4, CppUtils::TypeProbe_LValueRef<Foo>>, "Expected to be rejected from `const Foo&&` (const rvalue ref) ctr.");
}

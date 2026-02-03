// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils/Misc/Static_Execute.h>

// Simple cases.

CPPUTILS_STATIC_EXECUTE(
    int myNummy = 3;
);

CPPUTILS_STATIC_EXECUTE(
    int myNummy = 3;
    int myOthaNummy = 5;
);

// Something a little more complex. Call a lambda.
CPPUTILS_STATIC_EXECUTE(
    // Use commas and stuff to make things complicated.
    [](int param1st, char param2nd, bool param3rd)
    {
        float yoooo = 3.f;
    }(3, 'g', true);
);

// I should be able to return early in my logic.

CPPUTILS_STATIC_EXECUTE(
    return;
);

CPPUTILS_STATIC_EXECUTE(
    if (!true)
    {
        return;
    }

    auto& myStringView = "thingy";
);

// Constexpr if stuff.

CPPUTILS_STATIC_EXECUTE(
    if constexpr (true)
    {
        return;
    }
    else
    {
        // Code path that will never be compiled because the constexpr if condition prevents it from ever being taken.
        static_assert(false);
    }
);

// Calling on consteval functions.

namespace
{
    consteval void DoMyConstevalLogic()
    {
        static_assert(true);
        static_assert(true && true);

        using MyIntArrayUnknownBoundType = int[];

        MyIntArrayUnknownBoundType myValues = { 3, 5, 13531 };

        // Okay yeah we did our logic.
    }
}

CPPUTILS_STATIC_EXECUTE(
    DoMyConstevalLogic();
);

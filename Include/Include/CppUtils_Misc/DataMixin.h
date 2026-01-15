// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

// Simple mixin for single member declaration.
namespace CppUtils
{
    template <class T>
    struct DataMixin
    {
        T m_Data{};
    };
}

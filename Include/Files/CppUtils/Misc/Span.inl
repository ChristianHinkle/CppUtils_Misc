// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/Span.h>
#include <utility>
#include <algorithm>
#include <ranges>
#include <cassert>

namespace CppUtils::Detail
{
    template <class T>
    struct StdSpanTrait
    {
        // This is the primary template with value defaulted to false.
        enum
        {
            value = false
        };
    };

    template <class T, std::size_t extent>
    struct StdSpanTrait<std::span<T, extent>>
    {
        // This is a partial specialization for the true case.
        enum
        {
            value = true
        };
    };
}

template <class T>
consteval bool CppUtils::IsStdSpan()
{
    return CppUtils::Detail::StdSpanTrait<T>::value;
}

template <class T, std::size_t extent>
void CppUtils::RemoveElement(std::span<T, extent>& span, std::size_t index, std::remove_reference_t<T>&& replacementValue)
{
    assert(index >= 0u);
    assert(index < span.size());

    // Remove the element by shifting the next elements over to replace it.
    std::ranges::move(span.subspan(index + 1), span.subspan(index).begin());

    // Update size of our span.
    span.back() = std::move(replacementValue);
    span = span.subspan(0u, span.size() - 1);
}

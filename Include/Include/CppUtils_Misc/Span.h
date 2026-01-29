// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <span>
#include <type_traits>

namespace CppUtils::Misc::Span
{
    template <class T, std::size_t extent = std::dynamic_extent>
    void RemoveElement(std::span<T, extent>& span, std::size_t index, std::remove_reference_t<T>&& replacementValue);
}

#include <CppUtils_Misc/Span.inl>

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>
#include <CppUtils/Misc/CharBufferString.h>
#include <CppUtils/Core/Concepts.h>

/**
 * @brief String utilities to extend the default standard library tools.
 */
namespace CppUtils
{
    template <CppUtils::CharLike TToChar, std::size_t bufferSize, CppUtils::CharLike TFromChar, class TFromTraits>
    CharBufferString<TToChar, bufferSize> ConstructCharacterBufferFromString(const std::basic_string_view<TFromChar, TFromTraits>& fromString);

    template <CppUtils::CharLike TToChar, std::size_t bufferSize, CppUtils::CharLike TFromChar, class TFromTraits>
    void AppendStringToCharacterBuffer(
        CharBufferString<TToChar, bufferSize>& characterBuffer,
        const std::basic_string_view<TFromChar, TFromTraits>& fromString);
}

#include <CppUtils/Misc/String.inl>

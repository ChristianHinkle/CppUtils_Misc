// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>
#include <string>
#include <span>
#include <CppUtils_Misc/CharBufferString.h>

/**
 * @brief String utilities to extend the default standard library tools.
 */
namespace CppUtils::Misc::String
{
    template <class TChar, class TTraits>
    std::basic_string_view<TChar, TTraits> TrimLeadingWhitespace(std::basic_string_view<TChar, TTraits> sourceString);

    template <class TChar, class TTraits>
    std::basic_string_view<TChar, TTraits> TrimTrailingWhitespace(std::basic_string_view<TChar, TTraits> sourceString);

    template <class TChar>
    std::span<TChar> TrimLeadingWhitespace(std::span<TChar> sourceString);

    template <class TChar>
    std::span<TChar> TrimTrailingWhitespace(std::span<TChar> sourceString);

    template <class TChar, class TTraits>
    std::span<const TChar> MakeSpanFromStringView(const std::basic_string_view<TChar, TTraits>& stringView);

    template <class TChar, class TTraits = std::char_traits<TChar>>
    std::basic_string_view<TChar, TTraits> MakeStringViewFromSpan(const std::span<TChar>& charSpan);

    template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
    CharBufferString<TToChar, bufferSize> ConstructCharacterBufferFromString(const std::basic_string_view<TFromChar, TFromTraits>& fromString);

    /**
     * @brief Construct a string view from an `std::basic_string` via template argument deduction.
     */
    template <class TChar, class TTraits, class TAllocator>
    std::basic_string_view<TChar, TTraits> MakeStringView(const std::basic_string<TChar, TTraits, TAllocator>& fromString);

    template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
    void AppendStringToCharacterBuffer(
        CharBufferString<TToChar, bufferSize>& characterBuffer,
        const std::basic_string_view<TFromChar, TFromTraits>& fromString);
}

#include <CppUtils_Misc/String.inl>

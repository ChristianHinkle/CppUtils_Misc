// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>
#include <string>
#include <span>
#include <CppUtils/Misc/CharBufferString.h>
#include <CppUtils/Core/Concepts.h>

/**
 * @brief String utilities to extend the default standard library tools.
 */
namespace CppUtils
{
    template <CppUtils::CharLike TChar, class TTraits>
    bool HasLeadingWhitespace(std::basic_string_view<TChar, TTraits> sourceString);

    template <CppUtils::CharLike TChar, class TTraits>
    bool HasTrailingWhitespace(std::basic_string_view<TChar, TTraits> sourceString);

    template <CppUtils::CharLike TChar, class TTraits>
    std::basic_string_view<TChar, TTraits> TrimLeadingWhitespace(std::basic_string_view<TChar, TTraits> sourceString);

    template <CppUtils::CharLike TChar, class TTraits>
    std::basic_string_view<TChar, TTraits> TrimTrailingWhitespace(std::basic_string_view<TChar, TTraits> sourceString);

    template <CppUtils::CharLike TChar>
    std::span<TChar> TrimLeadingWhitespace(std::span<TChar> sourceString);

    template <CppUtils::CharLike TChar>
    std::span<TChar> TrimTrailingWhitespace(std::span<TChar> sourceString);

    template <CppUtils::CharLike TChar, class TTraits>
    std::size_t GetTrimmedLeadingWhitespaceStartPos(std::basic_string_view<TChar, TTraits> sourceString);

    template <CppUtils::CharLike TChar, class TTraits>
    std::size_t GetTrimmedTrailingWhitespaceSize(std::basic_string_view<TChar, TTraits> sourceString);

    template <CppUtils::CharLike TChar>
    std::size_t GetTrimmedLeadingWhitespaceStartPos(std::span<TChar> sourceString);

    template <CppUtils::CharLike TChar>
    std::size_t GetTrimmedTrailingWhitespaceSize(std::span<TChar> sourceString);

    template <CppUtils::CharLike TChar, class TTraits>
    std::span<const TChar> MakeSpanFromStringView(const std::basic_string_view<TChar, TTraits>& stringView);

    template <CppUtils::CharLike TChar, class TTraits = std::char_traits<TChar>>
    std::basic_string_view<TChar, TTraits> MakeStringViewFromSpan(const std::span<TChar>& charSpan);

    template <CppUtils::CharLike TToChar, std::size_t bufferSize, CppUtils::CharLike TFromChar, class TFromTraits>
    CharBufferString<TToChar, bufferSize> ConstructCharacterBufferFromString(const std::basic_string_view<TFromChar, TFromTraits>& fromString);

    /**
     * @brief Construct a string view from an `std::basic_string` via template argument deduction.
     */
    template <CppUtils::CharLike TChar, class TTraits, class TAllocator>
    std::basic_string_view<TChar, TTraits> MakeStringView(const std::basic_string<TChar, TTraits, TAllocator>& fromString);

    template <CppUtils::CharLike TToChar, std::size_t bufferSize, CppUtils::CharLike TFromChar, class TFromTraits>
    void AppendStringToCharacterBuffer(
        CharBufferString<TToChar, bufferSize>& characterBuffer,
        const std::basic_string_view<TFromChar, TFromTraits>& fromString);

    // @Christian: TODO: [todo] Move these to CppUtils_Core. And move non-string-related utils out of this string utilities files.

    template <StdReimpl::integral TInteger>
    constexpr std::size_t CountNumDigits(TInteger number, unsigned int base);

    template <StdReimpl::integral TInteger>
    constexpr std::size_t CountNumDigitsDec(TInteger number)
    {
        return CountNumDigits(number, 10u);
    }

    template <StdReimpl::integral TInteger>
    constexpr std::size_t CountNumDigitsHex(TInteger number)
    {
        return CountNumDigits(number, 16u);
    }

    template <StdReimpl::integral TInteger>
    constexpr std::size_t CountNumDigitsBin(TInteger number)
    {
        return CountNumDigits(number, 2u);
    }

    /**
     * @brief Gets the necessary string capacity to represent any number of the integer type in the decimal number system. Takes
     *        in an optional parameter if template argument deduction from a specific number is desired.
     */
    template <StdReimpl::integral TInteger>
    consteval std::size_t GetIntegerStringMaxSizeDec(TInteger = TInteger{});
}

#include <CppUtils/Misc/String.inl>

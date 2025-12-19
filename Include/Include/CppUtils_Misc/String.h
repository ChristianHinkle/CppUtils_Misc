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
    template <class TChar, class TTraits = std::char_traits<TChar>>
    std::basic_string_view<TChar, TTraits> GetStringViewFromCharacterBuffer(const std::span<TChar>& characterBuffer);

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

template <class TChar, class TTraits>
std::basic_string_view<TChar, TTraits> CppUtils::Misc::String::GetStringViewFromCharacterBuffer(
    const std::span<TChar>& characterBuffer)
{
    return std::basic_string_view<TChar, TTraits>(characterBuffer.data(), characterBuffer.size());
}

template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
CppUtils::Misc::CharBufferString<TToChar, bufferSize> CppUtils::Misc::String::ConstructCharacterBufferFromString(
    const std::basic_string_view<TFromChar, TFromTraits>& fromString)
{
    return CharBufferString<TToChar, bufferSize>(
        [&fromString](CharBufferString<TToChar, bufferSize>& charBufferString)
        {
            AppendStringToCharacterBuffer(charBufferString, fromString);
        }
        );
}

template <class TChar, class TTraits, class TAllocator>
std::basic_string_view<TChar, TTraits> CppUtils::Misc::String::MakeStringView(const std::basic_string<TChar, TTraits, TAllocator>& fromString)
{
    return std::basic_string_view<TChar, TTraits>(fromString);
}

template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
void CppUtils::Misc::String::AppendStringToCharacterBuffer(
    CharBufferString<TToChar, bufferSize>& characterBuffer,
    const std::basic_string_view<TFromChar, TFromTraits>& fromString)
{
    const std::size_t oldLength = characterBuffer.Length;

    // Add the length.
    characterBuffer.Length += fromString.length();

    // Copy each character to the buffer, converting them in the process.
    for (std::size_t i = 0; i < fromString.length(); ++i)
    {
        // Note: The first iteration will overwrite the null-terminating character, which is good.
        characterBuffer.CharBuffer[oldLength + i] = static_cast<TToChar>(fromString.at(i));
    }

    // Finally, terminate the string with a null character.
    characterBuffer.CharBuffer[oldLength + fromString.length()] = static_cast<TToChar>('\0');
}

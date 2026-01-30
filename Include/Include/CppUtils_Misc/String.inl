// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc/String.h>

#include <locale>

template <class TChar, class TTraits>
std::basic_string_view<TChar, TTraits> CppUtils::TrimLeadingWhitespace(std::basic_string_view<TChar, TTraits> sourceString)
{
    return TrimLeadingWhitespace(sourceString);
}

template <class TChar, class TTraits>
std::basic_string_view<TChar, TTraits> CppUtils::TrimTrailingWhitespace(std::basic_string_view<TChar, TTraits> sourceString)
{
    return TrimTrailingWhitespace(sourceString);
}

template <class TChar>
std::span<TChar> CppUtils::TrimLeadingWhitespace(std::span<TChar> sourceString)
{
    for (std::size_t pos = 0u; pos < sourceString.size(); ++pos)
    {
        if (!std::isspace(sourceString[pos]))
        {
            return sourceString.subspan(pos);
        }
    }

    return sourceString.subspan(sourceString.size());
}

template <class TChar>
std::span<TChar> CppUtils::TrimTrailingWhitespace(std::span<TChar> sourceString)
{
    for (std::size_t pos = sourceString.length() - 1; pos >= 0u; --pos)
    {
        if (!std::isspace(sourceString[pos]))
        {
            return sourceString.substr(pos);
        }
    }

    return sourceString.substr(0u, 0u);
}

template <class TChar, class TTraits>
std::span<const TChar> CppUtils::MakeSpanFromStringView(const std::basic_string_view<TChar, TTraits>& stringView)
{
    return std::span<const TChar>(stringView.data(), stringView.length());
}

template <class TChar, class TTraits>
std::basic_string_view<TChar, TTraits> CppUtils::MakeStringViewFromSpan(const std::span<TChar>& charSpan)
{
    return std::basic_string_view<TChar, TTraits>(charSpan.data(), charSpan.size());
}

template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
CppUtils::CharBufferString<TToChar, bufferSize> CppUtils::ConstructCharacterBufferFromString(
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
std::basic_string_view<TChar, TTraits> CppUtils::MakeStringView(const std::basic_string<TChar, TTraits, TAllocator>& fromString)
{
    return std::basic_string_view<TChar, TTraits>(fromString);
}

template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
void CppUtils::AppendStringToCharacterBuffer(
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

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/String.h>

#include <locale>

template <CppUtils::CharLike TChar, class TTraits>
bool CppUtils::HasLeadingWhitespace(std::basic_string_view<TChar, TTraits> sourceString)
{
    return !sourceString.empty() && std::isspace(sourceString.front());
}

template <CppUtils::CharLike TChar, class TTraits>
bool CppUtils::HasTrailingWhitespace(std::basic_string_view<TChar, TTraits> sourceString)
{
    return !sourceString.empty() && std::isspace(sourceString.back());
}

template <CppUtils::CharLike TChar, class TTraits>
std::basic_string_view<TChar, TTraits> CppUtils::TrimLeadingWhitespace(std::basic_string_view<TChar, TTraits> sourceString)
{
    return TrimLeadingWhitespace(std::span{sourceString.data(), sourceString.size()});
}

template <CppUtils::CharLike TChar, class TTraits>
std::basic_string_view<TChar, TTraits> CppUtils::TrimTrailingWhitespace(std::basic_string_view<TChar, TTraits> sourceString)
{
    return TrimTrailingWhitespace(std::span{sourceString.data(), sourceString.size()});
}

template <CppUtils::CharLike TChar>
std::span<TChar> CppUtils::TrimLeadingWhitespace(std::span<TChar> sourceString)
{
    return sourceString.subspan(GetTrimmedLeadingWhitespaceStartPos(sourceString));
}

template <CppUtils::CharLike TChar>
std::span<TChar> CppUtils::TrimTrailingWhitespace(std::span<TChar> sourceString)
{
    return sourceString.subspan(0u, GetTrimmedTrailingWhitespaceSize(sourceString));
}

template <CppUtils::CharLike TChar, class TTraits>
std::size_t CppUtils::GetTrimmedLeadingWhitespaceStartPos(std::basic_string_view<TChar, TTraits> sourceString)
{
    return GetTrimmedLeadingWhitespaceStartPos(std::span{sourceString.data(), sourceString.size()});
}

template <CppUtils::CharLike TChar, class TTraits>
std::size_t CppUtils::GetTrimmedTrailingWhitespaceSize(std::basic_string_view<TChar, TTraits> sourceString)
{
    return GetTrimmedTrailingWhitespaceSize(std::span{sourceString.data(), sourceString.size()});
}

template <CppUtils::CharLike TChar>
std::size_t CppUtils::GetTrimmedLeadingWhitespaceStartPos(std::span<TChar> sourceString)
{
    for (std::size_t pos = 0u; pos < sourceString.size(); ++pos)
    {
        if (!std::isspace(sourceString[pos]))
        {
            return pos;
        }
    }

    return sourceString.size();
}

template <CppUtils::CharLike TChar>
std::size_t CppUtils::GetTrimmedTrailingWhitespaceSize(std::span<TChar> sourceString)
{
    for (std::size_t pos = sourceString.size() - 1; pos >= 0u; --pos)
    {
        if (!std::isspace(sourceString[pos]))
        {
            return pos;
        }
    }

    return 0u;
}

template <CppUtils::CharLike TChar, class TTraits>
std::span<const TChar> CppUtils::MakeSpanFromStringView(const std::basic_string_view<TChar, TTraits>& stringView)
{
    return std::span<const TChar>(stringView.data(), stringView.length());
}

template <CppUtils::CharLike TChar, class TTraits>
std::basic_string_view<TChar, TTraits> CppUtils::MakeStringViewFromSpan(const std::span<TChar>& charSpan)
{
    return std::basic_string_view<TChar, TTraits>(charSpan.data(), charSpan.size());
}

template <CppUtils::CharLike TToChar, std::size_t bufferSize, CppUtils::CharLike TFromChar, class TFromTraits>
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

template <CppUtils::CharLike TChar, class TTraits, class TAllocator>
std::basic_string_view<TChar, TTraits> CppUtils::MakeStringView(const std::basic_string<TChar, TTraits, TAllocator>& fromString)
{
    return std::basic_string_view<TChar, TTraits>(fromString);
}

template <CppUtils::CharLike TToChar, std::size_t bufferSize, CppUtils::CharLike TFromChar, class TFromTraits>
void CppUtils::AppendStringToCharacterBuffer(
    CharBufferString<TToChar, bufferSize>& characterBuffer,
    const std::basic_string_view<TFromChar, TFromTraits>& fromString)
{
    const std::size_t oldLength = characterBuffer.GetLength();

    // Add the length.
    characterBuffer.GetLengthMutable() += fromString.length();

    // Copy each character to the buffer, converting them in the process.
    for (std::size_t i = 0; i < fromString.length(); ++i)
    {
        // Note: The first iteration will overwrite the null-terminating character, which is good.
        characterBuffer.GetCharBufferMutable()[oldLength + i] = static_cast<TToChar>(fromString.at(i));
    }

    // Finally, terminate the string with a null character.
    characterBuffer.GetCharBufferMutable()[oldLength + fromString.length()] = static_cast<TToChar>('\0');
}

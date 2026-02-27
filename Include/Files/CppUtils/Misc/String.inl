// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/String.h>

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

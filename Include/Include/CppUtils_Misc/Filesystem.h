// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <filesystem>
#include <string_view>
#include <CppUtils_Misc/String.h>
#include <CppUtils_Misc/CharBufferString.h>

/**
 * @brief Filesystem utilities to extend the default std::filesystem tools.
 */
namespace CppUtils::Misc::Filesystem
{
    using StdPathStringView = std::basic_string_view<const std::filesystem::path::value_type>;

    CPPUTILS_MISC_EXPORT StdPathStringView GetStringViewFromPath(const std::filesystem::path& path);

    template <std::size_t bufferSize, class TChar = char, class TCharTraits = std::char_traits<TChar>>
    CharBufferString<TChar, bufferSize, TCharTraits> ConstructCharacterBufferFromPath(const std::filesystem::path& path);

    template <std::size_t bufferSize, class TChar = char, class TCharTraits = std::char_traits<TChar>>
    void AppendPathToCharacterBuffer(
        CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer,
        const std::filesystem::path& path);
}

template <std::size_t bufferSize, class TChar, class TCharTraits>
CppUtils::Misc::CharBufferString<TChar, bufferSize, TCharTraits> CppUtils::Misc::Filesystem::ConstructCharacterBufferFromPath(
    const std::filesystem::path& path)
{
    return CharBufferString<TChar, bufferSize, TCharTraits>(
        [&path](CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer)
        {
            AppendPathToCharacterBuffer<bufferSize, TChar, TCharTraits>(characterBuffer, path);
        }
        );
}

template <std::size_t bufferSize, class TChar, class TCharTraits>
void CppUtils::Misc::Filesystem::AppendPathToCharacterBuffer(
    CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer,
    const std::filesystem::path& path)
{
    using TFromChar = std::filesystem::path::string_type::value_type;
    using TFromTraits = std::filesystem::path::string_type::traits_type;

    return String::AppendStringToCharacterBuffer<TChar, bufferSize, TFromChar, TFromTraits>(
        characterBuffer,
        path.native());
}

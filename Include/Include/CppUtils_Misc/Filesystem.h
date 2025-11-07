// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <filesystem>
#include <string_view>

/**
 * @brief Filesystem utilities to extend the default std::filesystem tools.
 */
namespace CppUtils::Misc::Filesystem
{
    using StdPathStringView = std::basic_string_view<std::filesystem::path::value_type>;

    CPPUTILS_MISC_EXPORT StdPathStringView GetStringViewFromPath(const std::filesystem::path& path);
}

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>

/**
 * @brief String literal for `std::format` to represent a time point in full ISO 8601 format: `YYYY-MM-DDThh:mm:ss.sssssss+zz:zz`.
 */
#define CPPUTILS_FORMAT_TIME_ISO_FULL_STRING_LITERAL "{:%Y-%m-%dT%H:%M:%S%Ez}"

namespace CppUtils
{
}

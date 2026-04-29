// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>

/**
 * @brief String literal for `std::format` to represent a time in full ISO 8601 format: yyyy-mm-ddThh:mm:ss+/-zzzz
 * @note Precision of the seconds is determined by the duration of the time point given to `std::format`. It may add a decimal point so that it can represent milliseconds, for example.
 * @see https://cppreference.com/cpp/chrono/system_clock/formatter for format specifiers.
 */
#define CPPUTILS_FORMAT_TIME_ISO_FULL_STRING_LITERAL "{:%FT%T%z}" // `%F` expands to `%Y-%m-%d` and `%T` expands to `%H:%M:%S`.

namespace CppUtils
{
}

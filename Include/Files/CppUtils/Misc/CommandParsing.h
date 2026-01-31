// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>
#include <vector>
#include <CppUtils/StdReimpl/Concepts.h>
#include <optional>
#include <CppUtils/Core/StringSpan.h>
#include <CppUtils/Core/Concepts.h>
#include <string>

/**
 * @brief Command line parsing utilities.
 */
namespace CppUtils
{
    template <CppUtils::CharLike TChar, class TCharTraits = std::char_traits<TChar>>
    struct CommandParsing
    {
        using CharType = TChar;
        using CharTraitsType = TCharTraits;

        /**
         * @note Returns views to the passed in string, so it should not be a temporary lifetime string! Keep the
         *       original string alive as storage for the returned string views.
         */
        static std::vector<std::basic_string_view<TChar, TCharTraits>> InPlaceShellTokenize(CppUtils::StringSpan<TChar, TCharTraits> mutableArgs);

        /**
         * @note Returns views to the passed in string, so it should not be a temporary lifetime string! Keep the
         *       original string alive as storage for the returned string views.
         */
        template <StdReimpl::invocable<const std::basic_string_view<TChar, TCharTraits>&> TVisitor>
        static void InPlaceShellTokenizeVisitor(CppUtils::StringSpan<TChar, TCharTraits> mutableArgs, TVisitor&& visitor);

        static std::optional<std::basic_string_view<TChar, TCharTraits>> InPlaceShellTokenizeNext(CppUtils::StringSpan<TChar, TCharTraits>& mutableArgs);
    };
}

#include <CppUtils/Misc/CommandParsing.inl>

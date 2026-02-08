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

        using StringViewType = std::basic_string_view<TChar, TCharTraits>;
        using StringSpanType = CppUtils::StringSpan<TChar, TCharTraits>;

        /**
         * @note Returns views to the passed in string, so it should not be a temporary lifetime string! Keep the
         *       original string alive as storage for the returned string views.
         */
        static std::vector<StringViewType> InPlaceShellTokenize(StringSpanType mutableArgs);

        /**
         * @note Returns views to the passed in string, so it should not be a temporary lifetime string! Keep the
         *       original string alive as storage for the returned string views.
         */
        template <StdReimpl::invocable<const std::basic_string_view<TChar, TCharTraits>&> TVisitor>
        static void InPlaceShellTokenizeVisitor(StringSpanType mutableArgs, TVisitor&& visitor);

        template <class TString = std::basic_string<TChar, TCharTraits>, StdReimpl::invocable<TString&&> TVisitor>
        static void ShellTokenizeVisitor(const StringViewType& args, TVisitor&& visitor);

        static std::optional<StringViewType> InPlaceShellTokenizeNext(StringSpanType& mutableArgs);

        template <class TString = std::basic_string<TChar, TCharTraits>>
        static std::optional<TString> ShellTokenizeNext(const StringViewType& args, std::size_t& currentPos);
    };
}

#include <CppUtils/Misc/CommandParsing.inl>

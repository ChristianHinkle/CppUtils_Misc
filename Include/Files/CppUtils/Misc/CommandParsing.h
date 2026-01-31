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

        // Warning: Returns string views of the passed in string, so the passed in string should not be a temporary.
        static std::vector<std::basic_string_view<TChar, TCharTraits>> ShellTokenize(CppUtils::StringSpan<TChar, TCharTraits> argsStr);

        // Warning: Returns string views of the passed in string, so the passed in string should not be a temporary.
        template <StdReimpl::invocable<const std::basic_string_view<TChar, TCharTraits>&> TVisitor>
        static void ShellTokenizeVisitor(CppUtils::StringSpan<TChar, TCharTraits> argsStr, TVisitor&& visitor);

        static std::optional<std::basic_string_view<TChar, TCharTraits>> ShellTokenizeNext(CppUtils::StringSpan<TChar, TCharTraits>& argsStr);
    };
}

#include <CppUtils/Misc/CommandParsing.inl>

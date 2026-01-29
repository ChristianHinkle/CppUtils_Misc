// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>
#include <vector>
#include <CppUtils_StdReimpl/Concepts.h>
#include <optional>
#include <CppUtils_Core/StringSpan.h>

/**
 * @brief Command line parsing utilities.
 * @todo @Christian: TODO: [todo][techdebt] Make these string view functions templated to take in any char type (and any traits type).
 */
namespace CppUtils::Misc::CommandParsing
{
    // Warning: Returns string views of the passed in string, so the passed in string should not be a temporary.
    std::vector<std::string_view> ShellTokenize(CppUtils::Core::StringSpan<char> argsStr);

    // Warning: Returns string views of the passed in string, so the passed in string should not be a temporary.
    template <CppUtils::StdReimpl::Concepts::invocable<const std::string_view&> TVisitor>
    void ShellTokenizeVisitor(CppUtils::Core::StringSpan<char> argsStr, TVisitor&& visitor);

    std::optional<std::string_view> ShellTokenizeNext(CppUtils::Core::StringSpan<char>& argsStr);
}

#include <CppUtils_Misc/CommandParsing.inl>

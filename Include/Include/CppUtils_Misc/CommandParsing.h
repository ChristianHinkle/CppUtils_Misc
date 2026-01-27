// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>
#include <vector>
#include <CppUtils_StdReimpl/Concepts.h>
#include <optional>

/**
 * @brief Command line parsing utilities.
 * @todo @Christian: TODO: [todo] Implement removal of quotes and escaping chars in tokenization. This should be done by taking an actual owning string parameter of all the args and manipulating it and returning views to the different parts of it.
 * @todo @Christian: TODO: [todo][techdebt] Make these string view functions templated to take in any char type (and any traits type).
 */
namespace CppUtils::Misc::CommandParsing
{
    // Warning: Returns string views of the passed in string, so the passed in string should not be a temporary.
    std::vector<std::string_view> ShellTokenize(std::string_view argsStr);

    // Warning: Returns string views of the passed in string, so the passed in string should not be a temporary.
    template <CppUtils::StdReimpl::Concepts::invocable<const std::string_view&> TVisitor>
    void ShellTokenizeVisitor(std::string_view argsStr, TVisitor&& visitor);

    std::optional<std::string_view> ShellTokenizeNext(std::string_view& argsStr);
}

#include <CppUtils_Misc/CommandParsing.inl>

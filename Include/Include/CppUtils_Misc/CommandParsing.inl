// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc/CommandParsing.h>

#include <locale>
#include <cassert>
#include <CppUtils_Misc/String.h>

std::vector<std::string_view> CppUtils::Misc::CommandParsing::ShellTokenize(std::string_view argsStr)
{
    std::vector<std::string_view> tokens;

    ShellTokenizeVisitor(argsStr,
        [&tokens](const std::string_view& token)
        {
            tokens.push_back(token);
        }
    );

    return tokens;
}

template <CppUtils::StdReimpl::Concepts::invocable<const std::string_view&> TVisitor>
void CppUtils::Misc::CommandParsing::ShellTokenizeVisitor(std::string_view argsStr, TVisitor&& visitor)
{
    while (std::optional<std::string_view> nextToken = ShellTokenizeNext(argsStr))
    {
        visitor(*nextToken);
    }
}

std::optional<std::string_view> CppUtils::Misc::CommandParsing::ShellTokenizeNext(std::string_view& argsStr)
{
    argsStr = CppUtils::Misc::String::TrimLeadingWhitespace(argsStr);

    if (argsStr.empty())
    {
        return std::nullopt;
    }

    std::size_t pos = 0u;

    {
        std::optional<char> currentSurroundingQuote = std::nullopt;
        bool isEscapedChar = false;
        for (pos; pos < argsStr.length(); ++pos)
        {
            const char ch = argsStr[pos];

            if (isEscapedChar)
            {
                // Whatever this current char is, let it be. Continue to the next.
                isEscapedChar = false;
                continue;
            }

            if (ch == '\\')
            {
                // This is an escaping symbol for the next char.
                isEscapedChar = true;
                continue;
            }

            if (ch == '"' || ch == '\'')
            {
                if (!currentSurroundingQuote.has_value())
                {
                    // This char is an opening quote.
                    currentSurroundingQuote = ch;
                    continue;
                }

                if (ch == *currentSurroundingQuote)
                {
                    // This char is a closing quote.
                    currentSurroundingQuote.reset();
                    continue;
                }
            }

            if (!currentSurroundingQuote && std::isspace(ch))
            {
                // We hit an argument delimiter (an unescaped, unquoted space).
                break;
            }
        }
    }

    // Assert for valid indices.
    assert(pos >= 0u);
    assert(pos <= argsStr.length());

    // Store the original value of the passed in string, before it can get modified.
    const std::string_view originalArgsStr = argsStr;

    // Adjust the caller's string to be viewing the next args.
    argsStr = argsStr.substr(pos, argsStr.length() - pos);

    // Return a view of the full arg that we found.
    return originalArgsStr.substr(0u, pos);
}

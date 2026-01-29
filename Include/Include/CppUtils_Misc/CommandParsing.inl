// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc/CommandParsing.h>

#include <locale>
#include <cassert>
#include <CppUtils_Misc/String.h>
#include <CppUtils_Misc/Span.h>

std::vector<std::string_view> CppUtils::Misc::CommandParsing::ShellTokenize(CppUtils::Core::StringSpan<char> argsStr)
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
void CppUtils::Misc::CommandParsing::ShellTokenizeVisitor(CppUtils::Core::StringSpan<char> argsStr, TVisitor&& visitor)
{
    while (std::optional<std::string_view> nextToken = ShellTokenizeNext(argsStr))
    {
        visitor(*nextToken);
    }
}

std::optional<std::string_view> CppUtils::Misc::CommandParsing::ShellTokenizeNext(CppUtils::Core::StringSpan<char>& argsStr)
{
    std::span<char>& argsStrSpan = argsStr.GetSpan();

    argsStrSpan = CppUtils::Misc::String::TrimLeadingWhitespace(argsStrSpan);

    if (argsStrSpan.empty())
    {
        return std::nullopt;
    }

    std::size_t pos = 0u;

    {
        std::optional<char> currentSurroundingQuote = std::nullopt;
        bool isEscapedChar = false;
        for (; pos < argsStrSpan.size(); ++pos)
        {
            const char ch = argsStrSpan[pos];

            if (isEscapedChar)
            {
                // Whatever this current char is, let it be. Continue to the next.
                isEscapedChar = false;
                continue;
            }

            // Handle escaping symbols.
            if (ch == '\\')
            {
                // Remove this special symbol and update pos.
                CppUtils::Misc::Span::RemoveElement(argsStrSpan, pos, ' ');
                --pos;

                // This is an escaping symbol for the next char.
                isEscapedChar = true;
                continue;
            }

            // Handle quote symbols.
            if (ch == '"' || ch == '\'')
            {
                // Handle opening quotes.
                if (!currentSurroundingQuote.has_value())
                {
                    // Remove this special symbol and update pos.
                    CppUtils::Misc::Span::RemoveElement(argsStrSpan, pos, ' ');
                    --pos;

                    currentSurroundingQuote = ch;
                    continue;
                }

                // Handle closing quotes.
                if (ch == *currentSurroundingQuote)
                {
                    // Remove this special symbol and update pos.
                    CppUtils::Misc::Span::RemoveElement(argsStrSpan, pos, ' ');
                    --pos;

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

    // Assert that we have valid indices.
    assert(pos >= 0u);
    assert(pos <= argsStrSpan.size());

    // Store a view of the entire arg token that we found.
    std::string_view tokenStr = argsStr.ToStringView().substr(0u, pos);

    // Adjust the caller's string to be viewing the next args.
    argsStrSpan = argsStrSpan.subspan(pos, argsStrSpan.size() - pos);

    // Return the arg token.
    return tokenStr;
}

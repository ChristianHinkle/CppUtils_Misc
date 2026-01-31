// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/CommandParsing.h>

#include <locale>
#include <cassert>
#include <CppUtils/Misc/String.h>
#include <CppUtils/Misc/Span.h>

template <CppUtils::CharLike TChar, class TCharTraits>
std::vector<std::basic_string_view<TChar, TCharTraits>> CppUtils::CommandParsing<TChar, TCharTraits>::InPlaceShellTokenize(CppUtils::StringSpan<TChar, TCharTraits> mutableArgs)
{
    std::vector<std::basic_string_view<TChar, TCharTraits>> tokens;

    InPlaceShellTokenizeVisitor(mutableArgs,
        [&tokens](const std::basic_string_view<TChar, TCharTraits>& token)
        {
            tokens.push_back(token);
        }
    );

    return tokens;
}

template <CppUtils::CharLike TChar, class TCharTraits>
template <StdReimpl::invocable<const std::basic_string_view<TChar, TCharTraits>&> TVisitor>
void CppUtils::CommandParsing<TChar, TCharTraits>::InPlaceShellTokenizeVisitor(CppUtils::StringSpan<TChar, TCharTraits> mutableArgs, TVisitor&& visitor)
{
    while (std::optional nextToken = InPlaceShellTokenizeNext(mutableArgs))
    {
        visitor(*nextToken);
    }
}

template <CppUtils::CharLike TChar, class TCharTraits>
std::optional<std::basic_string_view<TChar, TCharTraits>> CppUtils::CommandParsing<TChar, TCharTraits>::InPlaceShellTokenizeNext(CppUtils::StringSpan<TChar, TCharTraits>& mutableArgs)
{
    std::span<TChar>& mutableArgsSpan = mutableArgs.GetSpan();

    mutableArgsSpan = CppUtils::TrimLeadingWhitespace(mutableArgsSpan);

    if (mutableArgsSpan.empty())
    {
        return std::nullopt;
    }

    std::size_t pos = 0u;

    {
        std::optional<TChar> currentSurroundingQuote = std::nullopt;
        bool isEscapedChar = false;
        for (; pos < mutableArgsSpan.size(); ++pos)
        {
            const TChar ch = mutableArgsSpan[pos];

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
                CppUtils::RemoveElement(mutableArgsSpan, pos, ' ');
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
                    CppUtils::RemoveElement(mutableArgsSpan, pos, ' ');
                    --pos;

                    currentSurroundingQuote = ch;
                    continue;
                }

                // Handle closing quotes.
                if (ch == *currentSurroundingQuote)
                {
                    // Remove this special symbol and update pos.
                    CppUtils::RemoveElement(mutableArgsSpan, pos, ' ');
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
    assert(pos <= mutableArgsSpan.size());

    // Store a view of the entire arg token that we found.
    std::basic_string_view tokenStr = mutableArgs.ToStringView().substr(0u, pos);

    // Adjust the caller's string to be viewing the next args.
    mutableArgsSpan = mutableArgsSpan.subspan(pos, mutableArgsSpan.size() - pos);

    // Return the arg token.
    return tokenStr;
}

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/CommandParsing.h>

#include <locale>
#include <cassert>
#include <CppUtils/Misc/String.h>
#include <CppUtils/Misc/Span.h>
#include <CppUtils/Misc/FunctionTraits.h>
#include <type_traits>

namespace CppUtils::Detail
{
    namespace ShellTokenizeNextPolicyTypes
    {
        template <class T>
        struct PolicyTraits
        {
            static_assert(sizeof(T) && false, "No traits specialization defined for T.");
        };

        template <
            class TTag,
            class... TPolicies
            >
        struct FindPolicyByTag;

        template <
            class TTag,
            class TFirst,
            class... TRest
            >
        struct FindPolicyByTag<TTag, TFirst, TRest...>
        {
            using type =
                std::conditional_t<
                    std::is_same_v<typename PolicyTraits<TFirst>::tag, TTag>,
                    TFirst,
                    typename FindPolicyByTag<TTag, TRest...>::type
                >;
        };

        template <
            class TTag
            >
        struct FindPolicyByTag<TTag>
        {
            // No policy found case.
            using type = void;
        };

        /**
         * @brief Tag type that represents the `Policy_HandleSpecialSymbol` policy class.
         */
        struct PolicyTag_HandleSpecialSymbol{};

        /**
         * @brief Policy for handling special symbols.
         */
        template <auto callback>
        struct Policy_HandleSpecialSymbol
        {
            using ReturnType = typename CppUtils::FunctionPtrTraits<callback>::ReturnType;

            static_assert(std::is_same_v<ReturnType, void>, "The callback should not return anything.");

            using Param1st = typename CppUtils::FunctionPtrTraits<callback>::template ArgAt<0>;

            static_assert(CppUtils::IsStdSpan<std::remove_cvref_t<Param1st>>(), "The 1st parameter should be a span for the args.");
            static_assert(CppUtils::CharLike<typename std::remove_cvref_t<Param1st>::value_type>, "The args span should be a span of chars.");
            static_assert(std::is_lvalue_reference_v<Param1st>, "The args span shoule be passed by reference.");

            using Param2nd = typename CppUtils::FunctionPtrTraits<callback>::template ArgAt<1>;

            static_assert(std::is_same_v<std::remove_cvref_t<Param2nd>, std::size_t>, "The 2nd parameter should be an index for the current string pos.");
            static_assert(std::is_lvalue_reference_v<Param2nd>, "The current string pos should be passed by reference.");

            consteval static decltype(auto) GetCallback()
            {
                return callback;
            }
        };

        template <auto callback>
        struct PolicyTraits<Policy_HandleSpecialSymbol<callback>>
        {
            using tag = PolicyTag_HandleSpecialSymbol;
        };
    }

    /**
     * @brief A policy-based implementation to allow callers to decide how and what to do with the parsed token.
     */
    template <
        CppUtils::CharLike TChar,
        class TCharTraits,
        class... TPolicies
        >
    std::optional<std::basic_string_view<TChar, TCharTraits>> ShellTokenizeNext(CppUtils::StringSpan<TChar, TCharTraits>& args);
}

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
    using MyPolicy_HandleSpecialSymbol =
        CppUtils::Detail::ShellTokenizeNextPolicyTypes::Policy_HandleSpecialSymbol<
            [](std::span<TChar>& argsSpan, std::size_t& pos)
            {
                // Remove this special symbol and update pos.
                CppUtils::RemoveElement(argsSpan, pos, ' ');
                --pos;
            }
        >;

    return CppUtils::Detail::ShellTokenizeNext<TChar, TCharTraits, MyPolicy_HandleSpecialSymbol>(mutableArgs);
}

namespace CppUtils::Detail
{
    template <
        CppUtils::CharLike TChar,
        class TCharTraits,
        class... TPolicies
        >
    std::optional<std::basic_string_view<TChar, TCharTraits>> ShellTokenizeNext(CppUtils::StringSpan<TChar, TCharTraits>& args)
    {
        namespace PolicyTypes = ShellTokenizeNextPolicyTypes;

        using FoundPolicy_HandleSpecialSymbol = PolicyTypes::FindPolicyByTag<PolicyTypes::PolicyTag_HandleSpecialSymbol, TPolicies...>::type;

        if constexpr (!std::is_void_v<FoundPolicy_HandleSpecialSymbol>)
        {
            using policyCharType = typename std::remove_cvref_t<typename FoundPolicy_HandleSpecialSymbol::Param1st>::value_type;
            static_assert(std::is_same_v<policyCharType, TChar>, "The caller's policy's callback does not match the char type of this template function.");
        }

        std::span<TChar>& argsSpan = args.GetSpan();

        argsSpan = CppUtils::TrimLeadingWhitespace(argsSpan);

        if (argsSpan.empty())
        {
            return std::nullopt;
        }

        std::size_t pos = 0u;

        {
            std::optional<TChar> currentSurroundingQuote = std::nullopt;
            bool isEscapedChar = false;
            for (; pos < argsSpan.size(); ++pos)
            {
                const TChar ch = argsSpan[pos];

                if (isEscapedChar)
                {
                    // Whatever this current char is, let it be. Continue to the next.
                    isEscapedChar = false;
                    continue;
                }

                // Handle escaping symbols.
                if (ch == '\\')
                {
                    if constexpr (!std::is_void_v<FoundPolicy_HandleSpecialSymbol>)
                    {
                        FoundPolicy_HandleSpecialSymbol::GetCallback()(argsSpan, pos);
                    }

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
                        if constexpr (!std::is_void_v<FoundPolicy_HandleSpecialSymbol>)
                        {
                            FoundPolicy_HandleSpecialSymbol::GetCallback()(argsSpan, pos);
                        }

                        currentSurroundingQuote = ch;
                        continue;
                    }

                    // Handle closing quotes.
                    if (ch == *currentSurroundingQuote)
                    {
                        if constexpr (!std::is_void_v<FoundPolicy_HandleSpecialSymbol>)
                        {
                            FoundPolicy_HandleSpecialSymbol::GetCallback()(argsSpan, pos);
                        }

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
        assert(pos <= argsSpan.size());

        // Store a view of the entire arg token that we found.
        std::basic_string_view tokenStr = args.ToStringView().substr(0u, pos);

        // Adjust the caller's string to be viewing the next args.
        argsSpan = argsSpan.subspan(pos, argsSpan.size() - pos);

        // Return the arg token.
        return tokenStr;
    }
}

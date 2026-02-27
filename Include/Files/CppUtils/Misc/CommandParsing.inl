// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils/Misc/CommandParsing.h>

#include <locale>
#include <cassert>
#include <CppUtils/Core/String.h>
#include <CppUtils/Misc/Span.h>
#include <CppUtils/Misc/FunctionTraits.h>
#include <type_traits>
#include <cstddef>
#include <CppUtils/Misc/Static_Execute.h>
#include <utility>
#include <algorithm>
#include <cstdlib>

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
        struct PolicyTag_HandleSpecialSymbol {};

        /**
         * @brief Policy for handling special symbols.
         */
        template <auto callback, class TStorageType = void>
        struct Policy_HandleSpecialSymbol
        {
            using ReturnType = typename CppUtils::FunctionPtrTraits<callback>::ReturnType;

            static_assert(std::is_same_v<ReturnType, void>, "The callback should not return anything.");

            using Param1st = typename CppUtils::FunctionPtrTraits<callback>::template ArgAt<0>;

            static_assert(CppUtils::IsStringSpan<std::remove_cvref_t<Param1st>>(), "This parameter should be a span for the args.");
            static_assert(std::is_lvalue_reference_v<Param1st>, "The args span shoule be passed by reference.");

            using Param2nd = typename CppUtils::FunctionPtrTraits<callback>::template ArgAt<1>;

            static_assert(std::is_same_v<std::remove_cvref_t<Param2nd>, std::size_t>, "This parameter should be an index for the current string pos.");
            static_assert(std::is_lvalue_reference_v<Param2nd>, "The current string pos should be passed by reference.");

            using StringSpanType = std::remove_cvref_t<Param1st>;

            using StorageType = TStorageType;

            consteval static bool UsesStorage()
            {
                return !std::is_same_v<StorageType, void>;
            }

            CPPUTILS_STATIC_EXECUTE(
                constexpr bool has3rdParam = !(CppUtils::FunctionPtrTraits<callback>::GetArgsCount() < 3);
                if constexpr (has3rdParam)
                {
                    using Param3rd = typename CppUtils::FunctionPtrTraits<callback>::template ArgAt<2>;

                    static_assert(!UsesStorage() || std::is_same_v<std::remove_cvref_t<Param3rd>, StorageType>, "This parameter should be the storage.");
                    static_assert(!UsesStorage() || std::is_lvalue_reference_v<Param3rd>, "The storage should be passed by reference.");
                }
            );

            consteval static decltype(auto) GetCallback()
            {
                return callback;
            }
        };

        template <auto callback, class TStorageType>
        struct PolicyTraits<Policy_HandleSpecialSymbol<callback, TStorageType>>
        {
            using tag = PolicyTag_HandleSpecialSymbol;
        };

        /**
         * @brief Tag type that represents the `Policy_DataStorage` policy class.
         */
        struct PolicyTag_DataStorage {};

        /**
         * @brief Policy for storing data throughout the algorithm.
         */
        template <class TStorageType>
        struct Policy_DataStorage
        {
            using StorageType = TStorageType;
        };

        template <class TStorageType>
        struct PolicyTraits<Policy_DataStorage<TStorageType>>
        {
            using tag = PolicyTag_DataStorage;
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
    std::size_t ShellTokenizeNext(CppUtils::StringSpan<TChar, TCharTraits> args);
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
template <class TString, StdReimpl::invocable<TString&&> TVisitor>
void CppUtils::CommandParsing<TChar, TCharTraits>::ShellTokenizeVisitor(const StringViewType& args, TVisitor&& visitor)
{
    std::size_t currentPos = 0u;
    while (std::optional nextToken = ShellTokenizeNext<TString>(args, currentPos))
    {
        visitor(std::move(*nextToken));
    }
}

template <CppUtils::CharLike TChar, class TCharTraits>
std::optional<std::basic_string_view<TChar, TCharTraits>> CppUtils::CommandParsing<TChar, TCharTraits>::InPlaceShellTokenizeNext(CppUtils::StringSpan<TChar, TCharTraits>& mutableArgs)
{
    std::span<TChar>& mutableArgsSpan = mutableArgs.GetSpan();

    // Trim leading whitespace first-things-first.
    mutableArgsSpan = CppUtils::TrimLeadingWhitespace(mutableArgsSpan);

    if (mutableArgsSpan.empty())
    {
        return std::nullopt;
    }

    using MyPolicy_HandleSpecialSymbol =
        CppUtils::Detail::ShellTokenizeNextPolicyTypes::Policy_HandleSpecialSymbol<
            [](StringSpanType& args, std::size_t& pos)
            {
                // Remove this special symbol and update pos.
                constexpr TChar replacementValue = ' ';
                CppUtils::RemoveElement(args.GetSpan(), pos, TChar{replacementValue});
                --pos;
            }
        >;

    std::size_t pos = CppUtils::Detail::ShellTokenizeNext<TChar, TCharTraits, MyPolicy_HandleSpecialSymbol>(mutableArgs);

    // Store a view of the entire arg token that we found.
    std::basic_string_view tokenStr = mutableArgs.ToStringView().substr(0u, pos);

    // Adjust the caller's string to be viewing the next args.
    mutableArgsSpan = mutableArgsSpan.subspan(pos, mutableArgsSpan.size() - pos);

    // Return the arg token.
    return tokenStr;
}

template <CppUtils::CharLike TChar, class TCharTraits>
template <class TString>
std::optional<TString> CppUtils::CommandParsing<TChar, TCharTraits>::ShellTokenizeNext(const StringViewType& args, std::size_t& currentPos)
{
    std::basic_string_view argsAtCurrentPos = args.substr(currentPos);

    // Trim leading whitespace first-things-first.
    currentPos += CppUtils::GetTrimmedLeadingWhitespaceStartPos(argsAtCurrentPos);
    argsAtCurrentPos = args.substr(currentPos);

    if (argsAtCurrentPos.empty())
    {
        return std::nullopt;
    }

    struct TokenStorage
    {
        TString String{};
        std::size_t NextPosToStartCopyingFrom{0u};
    };

    using MyPolicy_DataStorage =
        CppUtils::Detail::ShellTokenizeNextPolicyTypes::Policy_DataStorage<TokenStorage>;

    using MyPolicy_HandleSpecialSymbol =
        CppUtils::Detail::ShellTokenizeNextPolicyTypes::Policy_HandleSpecialSymbol<
            [](const CppUtils::StringSpan<const TChar, TCharTraits>& args, const std::size_t& pos, TokenStorage& dataStorage)
            {
                const std::size_t posToCopyUpTo = std::min<std::size_t>(pos - 1, 0u);

                // Copy the characters up to this point, from the last point.
                dataStorage.String += args.ToStringView().substr(dataStorage.NextPosToStartCopyingFrom, posToCopyUpTo);

                // Update the next pos to start copying the characters from.
                dataStorage.NextPosToStartCopyingFrom = posToCopyUpTo;
            },
            TokenStorage
        >;

    // Adjust the caller's current pos to be viewing the next args.
    currentPos += CppUtils::Detail::ShellTokenizeNext<const TChar, TCharTraits, MyPolicy_DataStorage, MyPolicy_HandleSpecialSymbol>(
        CppUtils::StringSpan<const TChar, TCharTraits>{argsAtCurrentPos.data(), argsAtCurrentPos.size()});
    argsAtCurrentPos = {};

    // TODO: Append the rest of the characters and return the token storage string.
    assert(false); // This function is unfinished!
    std::abort();
}

namespace CppUtils::Detail
{
    template <
        CppUtils::CharLike TChar,
        class TCharTraits,
        class... TPolicies
        >
    std::size_t ShellTokenizeNext(CppUtils::StringSpan<TChar, TCharTraits> args)
    {
        namespace PolicyTypes = ShellTokenizeNextPolicyTypes;

        using CharType = std::remove_cv_t<TChar>;

        using FoundPolicy_DataStorage = PolicyTypes::FindPolicyByTag<PolicyTypes::PolicyTag_DataStorage, TPolicies...>::type;

        using DataStorageType = std::remove_reference_t<decltype(
            []() consteval -> decltype(auto)
            {
                if constexpr (!std::is_void_v<FoundPolicy_DataStorage>)
                {
                    // Use the caller-specified storage type.
                    return std::declval<FoundPolicy_DataStorage::StorageType>();
                }
                else
                {
                    // Default to a bare byte for the additional storage.
                    return std::declval<std::byte>();
                }
            }()
        )>;

        using FoundPolicy_HandleSpecialSymbol = PolicyTypes::FindPolicyByTag<PolicyTypes::PolicyTag_HandleSpecialSymbol, TPolicies...>::type;
        if constexpr (!std::is_void_v<FoundPolicy_HandleSpecialSymbol>)
        {
            using policyCharType = FoundPolicy_HandleSpecialSymbol::StringSpanType::StringViewType::value_type;
            static_assert(std::is_same_v<std::remove_cvref_t<policyCharType>, std::remove_cvref_t<TChar>>, "The caller's policy's callback does not match the char type of this template function!");

            if constexpr (FoundPolicy_HandleSpecialSymbol::UsesStorage())
            {
                static_assert(std::is_same_v<typename FoundPolicy_HandleSpecialSymbol::StorageType, DataStorageType>, "The caller's policy's storage type does not match the storage type of the data storage policy!");
            }
        }

        assert(!CppUtils::HasLeadingWhitespace(args.ToStringView())); // This function assumes the source string has no leading whitespace.

        std::span<TChar>& argsSpan = args.GetSpan();

        assert(!argsSpan.empty()); // This function assumes the source string is not empty.

        DataStorageType dataStorage{};

        std::size_t pos = 0u;

        {
            std::optional<CharType> currentSurroundingQuote = std::nullopt;
            bool isEscapedChar = false;
            for (; pos < argsSpan.size(); ++pos)
            {
                const CharType ch = argsSpan[pos];

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
                        if constexpr (FoundPolicy_HandleSpecialSymbol::UsesStorage())
                        {
                            FoundPolicy_HandleSpecialSymbol::GetCallback()(args, pos, dataStorage);
                        }
                        else
                        {
                            FoundPolicy_HandleSpecialSymbol::GetCallback()(args, pos);
                        }
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
                            if constexpr (FoundPolicy_HandleSpecialSymbol::UsesStorage())
                            {
                                FoundPolicy_HandleSpecialSymbol::GetCallback()(args, pos, dataStorage);
                            }
                            else
                            {
                                FoundPolicy_HandleSpecialSymbol::GetCallback()(args, pos);
                            }
                        }

                        currentSurroundingQuote = ch;
                        continue;
                    }

                    // Handle closing quotes.
                    if (ch == *currentSurroundingQuote)
                    {
                        if constexpr (!std::is_void_v<FoundPolicy_HandleSpecialSymbol>)
                        {
                            if constexpr (FoundPolicy_HandleSpecialSymbol::UsesStorage())
                            {
                                FoundPolicy_HandleSpecialSymbol::GetCallback()(args, pos, dataStorage);
                            }
                            else
                            {
                                FoundPolicy_HandleSpecialSymbol::GetCallback()(args, pos);
                            }
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

        // Return the last pos read.
        return pos;
    }
}

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_Misc_Export.h>
#include <string_view>
#include <array>
#include <CppUtils/Core/Concepts.h>
#include <CppUtils/Core/StringSpan.h>

namespace CppUtils
{
    /**
     * @brief An in-place character buffer which can be manipulated and viewed as a string.
     */
    template <CppUtils::CharLike TChar, std::size_t bufferSize, class TTraits = std::char_traits<TChar>>
    struct CharBufferString
    {
    public:

        CharBufferString() = default;

        /**
         * @brief An overload to support prvalue semantics, allowing callers to initialize this
         *        object manually before it's returned by the constructor.
         */
        template <class TFunctor>
        CharBufferString(TFunctor&& callbackFunctor)
            : CharBufferString()
        {
            callbackFunctor(*this);
        }

    public:

        std::array<TChar, bufferSize>::iterator begin()
        {
            return CharBuffer.begin();
        }

        std::array<TChar, bufferSize>::const_iterator begin() const
        {
            return CharBuffer.begin();
        }

        std::array<TChar, bufferSize>::iterator end()
        {
            return CharBuffer.begin() + Length;
        }

        std::array<TChar, bufferSize>::const_iterator end() const
        {
            return CharBuffer.begin() + Length;
        }

    public:

        std::basic_string_view<TChar, TTraits> ToStringView() const
        {
            return std::basic_string_view<TChar, TTraits>(CharBuffer.data(), Length);
        }

        CppUtils::StringSpan<TChar, TTraits> ToStringSpan() const
        {
            return CppUtils::StringSpan<TChar, TTraits>(CharBuffer.data(), Length);
        }

        const std::array<TChar, bufferSize>& GetCharBuffer() const
        {
            return CharBuffer;
        }

        std::array<TChar, bufferSize>& GetCharBufferMutable()
        {
            return CharBuffer;
        }

        std::size_t GetLength() const
        {
            return Length;
        }

        std::size_t& GetLengthMutable()
        {
            return Length;
        }

    private:

        std::array<TChar, bufferSize> CharBuffer{};

        std::size_t Length{0u};
    };
}

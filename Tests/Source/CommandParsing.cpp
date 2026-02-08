// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils/Misc/CommandParsing.h>
#include <CppUtils/Misc/CharBufferString.h>

#define COMPLICATED_TEST_ARGS_STRING_LITERAL R"(arg1 arg2 "arg 3" 'arg 4' \"arg\ 5\" \'arg\ 6\' arg7 "arg \"8\"" "arg \\\"9\\\"" arg"10")";

int main(int argc, char** argv)
{
    int testCode = 0;

    {
        char testArgs[] = R"(arg1 arg2 arg3 arg4)";

        std::vector<std::string_view> tokens =
            CppUtils::CommandParsing<char>::InPlaceShellTokenize(testArgs);

        ++testCode;
        if (tokens.size() != 4)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != R"(arg1)")
        {
            return testCode;
        }
    }

    {
        char testArgs[] = R"(    arg1    arg2  arg3 arg4    )";

        std::vector<std::string_view> tokens =
            CppUtils::CommandParsing<char>::InPlaceShellTokenize(testArgs);

        ++testCode;
        if (tokens.size() != 4)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != R"(arg1)")
        {
            return testCode;
        }
    }

    {
        char testArgs[] = R"("arg with spaces")";

        std::vector<std::string_view> tokens =
            CppUtils::CommandParsing<char>::InPlaceShellTokenize(testArgs);

        ++testCode;
        if (tokens.size() != 1)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != R"(arg with spaces)")
        {
            return testCode;
        }
    }

    {
        char testArgs[] = R"(arg" "with" "spaces)";

        std::vector<std::string_view> tokens =
            CppUtils::CommandParsing<char>::InPlaceShellTokenize(testArgs);

        ++testCode;
        if (tokens.size() != 1)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != R"(arg with spaces)")
        {
            return testCode;
        }
    }

    {
        char testArgs[] = R"(arg\ with\ spaces)";

        std::vector<std::string_view> tokens =
            CppUtils::CommandParsing<char>::InPlaceShellTokenize(testArgs);

        ++testCode;
        if (tokens.size() != 1)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != R"(arg with spaces)")
        {
            return testCode;
        }
    }

    {
        char testArgs[] = COMPLICATED_TEST_ARGS_STRING_LITERAL;

        std::vector<std::string_view> tokens =
            CppUtils::CommandParsing<char>::InPlaceShellTokenize(testArgs);

        ++testCode;
        if (tokens.size() != 10)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != R"(arg1)")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[1] != R"(arg2)")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[2] != R"(arg 3)")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[3] != R"(arg 4)")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[4] != R"("arg 5")")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[5] != R"('arg 6')")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[6] != R"(arg7)")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[7] != R"(arg "8")")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[8] != R"(arg \"9\")")
        {
            return testCode;
        }

        ++testCode;
        if (tokens[9] != R"(arg10)")
        {
            return testCode;
        }
    }

    {
        char testArgs[] = COMPLICATED_TEST_ARGS_STRING_LITERAL;

        std::vector<std::string> tokensFreeStoreAllocated{};

        CppUtils::CommandParsing<char>::ShellTokenizeVisitor(testArgs,
            [&tokensFreeStoreAllocated](std::string&& token)
            {
                tokensFreeStoreAllocated.push_back(std::move(token));
            }
        );

        std::vector<std::string_view> tokensInPlace =
            CppUtils::CommandParsing<char>::InPlaceShellTokenize(testArgs);

        ++testCode;
        if (tokensFreeStoreAllocated.size() != tokensInPlace.size())
        {
            return testCode;
        }

        for (std::size_t i = 0u; i < tokensInPlace.size(); ++i)
        {
            ++testCode;
            if (tokensFreeStoreAllocated[i] != tokensInPlace[i])
            {
                return testCode;
            }
        }
    }

    return 0;
}

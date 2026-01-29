// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils_Misc/CommandParsing.h>
#include <CppUtils_Misc/CharBufferString.h>

int main(int argc, char** argv)
{
    int testCode = 0;

    {
        char testArgs[] = R"(arg1 arg2 arg3 arg4)";

        std::vector<std::string_view> tokens =
            CppUtils::Misc::CommandParsing::ShellTokenize(testArgs);

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
            CppUtils::Misc::CommandParsing::ShellTokenize(testArgs);

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
            CppUtils::Misc::CommandParsing::ShellTokenize(testArgs);

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
            CppUtils::Misc::CommandParsing::ShellTokenize(testArgs);

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
            CppUtils::Misc::CommandParsing::ShellTokenize(testArgs);

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
        char testArgs[] = R"(arg1 arg2 "arg 3" 'arg 4' \"arg\ 5\" \'arg\ 6\' arg7 "arg \"8\"" "arg \\\"9\\\"" arg"10")";

        std::vector<std::string_view> tokens =
            CppUtils::Misc::CommandParsing::ShellTokenize(testArgs);

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

    return 0;
}

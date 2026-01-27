// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils_Misc/CommandParsing.h>

int main(int argc, char** argv)
{
    int testCode = 0;

    {
        std::vector<std::string_view> tokens = CppUtils::Misc::CommandParsing::ShellTokenize(
            R"(arg1 arg2 arg3 arg4)"
        );


        ++testCode;
        if (tokens.size() != 4)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != "arg1")
        {
            return testCode;
        }
    }

    {
        std::vector<std::string_view> tokens = CppUtils::Misc::CommandParsing::ShellTokenize(
            R"(    arg1    arg2  arg3 arg4    )"
        );


        ++testCode;
        if (tokens.size() != 4)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != "arg1")
        {
            return testCode;
        }
    }

    {
        std::vector<std::string_view> tokens = CppUtils::Misc::CommandParsing::ShellTokenize(
            R"("arg with spaces")"
        );

        ++testCode;
        if (tokens.size() != 1)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != "arg with spaces")
        {
            return testCode;
        }
    }

    {
        std::vector<std::string_view> tokens = CppUtils::Misc::CommandParsing::ShellTokenize(
            R"(arg" "with" "spaces)"
        );

        ++testCode;
        if (tokens.size() != 1)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != "arg with spaces")
        {
            return testCode;
        }
    }

    {
        std::vector<std::string_view> tokens = CppUtils::Misc::CommandParsing::ShellTokenize(
            R"(arg\ with\ spaces)"
        );

        ++testCode;
        if (tokens.size() != 1)
        {
            return testCode;
        }

        ++testCode;
        if (tokens[0] != "arg with spaces")
        {
            return testCode;
        }
    }

    {
        //
        // The following args list
        //
        // ```
        // arg1 arg2 "arg 3" 'arg 4' \"arg\ 5\" \'arg\ 6\' arg7 "arg \"8\"" "arg \\\"9\\\"" arg"10"
        // ```
        //
        // should result in the following array
        //
        // {
        //     R"(arg1)",
        //     R"(arg2)",
        //     R"(arg 3)",
        //     R"(arg 4)",
        //     R"("arg 5")",
        //     R"('arg 6')",
        //     R"(arg7)",
        //     R"(arg "8")",
        //     R"(arg \"9\")",
        //     R"(arg10)"
        // }
        //

        std::vector<std::string_view> tokens = CppUtils::Misc::CommandParsing::ShellTokenize(
            R"(arg1 arg2 "arg 3" 'arg 4' \"arg\ 5\" \'arg\ 6\' arg7 "arg \"8\"" "arg \\\"9\\\"" arg"10")"
        );
    }

    return 0;
}

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils/Misc/Math.h>

#include <limits>
#include <cmath>
#include <iostream>

int main(int argc, char** argv)
{
    int testCode = 0;

    // Basic sanity checks. Make sure the division behaves as expected for the simple cases.

    ++testCode;
    if (CppUtils::IeeeDivide(1.f, 1.f) != 1.f / 1.f)
    {
        std::cout << "Subtest `" << testCode << "` failed." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(1561.0, 18.0) != 1561.0 / 18.0)
    {
        std::cout << "Subtest `" << testCode << "` failed." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(.0000321l, .0123l) != .0000321l / .0123l)
    {
        std::cout << "Subtest `" << testCode << "` failed." << std::endl;
        return testCode;
    }

    // Now, test the edge cases.

    ++testCode;
    if (CppUtils::IeeeDivide(1.f, 0.f) != std::numeric_limits<float>::infinity())
    {
        constexpr char caseText[] = "1 / 0";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(-1.f, 0.f) != -std::numeric_limits<float>::infinity())
    {
        constexpr char caseText[] = "-1 / 0";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(1.f, -0.f) != -std::numeric_limits<float>::infinity())
    {
        constexpr char caseText[] = "1 / -0";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(-1.f, -0.f) != std::numeric_limits<float>::infinity())
    {
        constexpr char caseText[] = "-1 / -0";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(0.f, 0.f)))
    {
        constexpr char caseText[] = "0 / 0";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(-0.f, 0.f)))
    {
        constexpr char caseText[] = "-0 / 0";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(0.f, -0.f)))
    {
        constexpr char caseText[] = "0 / -0";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(-0.f, -0.f)))
    {
        constexpr char caseText[] = "-0 / -0";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity())))
    {
        constexpr char caseText[] = "inf / inf";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity())))
    {
        constexpr char caseText[] = "-inf / inf";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity())))
    {
        constexpr char caseText[] = "inf / -inf";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity())))
    {
        constexpr char caseText[] = "-inf / -inf";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(1.f, std::numeric_limits<float>::infinity()) != 0.f)
    {
        constexpr char caseText[] = "1 / inf";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(-1.f, std::numeric_limits<float>::infinity()) != -0.f)
    {
        constexpr char caseText[] = "-1 / inf";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(1.f, -std::numeric_limits<float>::infinity()) != -0.f)
    {
        constexpr char caseText[] = "1 / -inf";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (CppUtils::IeeeDivide(-1.f, -std::numeric_limits<float>::infinity()) != 0.f)
    {
        constexpr char caseText[] = "-1 / -inf";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(std::numeric_limits<float>::quiet_NaN(), 1.f)))
    {
        constexpr char caseText[] = "NaN / 1";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(1.f, std::numeric_limits<float>::quiet_NaN())))
    {
        constexpr char caseText[] = "1 / NaN";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN())))
    {
        constexpr char caseText[] = "NaN / NaN";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    ++testCode;
    if (!std::isnan(CppUtils::IeeeDivide(-std::numeric_limits<float>::quiet_NaN(), -std::numeric_limits<float>::quiet_NaN())))
    {
        constexpr char caseText[] = "-NaN / -NaN";
        std::cout << "Subtest `" << testCode << "` failed. Case: `"<< caseText << "`." << std::endl;
        return testCode;
    }

    return 0;
}

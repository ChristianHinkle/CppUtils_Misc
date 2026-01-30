// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

/*
* Utility macro for compile-time execution, commonly used for the reuse of static_asserts by invoking them through a consteval wrapper.
* This is done via a dummy static_assert, so execution moment is identical static_asserts.
* 
* Some example scenarios:
*   In global scope: Semantic analysis (before code generation, but after parsing).
*   In class: During class definition analysis.
*   In function: Checked when the function body is compiled, even if the function is never called.
*   Inside templates which we don't depend on: Template definition time (when the template is defined, not when it’s instantiated).
*   Inside templates which we do    depend on: Template instantiation time (when we know the template arguments due to the instantiation).
*
* NOTE: Though users only give one argument conceptually, it can potentially be parsed as multiple arguments for certain expressions.
*       To resolve this, the macro accepts variatic arguments.
*/
#define CPPUTILS_STATIC_EXECUTE(...) static_assert([](){ __VA_ARGS__; return true; }());

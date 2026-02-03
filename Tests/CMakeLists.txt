# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

# Write out the main project name here, in one place, to make renaming easier. This is mainly used by our
# boilerplate code. There may be other mentions of the project's name mixed in certain places of this file, so
# don't rely on these variables alone for renaming.
SET(MY_BASE_PROJECT_NAME_FULL "CppUtils_Misc")
string(TOLOWER ${MY_BASE_PROJECT_NAME_FULL} MY_BASE_PROJECT_NAME_FULL_LOWERCASE)
string(TOUPPER ${MY_BASE_PROJECT_NAME_FULL} MY_BASE_PROJECT_NAME_FULL_UPPERCASE)
SET(MY_BASE_PROJECT_NAME_NAMESPACE "CppUtils")
SET(MY_BASE_PROJECT_NAME_LEAFNAME "Misc")

project(${MY_BASE_PROJECT_NAME_FULL}_Tests
  VERSION 0.1.0
  LANGUAGES CXX
  )

# Note that we do not "find package" for our parent project. We don't need to since we are built in the same
# cmake invocation as the it. That means we're being processed during the same configuration step as them, which
# means we'll have all their targets. Also, the targets that we reference in `target_link_libraries` commands don't
# even need to exist yet because CMake only validates the existence of targets at the end of the entire
# configuration process.

add_executable(${MY_BASE_PROJECT_NAME_FULL}_CommandParsing)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_CommandParsing PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_CommandParsing PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/CommandParsing.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_CommandParsing
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Static
  )

add_test(NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.CommandParsing
  COMMAND ${MY_BASE_PROJECT_NAME_FULL}_CommandParsing
  )

add_library(${MY_BASE_PROJECT_NAME_FULL}_TypeProbing STATIC EXCLUDE_FROM_ALL)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_TypeProbing PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_TypeProbing PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/TypeProbing.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_TypeProbing
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Include
  )

add_test(
  NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.TypeProbing
  COMMAND ${CMAKE_COMMAND}
    --build ${CMAKE_CURRENT_BINARY_DIR}
    --target ${MY_BASE_PROJECT_NAME_FULL}_TypeProbing
  )

add_library(${MY_BASE_PROJECT_NAME_FULL}_StaticExecute STATIC EXCLUDE_FROM_ALL)
target_compile_features(${MY_BASE_PROJECT_NAME_FULL}_StaticExecute PUBLIC cxx_std_20)
target_sources(${MY_BASE_PROJECT_NAME_FULL}_StaticExecute PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/StaticExecute.cpp")
target_link_libraries(${MY_BASE_PROJECT_NAME_FULL}_StaticExecute
  PRIVATE
    ${MY_BASE_PROJECT_NAME_NAMESPACE}::${MY_BASE_PROJECT_NAME_LEAFNAME}::Include
  )

add_test(
  NAME ${MY_BASE_PROJECT_NAME_NAMESPACE}.${MY_BASE_PROJECT_NAME_LEAFNAME}.StaticExecute
  COMMAND ${CMAKE_COMMAND}
    --build ${CMAKE_CURRENT_BINARY_DIR}
    --target ${MY_BASE_PROJECT_NAME_FULL}_StaticExecute
  )

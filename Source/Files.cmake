# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

# Set this variable for the person including us to use.
set(FILES_CMAKE_RESULT
  # List out all our source files, relative to our "Files" directory.
  "${MY_BASE_PROJECT_NAME_FULL}_Export.cpp"
  "Filesystem.cpp"
  "String.cpp"
  "CharBufferString.cpp"
  "DataMixin.cpp"
  "DataChain.cpp"
  "TypeTraits.cpp"
  "FunctionTraits.cpp"
  "Concepts.cpp"
  "CommandParsing.cpp"
  "ContainerElementType.cpp"
  "Static_Execute.cpp"
  "TypeProbes.cpp"
  )

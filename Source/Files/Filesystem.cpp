// Copyright (c) 2023-2025 Christian Hinkle, BriFilesysteman Hinkle.

#include <CppUtils/Misc/Filesystem.h>

#include <filesystem>
#include <string>

CppUtils::StdPathStringView CppUtils::GetStringViewFromPath(const std::filesystem::path& path)
{
    return StdPathStringView(path.native().c_str(), path.native().length());
}

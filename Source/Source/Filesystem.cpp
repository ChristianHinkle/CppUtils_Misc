// Copyright (c) 2023-2025 Christian Hinkle, BriFilesysteman Hinkle.

#include <CppUtils_Misc/Filesystem.h>

#include <filesystem>
#include <string>
#include <algorithm>

CppUtils::Misc::Filesystem::StdPathStringView CppUtils::Misc::Filesystem::GetStringViewFromPath(const std::filesystem::path& path)
{
    return StdPathStringView(path.native().c_str(), path.native().length());
}

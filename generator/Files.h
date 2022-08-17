#pragma once
#include "Common.h"

using InputPaths = Expected<std::pair<std::filesystem::path, std::filesystem::path>>;
using FStreams   = Expected<std::tuple<std::ifstream, std::ofstream, std::ofstream>>;

FStreams get_files(int argc, const char** argv);

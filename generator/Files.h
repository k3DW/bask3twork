#pragma once
#include "Forward.h"
#include <filesystem>

using InputPaths = std::pair<std::filesystem::path, std::filesystem::path>;
using FStreams   = std::tuple<std::ifstream, std::ofstream, std::ofstream>;

Expected<FStreams> get_files(int argc, const char** argv);

#pragma once
#include <array>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

template <class T>
using Expected = std::expected<T, std::string>;

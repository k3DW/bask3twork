#pragma once
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <utility>

template <class T>
using Expected = std::expected<T, std::string>;

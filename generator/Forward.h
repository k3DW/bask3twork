#pragma once
#include <expected>
#include <map>
#include <string>
#include <vector>



// Expected
template <class T>
using Expected = std::expected<T, std::string>;



// Processing
struct ProcessedLine;

using CodepointToIndex = std::map<int, int>;
using ProcessedLines   = std::vector<ProcessedLine>;
using ProcessedData    = std::pair<CodepointToIndex, ProcessedLines>;

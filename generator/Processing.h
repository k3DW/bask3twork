#pragma once
#include "Common.h"

using InputLines = std::vector<std::vector<std::string>>;

struct ProcessedLine
{
	int codepoint;
	int rotate_90_index;
	int rotate_180_index;
	int mirror_x_index;
	int mirror_y_index;
	int mirror_forward_diag_index;
	int mirror_backward_diag_index;
	std::string up_connection;
	std::string down_connection;
	std::string left_connection;
	std::string right_connection;
};

using CodepointToIndex = std::map<int, int>;
using ProcessedLines   = std::vector<ProcessedLine>;
using ProcessedData    = std::pair<CodepointToIndex, ProcessedLines>;

Expected<ProcessedData> get_processed_data(std::ifstream& csv_file);

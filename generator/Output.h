#pragma once
#include "Forward.h"

void output_all_glyphs(std::ofstream& all_glyphs_file, const ProcessedLines& processed_input_lines);

void output_unichar_to_glyphs(std::ofstream& unichar_to_glyphs_file, const CodepointToIndex& codepoint_to_index);

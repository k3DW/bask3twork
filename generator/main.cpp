#include "Files.h"
#include "Processing.h"

#include <iostream>



void output_all_glyphs(std::ofstream& all_glyphs_file, const ProcessedLines& processed_input_lines)
{
	std::array<std::size_t, 11> max_sizes = {};

	for (auto& line : processed_input_lines)
	{
		max_sizes[0] = std::max(max_sizes[0], std::to_string(line.codepoint).size());
		max_sizes[1] = std::max(max_sizes[1], std::to_string(line.rotate_90_index).size());
		max_sizes[2] = std::max(max_sizes[2], std::to_string(line.rotate_180_index).size());
		max_sizes[3] = std::max(max_sizes[3], std::to_string(line.mirror_x_index).size());
		max_sizes[4] = std::max(max_sizes[4], std::to_string(line.mirror_y_index).size());
		max_sizes[5] = std::max(max_sizes[5], std::to_string(line.mirror_forward_diag_index).size());
		max_sizes[6] = std::max(max_sizes[6], std::to_string(line.mirror_backward_diag_index).size());
		max_sizes[7] = std::max(max_sizes[7], line.up_connection.size());
		max_sizes[8] = std::max(max_sizes[8], line.down_connection.size());
		max_sizes[9] = std::max(max_sizes[9], line.left_connection.size());
		max_sizes[10] = std::max(max_sizes[10], line.right_connection.size());
	}

	all_glyphs_file << std::format("constexpr std::array<Glyph, {}> AllGlyphs =\n", processed_input_lines.size());
	all_glyphs_file << "{\n";

	for (auto& line : processed_input_lines)
	{
		all_glyphs_file << "\tGlyph( ";

		all_glyphs_file << std::format("{:{}}", std::to_string(line.codepoint) + ", ", max_sizes[0] + 2);

		auto rotate_90_index = std::format("&AllGlyphs[{}], ", line.rotate_90_index);
		auto rotate_180_index = std::format("&AllGlyphs[{}], ", line.rotate_180_index);
		auto mirror_x_index = std::format("&AllGlyphs[{}], ", line.mirror_x_index);
		auto mirror_y_index = std::format("&AllGlyphs[{}], ", line.mirror_y_index);
		auto mirror_forward_diag_index = std::format("&AllGlyphs[{}], ", line.mirror_forward_diag_index);
		auto mirror_backward_diag_index = std::format("&AllGlyphs[{}]", line.mirror_backward_diag_index);

		all_glyphs_file << "{ ";
		all_glyphs_file << std::format("{:{}}", rotate_90_index, max_sizes[1] + 14);
		all_glyphs_file << std::format("{:{}}", rotate_180_index, max_sizes[2] + 14);
		all_glyphs_file << std::format("{:{}}", mirror_x_index, max_sizes[3] + 14);
		all_glyphs_file << std::format("{:{}}", mirror_y_index, max_sizes[4] + 14);
		all_glyphs_file << std::format("{:{}}", mirror_forward_diag_index, max_sizes[5] + 14);
		all_glyphs_file << std::format("{:{}}", mirror_backward_diag_index, max_sizes[6] + 12);
		all_glyphs_file << " }, ";

		all_glyphs_file << std::format("Connection::{:{}}", line.up_connection + ", ", max_sizes[7] + 2);
		all_glyphs_file << std::format("Connection::{:{}}", line.down_connection + ", ", max_sizes[8] + 2);
		all_glyphs_file << std::format("Connection::{:{}}", line.left_connection + ", ", max_sizes[9] + 2);
		all_glyphs_file << std::format("Connection::{:{}}", line.right_connection, max_sizes[10]);

		all_glyphs_file << " ),\n";
	}

	all_glyphs_file << "};\n";
}

void output_unichar_to_glyphs(std::ofstream& unichar_to_glyphs_file, const CodepointToIndex& codepoint_to_index)
{
	std::array<std::size_t, 2> max_sizes = {};

	for (auto [codepoint, index] : codepoint_to_index)
	{
		max_sizes[0] = std::max(max_sizes[0], std::to_string(codepoint).size());
		max_sizes[1] = std::max(max_sizes[1], std::to_string(index).size());
	}

	unichar_to_glyphs_file << "inline const std::map<wxUniChar, const Glyph*> UnicharToGlyph =\n";
	unichar_to_glyphs_file << "{\n";

	for (auto& line : codepoint_to_index)
	{
		unichar_to_glyphs_file << "\t{ ";

		auto codepoint = std::format("wxUniChar({}), ", line.first);
		auto glyph = std::format("&AllGlyphs[{}]", line.second);

		unichar_to_glyphs_file << std::format("{:{}}", codepoint, max_sizes[0] + 13);
		unichar_to_glyphs_file << std::format("{:{}}", glyph, max_sizes[1] + 12);

		unichar_to_glyphs_file << " },\n";
	}

	unichar_to_glyphs_file << "};\n";
}



#define return_if_error(_exception_)          \
if (not _exception_.has_value())              \
{                                             \
	std::cout << _exception_.error() << "\n"; \
	return EXIT_FAILURE;                      \
}



int main(int argc, const char** argv)
{
	Expected<FStreams> files = get_files(argc, argv);
	return_if_error(files);

	auto& [csv_file, all_glyphs_file, unichar_to_glyphs_file] = files.value();

	Expected<ProcessedData> processed_data = get_processed_data(csv_file);
	return_if_error(processed_data);

	const auto& [codepoint_to_index, processed_lines] = processed_data.value();

	output_all_glyphs(all_glyphs_file, processed_lines);
	output_unichar_to_glyphs(unichar_to_glyphs_file, codepoint_to_index);

	std::cout << R"(Bask3twork successfully generated "AllGlyphs.impl" and "UnicharToGlyph.impl")" << "\n";
	return EXIT_SUCCESS;
}

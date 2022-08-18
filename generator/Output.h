#pragma once
#include "Processing.h"

#include <iostream>

static size_t size(int i)                  { return std::to_string(i).size(); }
static size_t size(const std::string& str) { return str.size(); }

template <class T, class... Us>
static std::array<std::size_t, sizeof...(Us)> get_max_sizes(const T& t, Us T::value_type::*...members)
{
	std::array<std::size_t, sizeof...(Us)> max_sizes = {};

	for (const typename T::value_type& elem : t)
	{
		std::array<std::size_t, sizeof...(Us)> elem_sizes = { size(elem.*members)... };

		static constexpr auto max_fn = [](std::size_t a, std::size_t b) -> std::size_t { return std::max(a, b); };
		std::ranges::transform(max_sizes, elem_sizes, max_sizes.begin(), max_fn);
	}

	return max_sizes;
}



void output_all_glyphs(std::ofstream& all_glyphs_file, const ProcessedLines& processed_input_lines)
{
	using T = ProcessedLines::value_type;
	const std::array<std::size_t, 11> max_sizes = get_max_sizes(processed_input_lines
		, &T::codepoint, &T::rotate_90_index, &T::rotate_180_index, &T::mirror_x_index, &T::mirror_y_index, &T::mirror_forward_diag_index, &T::mirror_backward_diag_index
		, &T::up_connection, &T::down_connection, &T::left_connection, &T::right_connection
	);

	all_glyphs_file << std::format("constexpr std::array<Glyph, {}> AllGlyphs =\n", processed_input_lines.size());
	all_glyphs_file << "{\n";

	for (auto& line : processed_input_lines)
	{
		all_glyphs_file << "\tGlyph( ";

		all_glyphs_file << std::format("{:{}}", std::to_string(line.codepoint) + ", ", max_sizes[0] + 2);

		auto rotate_90_index = std::format("&AllGlyphs[{}]", line.rotate_90_index) + ", ";
		auto rotate_180_index = std::format("&AllGlyphs[{}]", line.rotate_180_index) + ", ";
		auto mirror_x_index = std::format("&AllGlyphs[{}]", line.mirror_x_index) + ", ";
		auto mirror_y_index = std::format("&AllGlyphs[{}]", line.mirror_y_index) + ", ";
		auto mirror_forward_diag_index = std::format("&AllGlyphs[{}]", line.mirror_forward_diag_index) + ", ";
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
	using T = CodepointToIndex::value_type;
	const std::array<std::size_t, 2> max_sizes = get_max_sizes(codepoint_to_index, &T::first, &T::second);

	unichar_to_glyphs_file << "inline const std::map<wxUniChar, const Glyph*> UnicharToGlyph =\n";
	unichar_to_glyphs_file << "{\n";

	for (auto& line : codepoint_to_index)
	{
		auto codepoint = std::format("wxUniChar({}), ", line.first);
		auto glyph = std::format("&AllGlyphs[{}]", line.second);

		unichar_to_glyphs_file << "\t{ ";
		unichar_to_glyphs_file << std::format("{:{}}", codepoint, max_sizes[0] + 13);
		unichar_to_glyphs_file << std::format("{:{}}", glyph, max_sizes[1] + 12);
		unichar_to_glyphs_file << " },\n";
	}

	unichar_to_glyphs_file << "};\n";
}

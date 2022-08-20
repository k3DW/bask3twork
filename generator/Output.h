#pragma once
#include "Processing.h"

#include <iostream>

static std::string        get_string(int i)                  { return std::to_string(i); }
static const std::string& get_string(const std::string& str) { return str; }

template <class Collection, std::size_t Width = std::tuple_size_v<typename Collection::value_type>>
static std::array<std::size_t, Width> get_max_sizes(const Collection& collection)
{
	static constexpr auto get_element_sizes = []<std::size_t... Is>(std::index_sequence<Is...>)
	{
		return [](auto& element) -> std::array<std::size_t, Width>
		{
			return { get_string(get<Is>(element)).size()... };
		};
	}(std::make_index_sequence<Width>{});

	std::array<std::size_t, Width> max_sizes = {};
	for (const auto& elem : collection)
	{
		static constexpr auto max_fn = [](std::size_t a, std::size_t b) -> std::size_t { return std::max(a, b); };
		std::ranges::transform(max_sizes, get_element_sizes(elem), max_sizes.begin(), max_fn);
	}
	return max_sizes;
}



const auto formatted = [](std::size_t max_size, std::string_view format, auto value) -> std::string
{
	std::string str = get_string(value);
	std::string formatted = std::vformat(format, std::make_format_args(str));
	if (std::size_t padding = max_size - str.size(); padding != 0)
		return std::format("{}{:{}}", formatted, "", padding);
	else
		return formatted;
};

void output_all_glyphs(std::ofstream& all_glyphs_file, const ProcessedLines& processed_input_lines)
{
	const std::array<std::size_t, 11> max_sizes = get_max_sizes(processed_input_lines);

	all_glyphs_file << std::format("constexpr std::array<Glyph, {}> AllGlyphs =\n", processed_input_lines.size());
	all_glyphs_file << "{\n";

	for (auto& line : processed_input_lines)
	{
		all_glyphs_file << "\tGlyph( ";

		all_glyphs_file << formatted(max_sizes[0], "{}, ", line.codepoint);

		all_glyphs_file << "{ ";
		all_glyphs_file << formatted(max_sizes[1], "&AllGlyphs[{}], ", line.rotate_90_index);
		all_glyphs_file << formatted(max_sizes[2], "&AllGlyphs[{}], ", line.rotate_180_index);
		all_glyphs_file << formatted(max_sizes[3], "&AllGlyphs[{}], ", line.mirror_x_index);
		all_glyphs_file << formatted(max_sizes[4], "&AllGlyphs[{}], ", line.mirror_y_index);
		all_glyphs_file << formatted(max_sizes[5], "&AllGlyphs[{}], ", line.mirror_forward_diag_index);
		all_glyphs_file << formatted(max_sizes[6], "&AllGlyphs[{}]", line.mirror_backward_diag_index);
		all_glyphs_file << " }, ";

		all_glyphs_file << formatted(max_sizes[7], "Connection::{}, ", line.up_connection);
		all_glyphs_file << formatted(max_sizes[8], "Connection::{}, ", line.down_connection);
		all_glyphs_file << formatted(max_sizes[9], "Connection::{}, ", line.left_connection);
		all_glyphs_file << formatted(max_sizes[10], "Connection::{}", line.right_connection);

		all_glyphs_file << " ),\n";
	}

	all_glyphs_file << "};\n";
}

void output_unichar_to_glyphs(std::ofstream& unichar_to_glyphs_file, const CodepointToIndex& codepoint_to_index)
{
	const std::array<std::size_t, 2> max_sizes = get_max_sizes(codepoint_to_index);

	unichar_to_glyphs_file << "inline const std::map<wxUniChar, const Glyph*> UnicharToGlyph =\n";
	unichar_to_glyphs_file << "{\n";

	for (auto& [codepoint, glyph] : codepoint_to_index)
	{
		unichar_to_glyphs_file << "\t{ ";
		unichar_to_glyphs_file << formatted(max_sizes[0], "wxUniChar({}), ", codepoint);
		unichar_to_glyphs_file << formatted(max_sizes[1], "&AllGlyphs[{}]", glyph);
		unichar_to_glyphs_file << " },\n";
	}

	unichar_to_glyphs_file << "};\n";
}

#include "pch.h"
#include "Output.h"
#include "Processing.h"

static std::string        get_string(int i)                  { return std::to_string(i); }
static const std::string& get_string(const std::string& str) { return str; }

template <class Collection>
class OutputFormatter
{
public:
	using line_type = typename Collection::value_type;
	static constexpr std::size_t width = std::tuple_size_v<line_type>;
	using array_type = std::array<std::size_t, width>;

	OutputFormatter(const Collection& collection) : max_sizes(get_max_sizes(collection)) {}

	template <std::size_t I>
	std::string format(const line_type& line, std::string_view formatting) const
	{
		std::string str = get_string(get<I>(line));
		std::string formatted = std::vformat(formatting, std::make_format_args(str));
		if (std::size_t padding = get<I>(max_sizes) - str.size(); padding != 0)
			return std::format("{}{:{}}", formatted, "", padding);
		else
			return formatted;
	}

private:
	static array_type get_max_sizes(const Collection& collection)
	{
		static constexpr auto get_element_sizes = []<std::size_t... Is>(std::index_sequence<Is...>)
		{
			return[](auto& element)->array_type
			{
				return { get_string(get<Is>(element)).size()... };
			};
		}(std::make_index_sequence<width>{});

		array_type max_sizes = {};
		for (const auto& elem : collection)
		{
			static constexpr auto max_fn = [](std::size_t a, std::size_t b) -> std::size_t { return std::max(a, b); };
			std::ranges::transform(max_sizes, get_element_sizes(elem), max_sizes.begin(), max_fn);
		}
		return max_sizes;
	}

	array_type max_sizes;
};



void output_all_glyphs(std::ofstream& all_glyphs_file, const ProcessedLines& processed_input_lines)
{
	const OutputFormatter formatter(processed_input_lines);

	all_glyphs_file << std::format("constexpr std::array<Glyph, {}> AllGlyphs =\n", processed_input_lines.size());
	all_glyphs_file << "{\n";

	for (auto& line : processed_input_lines)
	{
		all_glyphs_file << "\tGlyph( ";

		all_glyphs_file << formatter.format<0>(line, "{}, ");

		all_glyphs_file << "{ ";
		all_glyphs_file << formatter.format<1>(line, "&AllGlyphs[{}], ");
		all_glyphs_file << formatter.format<2>(line, "&AllGlyphs[{}], ");
		all_glyphs_file << formatter.format<3>(line, "&AllGlyphs[{}], ");
		all_glyphs_file << formatter.format<4>(line, "&AllGlyphs[{}], ");
		all_glyphs_file << formatter.format<5>(line, "&AllGlyphs[{}], ");
		all_glyphs_file << formatter.format<6>(line, "&AllGlyphs[{}]");
		all_glyphs_file << " }, ";

		all_glyphs_file << "{ ";
		all_glyphs_file << formatter.format<7>(line, "Connection::{}, ");
		all_glyphs_file << formatter.format<8>(line, "Connection::{}, ");
		all_glyphs_file << formatter.format<9>(line, "Connection::{}, ");
		all_glyphs_file << formatter.format<10>(line, "Connection::{}");
		all_glyphs_file << " }";

		all_glyphs_file << " ),\n";
	}

	all_glyphs_file << "};\n";
}

void output_unichar_to_glyphs(std::ofstream& unichar_to_glyphs_file, const CodepointToIndex& codepoint_to_index)
{
	const OutputFormatter formatter(codepoint_to_index);

	unichar_to_glyphs_file << "inline const std::map<int32_t, const Glyph*> UnicharToGlyph =\n";
	unichar_to_glyphs_file << "{\n";

	for (auto& line : codepoint_to_index)
	{
		unichar_to_glyphs_file << "\t{ ";
		unichar_to_glyphs_file << formatter.format<0>(line, "{}, ");
		unichar_to_glyphs_file << formatter.format<1>(line, "&AllGlyphs[{}]");
		unichar_to_glyphs_file << " },\n";
	}

	unichar_to_glyphs_file << "};\n";
}

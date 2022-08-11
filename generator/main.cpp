#include <array>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <variant>

template <class T>
using ErrorVariant = std::variant<T, std::string>;

constexpr bool is_uint(std::string_view str)
{
	return std::ranges::all_of(str, &isdigit);
}
constexpr std::string_view is_uint_name = "a positive integer";

constexpr bool is_connection(std::string_view str)
{
	return str == "EMPTY"
		|| str == "DIAG_BOTH"
		|| str == "ORTHO_BOTH"
		|| str == "DIAG_FRONT"
		|| str == "DIAG_BACK"
		|| str == "ORTHO_UP"
		|| str == "ORTHO_DOWN"
		|| str == "ORTHO_LEFT"
		|| str == "ORTHO_RIGHT"
	;
}
constexpr std::string_view is_connection_name = "a connection type";

constexpr std::size_t entries_per_line = 9;

constexpr std::array validator_functions = { is_uint, is_uint, is_uint, is_uint, is_uint, is_connection, is_connection, is_connection, is_connection };
constexpr std::array validator_functions_names = { is_uint_name, is_uint_name, is_uint_name, is_uint_name, is_uint_name, is_connection_name, is_connection_name, is_connection_name, is_connection_name };

static_assert(validator_functions.size() == entries_per_line);
static_assert(validator_functions_names.size() == entries_per_line);



using InputPathsVariant = ErrorVariant<std::pair<std::filesystem::path, std::filesystem::path>>;

InputPathsVariant get_input_paths(int argc, const char** argv)
{
	if (argc != 3)
		return "Bask3twork generator must be given exactly 2 arguments:\n\t(1) The path of the CSV file; and\n\t(2) The path of the output directory";

	std::filesystem::path csv_path = argv[1];
	std::filesystem::path output_path = argv[2];

	if (not std::filesystem::exists(csv_path))
		return std::format(R"(The CSV file path does not exist: "{}")", csv_path.string());
	if (not std::filesystem::is_regular_file(csv_path) || csv_path.extension() != ".csv")
		return R"(The CSV file path must have ".csv" extension)";

	if (not std::filesystem::exists(output_path))
		return std::format(R"(The output path does not exist: "{}")", output_path.string());
	if (not std::filesystem::is_directory(output_path))
		return "The output path must be a directory";

	return std::pair(std::move(csv_path), std::move(output_path));
}



using FStreamsVariant = ErrorVariant<std::tuple<std::ifstream, std::ofstream, std::ofstream>>;

FStreamsVariant get_fstreams(const std::pair<std::filesystem::path, std::filesystem::path>& paths)
{
	const auto& [csv_path, output_path] = paths;

	std::ifstream csv = std::ifstream(csv_path);
	if (not csv.is_open())
		return std::format(R"(Could not open the CSV file: "{}")", csv_path.string());

	std::ofstream all_glyphs = std::ofstream(output_path / "AllGlyphs.ipp");
	if (not all_glyphs.is_open())
		return std::format(R"(Could not open or create the output file: "{}/AllGlyphs.ipp")", csv_path.string());

	std::ofstream unichar_to_glyph = std::ofstream(output_path / "UnicharToGlyph.ipp");
	if (not unichar_to_glyph.is_open())
		return std::format(R"(Could not open or create the output file: "{}/UnicharToGlyph.ipp")", csv_path.string());

	return std::tuple(std::move(csv), std::move(all_glyphs), std::move(unichar_to_glyph));
}



using RawInputVariant = ErrorVariant<std::vector<std::vector<std::string>>>;

RawInputVariant get_raw_input(std::ifstream& csv)
{
	std::vector<std::vector<std::string>> raw_input;

	for (std::string line; std::getline(csv, line); )
	{
		std::vector<std::string> vec;

		auto& back = raw_input.emplace_back();
		for (auto value : line | std::views::split(','))
		{
			back.emplace_back(value.begin(), value.end());
		}
	}

	return RawInputVariant(std::move(raw_input));
}



struct ProcessedInputLine
{
	std::string codepoint;
	std::string rotate_90_index;
	std::string rotate_180_index;
	std::string mirror_x_index;
	std::string mirror_y_index;
	std::string up_connection;
	std::string down_connection;
	std::string left_connection;
	std::string right_connection;
};

using CodepointToIndexMap = std::map<std::string, std::string>;
using ProcessedInputVector = std::vector<ProcessedInputLine>;

using ProcessedInputVariant = ErrorVariant<std::pair<CodepointToIndexMap, ProcessedInputVector>>;

ProcessedInputVariant get_processed_input(const std::vector<std::vector<std::string>>& raw_input)
{
	for (int line_number = 0; auto& line : raw_input)
	{
		line_number++;

		if (line.size() != entries_per_line)
			return std::format("Line {} of the CSV file has the wrong number of entries. Expected = {}, actual = {}.", line_number, entries_per_line, line.size());

		if (std::ranges::all_of(line, &std::string::empty))
			continue; // Entirely empty lines are fine, they will just be skipped later

		for (int entry_number = 0; const std::string& entry : line)
		{
			entry_number++;
			if (not std::invoke(validator_functions[entry_number - 1], entry))
				return std::format(R"(Entry {} in line {} of the CSV file does not match the required form. Expected = *{}*, actual = "{}")", entry_number, line_number, validator_functions_names[entry_number - 1], entry);
		}
	}

	CodepointToIndexMap codepoint_to_index;

	for (int index = 0; auto& line : raw_input)
	{
		if (std::ranges::all_of(line, &std::string::empty))
			continue; // Entirely empty lines are skipped

		codepoint_to_index.emplace(line[0], std::to_string(index++));
	}

	ProcessedInputVector processed_input_lines;

	for (int line_number = 0; auto& line : raw_input)
	{
		line_number++;

		if (std::ranges::all_of(line, &std::string::empty))
			continue; // Entirely empty lines are skipped

		auto& processed = processed_input_lines.emplace_back();
		processed.codepoint = line[0];

		for (int i = 1; i <= 4; i++)
		{
			if (not codepoint_to_index.contains(line[i]))
				return std::format(R"(Entry {} in line {} of the CSV file is not present as the 1st entry of any other line. Actual = {})", i, line_number, line[i]);
		}

		processed.rotate_90_index  = codepoint_to_index.at(line[1]);
		processed.rotate_180_index = codepoint_to_index.at(line[2]);
		processed.mirror_x_index   = codepoint_to_index.at(line[3]);
		processed.mirror_y_index   = codepoint_to_index.at(line[4]);

		processed.up_connection    = line[5];
		processed.down_connection  = line[6];
		processed.left_connection  = line[7];
		processed.right_connection = line[8];
	}

	if (codepoint_to_index.size() != processed_input_lines.size())
		return "Bask3twork generator internal error.";

	return std::pair(std::move(codepoint_to_index), std::move(processed_input_lines));
}



void output_all_glyphs(std::ofstream& all_glyphs_file, const ProcessedInputVector& processed_input_lines)
{
	std::array<std::size_t, entries_per_line> max_sizes = {};

	for (auto& line : processed_input_lines)
	{
		max_sizes[0] = std::max(max_sizes[0], line.codepoint.size());
		max_sizes[1] = std::max(max_sizes[1], line.rotate_90_index.size());
		max_sizes[2] = std::max(max_sizes[2], line.rotate_180_index.size());
		max_sizes[3] = std::max(max_sizes[3], line.mirror_x_index.size());
		max_sizes[4] = std::max(max_sizes[4], line.mirror_y_index.size());
		max_sizes[5] = std::max(max_sizes[5], line.up_connection.size());
		max_sizes[6] = std::max(max_sizes[6], line.down_connection.size());
		max_sizes[7] = std::max(max_sizes[7], line.left_connection.size());
		max_sizes[8] = std::max(max_sizes[8], line.right_connection.size());
	}

	all_glyphs_file << std::format("constexpr std::array<Glyph, {}> AllGlyphs =\n", processed_input_lines.size());
	all_glyphs_file << "{\n";

	for (auto& line : processed_input_lines)
	{
		all_glyphs_file << "\tGlyph( ";

		all_glyphs_file << std::format("{:{}}", line.codepoint + ", ", max_sizes[0] + 2);

		auto rotate_90_index = std::format("&AllGlyphs[{}], ", line.rotate_90_index);
		auto rotate_180_index = std::format("&AllGlyphs[{}], ", line.rotate_180_index);
		auto mirror_x_index = std::format("&AllGlyphs[{}], ", line.mirror_x_index);
		auto mirror_y_index = std::format("&AllGlyphs[{}], ", line.mirror_y_index);

		all_glyphs_file << std::format("{:{}}", rotate_90_index, max_sizes[1] + 14);
		all_glyphs_file << std::format("{:{}}", rotate_180_index, max_sizes[2] + 14);
		all_glyphs_file << std::format("{:{}}", mirror_x_index, max_sizes[3] + 14);
		all_glyphs_file << std::format("{:{}}", mirror_y_index, max_sizes[4] + 14);

		all_glyphs_file << std::format("{:{}}", line.up_connection + ", ", max_sizes[5] + 2);
		all_glyphs_file << std::format("{:{}}", line.down_connection + ", ", max_sizes[6] + 2);
		all_glyphs_file << std::format("{:{}}", line.left_connection + ", ", max_sizes[7] + 2);
		all_glyphs_file << std::format("{:{}}", line.right_connection, max_sizes[8]);

		all_glyphs_file << " ),\n";
	}

	all_glyphs_file << "};\n";
}

void output_unichar_to_glyphs(std::ofstream& unichar_to_glyphs_file, const CodepointToIndexMap& codepoint_to_index)
{
	std::array<std::size_t, 2> max_sizes = {};

	for (auto& line : codepoint_to_index)
	{
		max_sizes[0] = std::max(max_sizes[0], line.first.size());
		max_sizes[1] = std::max(max_sizes[1], line.second.size());
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



int main(int argc, const char** argv)
{
	auto initial_paths = get_input_paths(argc, argv);
	if (initial_paths.index() == 1)
	{
		std::cout << std::get<1>(initial_paths) << "\n";
		return EXIT_FAILURE;
	}
	
	auto fstreams = get_fstreams(std::get<0>(initial_paths));
	if (fstreams.index() == 1)
	{
		std::cout << std::get<1>(fstreams) << "\n";
		return EXIT_FAILURE;
	}

	auto& [csv, all_glyphs_file, unichar_to_glyphs_file] = std::get<0>(fstreams);

	auto raw_input = get_raw_input(csv);
	if (raw_input.index() == 1)
	{
		std::cout << std::get<1>(raw_input) << "\n";
		return EXIT_FAILURE;
	}

	auto processed_input = get_processed_input(std::get<0>(raw_input));
	if (processed_input.index() == 1)
	{
		std::cout << std::get<1>(processed_input) << "\n";
		return EXIT_FAILURE;
	}

	const auto& [codepoint_to_index, processed_input_lines] = std::get<0>(processed_input);

	output_all_glyphs(all_glyphs_file, processed_input_lines);
	output_unichar_to_glyphs(unichar_to_glyphs_file, codepoint_to_index);

	std::cout << R"(Bask3twork successfully generated "AllGlyphs.ipp" and "UnicharToGlyph.ipp")" << "\n";
	return EXIT_SUCCESS;
}

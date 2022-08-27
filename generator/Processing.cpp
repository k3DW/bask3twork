#include "pch.h"
#include "Processing.h"

using InputLines = std::vector<std::vector<std::string>>;

static InputLines get_raw_input(std::ifstream& csv_file)
{
	InputLines raw_input;

	for (std::string line; std::getline(csv_file, line); )
	{
		auto& back = raw_input.emplace_back();
		for (auto value : line | std::views::split(','))
		{
			back.emplace_back(value.begin(), value.end());
		}

		if (std::ranges::all_of(back, &std::string::empty))
			raw_input.pop_back(); // Entirely empty lines are fine, but they are skipped
	}

	return raw_input;
}

namespace // Validation internals
{

	constexpr std::size_t entries_per_line_raw = 9;

	struct ValidatorFunction
	{
		bool(*callable)(std::string_view);
		std::string_view description;

		constexpr bool operator()(std::string_view sv) const { return callable(sv); }
	};

	constexpr ValidatorFunction is_uint{ .callable = [](std::string_view str) -> bool { return std::ranges::all_of(str, &isdigit); }, .description = "a natural number" };

	constexpr std::array connections = { "EMPTY", "DIAG_BOTH", "ORTHO_BOTH", "DIAG_FRONT", "DIAG_BACK", "ORTHO_UP", "ORTHO_DOWN", "ORTHO_LEFT", "ORTHO_RIGHT", };
	constexpr ValidatorFunction is_connection{ .callable = [](std::string_view str) -> bool { return std::ranges::find(connections, str) != std::ranges::end(connections); }, .description = "a connection type" };

	constexpr std::array<ValidatorFunction, entries_per_line_raw> validator_functions = { is_uint, is_uint, is_uint, is_uint, is_uint, is_connection, is_connection, is_connection, is_connection };

}

static Expected<InputLines> get_validated_input(InputLines& raw_input)
{
	// This loop would look nicer as an enumerate
	for (int line_number = 0; const auto& line : raw_input)
	{
		line_number++;

		if (line.size() != entries_per_line_raw)
			return std::unexpected(std::format("Line {} of the CSV file has the wrong number of entries. Expected = {}, actual = {}.", line_number, entries_per_line_raw, line.size()));

		// This loop would look nicer as a zip or enumerate
		for (int entry_number = 0; const std::string& entry : line)
		{
			entry_number++;

			const auto validator = validator_functions[entry_number - 1];
			if (not std::invoke(validator, entry))
				return std::unexpected(std::format(R"(Entry {} in line {} of the CSV file does not match the required form. Expected = *{}*, actual = "{}")", entry_number, line_number, validator.description, entry));
		}
	}

	return Expected<InputLines>(std::move(raw_input));
}



static CodepointToIndex get_codepoint_to_index_map(const InputLines& validated_input)
{
	CodepointToIndex codepoint_to_index;

	// This loop would look nicer as an enumerate
	for (int index = 0; auto& line : validated_input)
		codepoint_to_index.emplace(std::stoi(line[0]), index++);

	return codepoint_to_index;
}

static Expected<ProcessedLines> get_processed_lines(const InputLines& validated_input, const CodepointToIndex& codepoint_to_index)
{
	ProcessedLines processed_lines;

	// This loop would look nicer as an enumerate
	for (int line_number = 0; auto& raw_line : validated_input)
	{
		line_number++;

		auto ints = raw_line | std::views::take(5) | std::views::transform([](const std::string& str) -> int { return std::stoi(str); });
		
		for (int i = 1; i <= 4; i++)
		{
			if (not codepoint_to_index.contains(ints[i]))
			{
				return std::unexpected(std::format(R"(Entry {} in line {} of the CSV file is not present as the 1st entry of any other line. Actual = {})", i, line_number, ints[i]));
			}
		}

		auto& processed_line = processed_lines.emplace_back();
		processed_line.codepoint = ints[0];

		processed_line.rotate_90_index  = codepoint_to_index.at(ints[1]);
		processed_line.rotate_180_index = codepoint_to_index.at(ints[2]);
		processed_line.mirror_x_index   = codepoint_to_index.at(ints[3]);
		processed_line.mirror_y_index   = codepoint_to_index.at(ints[4]);

		processed_line.up_connection    = raw_line[5];
		processed_line.down_connection  = raw_line[6];
		processed_line.left_connection  = raw_line[7];
		processed_line.right_connection = raw_line[8];
	}

	for (auto& line : processed_lines)
	{
		line.mirror_forward_diag_index  = processed_lines[line.rotate_90_index].mirror_x_index;
		line.mirror_backward_diag_index = processed_lines[line.mirror_x_index].rotate_90_index;
	}

	return Expected<ProcessedLines>(std::move(processed_lines));
}



Expected<ProcessedData> get_processed_data(std::ifstream& csv_file)
{
	auto raw_input = get_raw_input(csv_file);
	auto validated_input = get_validated_input(raw_input);
	if (not validated_input.has_value())
		return std::unexpected(std::move(validated_input.error()));

	auto codepoint_to_index = get_codepoint_to_index_map(validated_input.value());
	auto processed_lines = get_processed_lines(validated_input.value(), codepoint_to_index);
	if (not processed_lines.has_value())
		return std::unexpected(std::move(processed_lines.error()));

	if (codepoint_to_index.size() != processed_lines.value().size())
		return std::unexpected("Bask3twork generator internal error.");

	return ProcessedData(std::move(codepoint_to_index), std::move(processed_lines.value()));
}

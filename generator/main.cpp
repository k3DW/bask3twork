#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <variant>
#include <format>

template <class T>
using ErrorVariant = std::variant<T, std::string>;



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



int main(int argc, const char** argv)
{
	InputPathsVariant initial_paths = get_input_paths(argc, argv);
	if (initial_paths.index() == 1)
	{
		std::cout << std::get<1>(initial_paths) << "\n";
		return EXIT_FAILURE;
	}
	const auto& [csv_path, output_path] = std::get<0>(initial_paths);

	std::ifstream csv = std::ifstream(csv_path);
	if (not csv.is_open())
	{
		std::cout << std::format(R"(Could not open the CSV file: "{}")", csv_path.string());
		return EXIT_FAILURE;
	}
}

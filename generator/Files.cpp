#include "Files.h"

static InputPaths get_input_paths(int argc, const char** argv)
{
	if (argc != 3)
		return std::unexpected("Bask3twork generator must be given exactly 2 arguments:\n\t(1) The path of the CSV file; and\n\t(2) The path of the output directory");

	std::filesystem::path csv_path = argv[1];
	std::filesystem::path output_path = argv[2];
	
	if (not std::filesystem::exists(csv_path))
		return std::unexpected(std::format(R"(The CSV file path does not exist: "{}")", csv_path.string()));
	if (not std::filesystem::is_regular_file(csv_path) || csv_path.extension() != ".csv")
		return std::unexpected(R"(The CSV file path must have ".csv" extension)");
	
	if (not std::filesystem::exists(output_path))
	{
		std::filesystem::create_directory(output_path);
	}
	if (not std::filesystem::is_directory(output_path))
		return std::unexpected("The output path must be a directory");
	
	return std::pair(std::move(csv_path), std::move(output_path));
}

static FStreams get_fstreams(const std::pair<std::filesystem::path, std::filesystem::path>& paths)
{
	const auto& [csv_path, output_path] = paths;

	std::ifstream csv_file = std::ifstream(csv_path);
	if (not csv_file.is_open())
		return std::unexpected(std::format(R"(Could not open the CSV file: "{}")", csv_path.string()));

	std::ofstream all_glyphs = std::ofstream(output_path / "AllGlyphs.impl");
	if (not all_glyphs.is_open())
		return std::unexpected(std::format(R"(Could not open or create the output file: "{}/AllGlyphs.impl")", csv_path.string()));

	std::ofstream unichar_to_glyph = std::ofstream(output_path / "UnicharToGlyph.impl");
	if (not unichar_to_glyph.is_open())
		return std::unexpected(std::format(R"(Could not open or create the output file: "{}/UnicharToGlyph.impl")", csv_path.string()));

	return std::tuple(std::move(csv_file), std::move(all_glyphs), std::move(unichar_to_glyph));
}



FStreams get_files(int argc, const char** argv)
{
	auto paths = get_input_paths(argc, argv);
	if (not paths.has_value())
		return std::unexpected(std::move(paths.error()));

	return get_fstreams(paths.value());
}

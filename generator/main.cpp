#include "pch.h"
#include "Files.h"
#include "Processing.h"
#include "Output.h"



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

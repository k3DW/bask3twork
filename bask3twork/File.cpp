#include "pch.h"
#include "File.h"
#include "pure/GridSize.h"
#include "grid/Display.h"
#include "grid/Knot.h"
#include "grid/Tile.h"



void File::write(const wxString& file_name, const Knot* knot, const DisplayGrid* disp)
{
	wxFile file;
	if (!file.Create(file_name, true)) // Overwrite if it already exists
	{
		wxMessageBox("Failed to create file.", "Error");
		return;
	}

	const std::vector<std::byte> buffer = make_buffer(knot, disp);

	file.Write(buffer.data(), buffer.size());
}



std::size_t File::file_size(GridSize size)
{
	const int area = size.area();

	return 0
		+ sizeof(GridSize)           // The file "header" as the grid size
		+ (area * sizeof(CodePoint)) // Each code point in the knot
		+ (area * sizeof(bool))      // The locked state of the tiles
		;
}

std::vector<std::byte> File::make_buffer(const Knot* knot, const DisplayGrid* disp)
{
	const GridSize size = knot->size;
	const std::size_t length = file_size(size);

	std::vector<std::byte> buffer;
	buffer.reserve(length);

	auto add_to_buffer = [&buffer](const auto& arg) -> void
		{
			if (sizeof(arg) > buffer.capacity() - buffer.size())
				throw;
			std::array<std::byte, sizeof(arg)> array;
			std::memcpy(array.data(), &arg, sizeof(arg));
			buffer.insert(buffer.end(), array.begin(), array.end());
		};

	add_to_buffer(size);

	// Write the knot to the buffer
	for (int i = 0; i < size.rows; ++i)
	for (int j = 0; j < size.columns; ++j)
	{
		add_to_buffer(CodePoint{ knot->code_point(i, j) });
	}
	
	// Write the locking state to the buffer
	for (int i = 0; i < size.rows; ++i)
	for (int j = 0; j < size.columns; ++j)
	{
		add_to_buffer(Bool{ disp->get_tiles()[i][j]->locked() });
	}

	return buffer;
}

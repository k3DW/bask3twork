#include "pch.h"
#include "File.h"
#include "pure/Glyph.h"
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

auto File::read(const wxString& file_name)
	-> std::optional<std::tuple<GridSize, Glyphs, std::vector<Bool>>>
{
	wxFile file;
	if (!file.Open(file_name))
	{
		wxMessageBox("Failed to open file.", "Error");
		return std::nullopt;
	}

	const std::size_t length = file.Length();

	if (length < sizeof(GridSize))
	{
		wxMessageBox("File has been corrupted, reason 1.", "Error");
		return std::nullopt;
	}

	const auto size_opt = read_single<GridSize>(file);
	if (!size_opt)
	{
		wxMessageBox("File has been corrupted, reason 2.", "Error");
		return std::nullopt;
	}
	const GridSize size = *size_opt;

	const std::size_t expected_length = file_size(size);
	if (length != expected_length)
	{
		wxMessageBox("File has been corrupted, reason 3.", "Error");
		return std::nullopt;
	}

	const int area = size.area();

	const std::vector<CodePoint> code_points = read_range<CodePoint>(file, area);
	if (code_points.empty())
	{
		wxMessageBox("File has been corrupted, reason 4.", "Error");
		return std::nullopt;
	}

	std::vector<Bool> locking = read_range<Bool>(file, area);
	if (locking.empty())
	{
		wxMessageBox("File has been corrupted, reason 5.", "Error");
		return std::nullopt;
	}

	if (!file.Eof())
	{
		wxMessageBox("File has been corrupted, reason 6.", "Error");
		return std::nullopt;
	}

	for (CodePoint code_point : code_points)
	{
		if (UnicharToGlyph.find(code_point) == UnicharToGlyph.end())
		{
			wxMessageBox(wxString::Format("File contains unsupported code point %i, may have been corrupted.", code_point), "Error");
			return std::nullopt;
		}
	}



	Glyphs glyphs;
	glyphs.reserve(size.rows);

	std::size_t running_index = 0;
	for (int i = 0; i < size.rows; ++i)
	{
		auto& row = glyphs.emplace_back();
		row.reserve(size.columns);
		for (int j = 0; j < size.columns; ++j)
		{
			row.push_back(UnicharToGlyph.at(code_points[running_index++]));
		}
	}



	return std::tuple{ size, std::move(glyphs), std::move(locking) };
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

template <class T>
std::vector<T> File::read_range(wxFile& file, std::size_t count)
{
	const std::size_t num_bytes = sizeof(T) * count;
	auto buffer = std::make_unique<std::byte[]>(num_bytes);
	if (num_bytes != file.Read(buffer.get(), num_bytes))
		return {};

	auto ptr = reinterpret_cast<T*>(buffer.get());
	return std::vector<T>(ptr, ptr + count);
}

template <class T>
std::optional<T> File::read_single(wxFile& file)
{
	const std::size_t num_bytes = sizeof(T);
	auto buffer = std::make_unique<std::byte[]>(num_bytes);
	if (num_bytes != file.Read(buffer.get(), num_bytes))
		return std::nullopt;

	return { std::move(*reinterpret_cast<T*>(buffer.get())) };
}

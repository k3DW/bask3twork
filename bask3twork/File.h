#pragma once
#include "Forward.h"

struct File
{
public:
	static void write(const wxString& file_name, const Knot* knot, const DisplayGrid* disp);

	struct Bool
	{
		bool value;
		operator bool() const { return value; }
	};

	static auto read(const wxString& file_name)
		-> std::optional<std::tuple<GridSize, Glyphs, std::vector<Bool>>>;

	static constexpr const char* ext = "k3DW Knot Files (*.k3knot)|*.k3knot|Text files (*.txt)|*.txt";

private:
	static std::size_t file_size(GridSize size);
	static std::vector<std::byte> make_buffer(const Knot* knot, const DisplayGrid* disp);

	template <class T>
	static std::vector<T> read_range(wxFile& file, std::size_t count);
	template <class T>
	static std::optional<T> read_single(wxFile& file);
};

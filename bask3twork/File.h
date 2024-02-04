#pragma once
#include "Forward.h"

struct File
{
public:
	static void write(const wxString& file_name, const Knot* knot, const DisplayGrid* disp);

	static constexpr const char* ext = "k3DW Knot Files (*.k3knot)|*.k3knot|Text files (*.txt)|*.txt";

private:
	static std::size_t file_size(GridSize size);
	static std::vector<std::byte> make_buffer(const Knot* knot, const DisplayGrid* disp);
};

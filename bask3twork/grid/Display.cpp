#include "pch.h"
#include "grid/Display.h"
#include "grid/Knot.h"
#include "grid/Tile.h"
#include "pure/Glyph.h"
#include "pure/GridSize.h"
#include "pure/Selection.h"
#include "Constants.h"
#include "MainWindow.h"

DisplayGrid::DisplayGrid(MainWindow* parent, GridSize size)
	: wxWindow(parent, wxID_ANY)
	, parent(parent)
	, glyph_font(Fonts::glyph)
	, axis_font(Fonts::axis)
{
	Hide();
	resize(size);

	Bind(wxEVT_PAINT, &DisplayGrid::on_paint, this);
	Bind(wxEVT_LEFT_DOWN, &DisplayGrid::on_lclick, this);
	Bind(wxEVT_RIGHT_DOWN, &DisplayGrid::on_rclick, this);

	SetDoubleBuffered(true);
	Show();
}

DisplayGrid::~DisplayGrid()
{
	Hide();
}



void DisplayGrid::resize(GridSize size)
{
	grid_size = size;
	make_tiles();
	update_sizes_and_offsets();
	knot = nullptr;
}

void DisplayGrid::set_glyph_font_size(int i)
{
	wxSize size{ i, i };
	glyph_font.SetPixelSize(size);

	const int axis_point_size = std::max(std::min(Sizes::font_point, size.x / 3), 1);
	axis_font.SetPointSize(axis_point_size);

	update_sizes_and_offsets();
}

void DisplayGrid::update_sizes_and_offsets()
{
	glyph_font_size = glyph_font.GetPixelSize();

	wxSize axis_font_size = axis_font.GetPixelSize();
	tiles_offset = { axis_font_size.y / 2 + 1, axis_font_size.y + 1 }; // For some reason, the x value reads 0
	if (grid_size.rows >= 10)
		tiles_offset.x *= 2;

	tiles_size = { glyph_font_size.x * grid_size.columns, glyph_font_size.y * grid_size.rows };
	window_size = { tiles_offset.x + tiles_size.x, tiles_offset.y + tiles_size.y };

	update_tile_offsets();
	SetMinSize(wxDefaultSize);
	SetMinSize(window_size);
}



void DisplayGrid::on_lclick(wxMouseEvent& evt)
{
	const Point tile_pos = tile_position(evt.GetPosition());
	if (tile_pos == Point{ -1, -1 })
		return evt.Skip();

	if (wxGetKeyState(WXK_CONTROL))
	{
		tiles[tile_pos.i][tile_pos.j].lock();
	}
	else
	{
		parent->set_min(tile_pos);
	}

	parent->hide_selection();
	unhighlight();
	evt.Skip();
}

void DisplayGrid::on_rclick(wxMouseEvent& evt)
{
	const Point tile_pos = tile_position(evt.GetPosition());
	if (tile_pos == Point{ -1, -1 })
		return evt.Skip();

	if (wxGetKeyState(WXK_CONTROL))
	{
		tiles[tile_pos.i][tile_pos.j].unlock();
	}
	else
	{
		parent->set_max(tile_pos);
	}

	parent->hide_selection();
	unhighlight();
	evt.Skip();
}

void DisplayGrid::on_paint(wxPaintEvent&)
{
	wxPaintDC dc(this);
	render(dc);
}



void DisplayGrid::render()
{
	wxClientDC dc(this);
	render(dc);
}

void DisplayGrid::render(wxDC& dc)
{
	dc.SetPen(*wxTRANSPARENT_PEN);
	render_axis_labels(dc);
	render_tiles(dc);
	render_knot(dc);
}

void DisplayGrid::render_axis_labels(wxDC& dc)
{
	static const wxBrush background = Colours::background;
	dc.SetBrush(background);
	dc.DrawRectangle({ 0, 0 }, { tiles_offset.x, window_size.y });
	dc.DrawRectangle({ 0, 0 }, { window_size.x, tiles_offset.y });

	dc.SetFont(axis_font);

	for (int i = 0; i < grid_size.rows; i++)
		dc.DrawText(wxString::Format("%i", i + 1), y_label_offset(i));

	for (int j = 0; j < grid_size.columns; j++)
		dc.DrawText(wxString::Format("%i", j + 1), x_label_offset(j));
}

void DisplayGrid::render_tiles(wxDC& dc)
{
	for (const Tile& tile : tiles)
		tile.render(dc, glyph_font_size);
}

void DisplayGrid::render_knot(wxDC& dc)
{
	if (nullptr == knot)
		return;

	dc.SetFont(glyph_font);
	for (int i = 0; i < grid_size.rows; i++)
	{
		wxString str;
		for (int j = 0; j < grid_size.columns; j++)
		{
			str << knot->get(i, j);
		}
		dc.DrawText(str, tile_offset(i, 0));
	}
}



void DisplayGrid::highlight(Selection selection)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			tiles[i][j].highlight();
	render();
}

void DisplayGrid::unhighlight()
{
	for (int i = 0; i < grid_size.rows; i++)
		for (int j = 0; j < grid_size.columns; j++)
			tiles[i][j].unhighlight();
	render();
}

void DisplayGrid::lock(Point point)
{
	tiles[point.i][point.j].lock();
	render();
}

void DisplayGrid::lock_no_render(Point point)
{
	tiles[point.i][point.j].lock();
}

void DisplayGrid::unlock(Point point)
{
	tiles[point.i][point.j].unlock();
	render();
}

void DisplayGrid::lock(Selection selection)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			tiles[i][j].lock();
	render();
}

void DisplayGrid::unlock(Selection selection)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			tiles[i][j].unlock();
	render();
}

void DisplayGrid::invert_locking(Selection selection)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
		{
			Tile& tile = tiles[i][j];
			tile.locked()
				? tile.unlock()
				: tile.lock();
		}
	render();
}

void DisplayGrid::reset_tiles()
{
	for (int i = 0; i < grid_size.rows; i++)
		for (int j = 0; j < grid_size.columns; j++)
			tiles[i][j].reset_state();
	render();
}



void DisplayGrid::make_tiles()
{
	tiles = Tiles(grid_size);
	const auto [rows, columns] = grid_size;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			tiles.emplace_back(TileBrushes::all[i % 2][j % 2], tile_offset(i, j));
		}
	}
}

void DisplayGrid::update_tile_offsets()
{
	if (tiles.empty())
		return;

	const auto [rows, columns] = grid_size;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			tiles[i][j].set_offset(tile_offset(i, j));
}

wxPoint DisplayGrid::x_label_offset(int pos) const
{
	int x = tiles_offset.x + (pos * glyph_font_size.x) + (glyph_font_size.x / 2) - (tiles_offset.x / 2);
	int y = 0;
	return { x, y };
}

wxPoint DisplayGrid::y_label_offset(int pos) const
{
	int x = (grid_size.rows < 10 || pos >= 9) ? 0 : tiles_offset.x / 4;
	int y = tiles_offset.y + (pos * glyph_font_size.y) + (glyph_font_size.y / 2) - (tiles_offset.y / 2);
	return { x, y };
}

wxPoint DisplayGrid::tile_offset(int i, int j) const
{
	int x = tiles_offset.x + (j * glyph_font_size.x);
	int y = tiles_offset.y + (i * glyph_font_size.y);
	return { x, y };
}

Point DisplayGrid::tile_position(wxPoint offset) const
{
	offset -= tiles_offset;
	if (offset.x < 0 || offset.y < 0)
		return { -1, -1 };
	return { offset.y / glyph_font_size.y, offset.x / glyph_font_size.x };
}

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
	: wxPanel(parent)
	, sizer(new wxGridBagSizer(-1, 0))
	, tiles(make_tiles(parent))
	, highlighted(false)
	, glyph_font(Fonts::glyph)
	, axis_font(Fonts::axis)
{
	Hide();
	SetSizer(sizer);
	add_axis_labels(size);
	Show();
}

DisplayGrid::~DisplayGrid()
{
	Hide();
}

void DisplayGrid::highlight(Selection selection)
{
	unhighlight(false);
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			tiles[i][j]->highlight();
	highlighted = true;
	Refresh();
}

void DisplayGrid::unhighlight(bool refresh) {// This function clears all tiles from being highlighted.
	if (highlighted)
	{
		for (auto& tile_row : tiles)
			for (Tile* tile : tile_row)
				tile->unhighlight();
		if (refresh)
			Refresh();
		highlighted = false;
	}
}

void DisplayGrid::lock(Selection selection)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			tiles[i][j]->lock();
	Refresh();
}

void DisplayGrid::lock(Point point)
{
	tiles[point.i][point.j]->lock();
	tiles[point.i][point.j]->Refresh();
}

void DisplayGrid::unlock(Selection selection)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			tiles[i][j]->unlock();
	Refresh();
}

void DisplayGrid::unlock(Point point)
{
	tiles[point.i][point.j]->unlock();
	tiles[point.i][point.j]->Refresh();
}

void DisplayGrid::invert_locking(Selection selection)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
		{
			Tile* tile = tiles[i][j];
			tile->locked()
				? tile->unlock()
				: tile->lock();
		}
	Refresh();
}

void DisplayGrid::draw(const Knot* knot)
{
	for (int i = 0; i < knot->size.rows; i++)
		for (int j = 0; j < knot->size.columns; j++)
			tiles[i][j]->SetLabelText(knot->get(i, j));
}

void DisplayGrid::reduce_glyph_font_size_by(int i)
{
	wxSize old_size = glyph_font.GetPixelSize();
	wxSize size{ old_size.x - i, old_size.y - i };

	glyph_font.SetPixelSize(size);

	int axis_point_size = std::max(std::min(12, size.x / 3), 1);
	axis_font.SetPointSize(axis_point_size);

	for (const auto& row : tiles)
		for (Tile* tile : row)
		{
			tile->SetFont(glyph_font);
			tile->SetMinSize(size);
		}

	for (AxisLabel* label : x_axis)
	{
		label->SetFont(axis_font);
		label->SetMinSize(wxDefaultSize);
	}

	for (AxisLabel* label : y_axis)
	{
		label->SetFont(axis_font);
		label->SetMinSize(wxDefaultSize);
	}
}

void DisplayGrid::add_axis_labels(GridSize size)
{
	for (int i = 1; i <= size.columns; i++)
	{
		AxisLabel* label = new AxisLabel(this, i);
		x_axis.push_back(label);
		sizer->Add(label, wxGBPosition(0, i), wxDefaultSpan, wxALIGN_CENTER);
	}

	for (int i = 1; i <= size.rows; i++)
	{
		AxisLabel* label = new AxisLabel(this, i);
		y_axis.push_back(label);
		sizer->Add(label, wxGBPosition(i, 0), wxDefaultSpan, wxALIGN_CENTER);
	}
}

Tiles DisplayGrid::make_tiles(MainWindow* parent)
{
	const auto [rows, columns] = parent->size;
	auto tiles = Tiles(rows, std::vector<Tile*>(columns, nullptr));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			wxWindowID id = (i * columns) + j;
			const TileColours& colours = TileColours::all[i % 2][j % 2];
			tiles[i][j] = new Tile(this, id, wxUniChar(DefaultGlyph->code_point), colours);
			tiles[i][j]->Bind(wxEVT_LEFT_DOWN, &MainWindow::left_click_tile, parent);
			tiles[i][j]->Bind(wxEVT_RIGHT_DOWN, &MainWindow::right_click_tile, parent);
			sizer->Add(tiles[i][j], wxGBPosition(i + 1, j + 1)/*, wxDefaultSpan, wxALIGN_CENTER*/);
		}
	}
	return tiles;
}

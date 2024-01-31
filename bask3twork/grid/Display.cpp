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

void DisplayGrid::unlock(Selection selection)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			tiles[i][j]->unlock();
	Refresh();
}

void DisplayGrid::draw(const Knot* knot)
{
	for (int i = 0; i < knot->size.rows; i++)
		for (int j = 0; j < knot->size.columns; j++)
			tiles[i][j]->SetLabelText(knot->get(i, j));
}

void DisplayGrid::add_axis_labels(GridSize size)
{
	for (int i = 1; i <= size.columns; i++)
		sizer->Add(new AxisLabel(this, i), wxGBPosition(0, i), wxDefaultSpan, wxALIGN_CENTER);
	for (int i = 1; i <= size.rows; i++)
		sizer->Add(new AxisLabel(this, i), wxGBPosition(i, 0), wxDefaultSpan, wxALIGN_CENTER);
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
			const wxColour& colour = Colours::tile[i % 2][j % 2];
			tiles[i][j] = new Tile(this, id, wxUniChar(DefaultGlyph->code_point), colour);
			tiles[i][j]->Bind(wxEVT_LEFT_DOWN, &MainWindow::left_click_tile, parent);
			tiles[i][j]->Bind(wxEVT_RIGHT_DOWN, &MainWindow::right_click_tile, parent);
			sizer->Add(tiles[i][j], wxGBPosition(i + 1, j + 1)/*, wxDefaultSpan, wxALIGN_CENTER*/);
		}
	}
	return tiles;
}

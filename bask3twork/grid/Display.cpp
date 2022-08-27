#include "grid/Display.h"
#include "grid/Knot.h"
#include "grid/Tile.h"

#include "MainWindow.h"

DisplayGrid::DisplayGrid(MainWindow* parent, int h, int w)
	: wxPanel(parent)
	, sizer(new wxGridBagSizer(-1, 0))
	, tiles(make_tiles(parent, this))
	, highlighted(false)
{
	Hide();
	SetSizer(sizer);
	add_axis_labels(this, h, w);
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

void DisplayGrid::draw(const Knot* knot)
{
	for (int i = 0; i < knot->h; i++)
		for (int j = 0; j < knot->w; j++)
			tiles[i][j]->SetLabelText(knot->get(i, j));
}

void DisplayGrid::add_axis_labels(DisplayGrid* self, int h, int w)
{
	for (int i = 1; i <= w; i++)
		self->sizer->Add(new AxisLabel(self, i), wxGBPosition(0, i), wxDefaultSpan, wxALIGN_CENTER);
	for (int i = 1; i <= h; i++)
		self->sizer->Add(new AxisLabel(self, i), wxGBPosition(i, 0), wxDefaultSpan, wxALIGN_CENTER);
}

Tiles DisplayGrid::make_tiles(MainWindow* parent, DisplayGrid* self)
{
	auto tiles = Tiles(parent->h, std::vector<Tile*>(parent->w, nullptr));
	for (int i = 0; i < parent->h; i++)
	{
		for (int j = 0; j < parent->w; j++)
		{
			wxWindowID id = (i * parent->w) + j;
			const wxColour& colour = Colours::tile[i % 2][j % 2];
			tiles[i][j] = new Tile(self, id, wxUniChar(DefaultGlyph->code_point), colour);
			tiles[i][j]->Bind(wxEVT_LEFT_DOWN, &MainWindow::left_click_tile, parent);
			tiles[i][j]->Bind(wxEVT_RIGHT_DOWN, &MainWindow::right_click_tile, parent);
			self->sizer->Add(tiles[i][j], wxGBPosition(i + 1, j + 1)/*, wxDefaultSpan, wxALIGN_CENTER*/);
		}
	}
	return tiles;
}

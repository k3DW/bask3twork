#include "DisplayGrid.h"
#include "MainWindow.h"

DisplayGrid::DisplayGrid(MainWindow* parent, Knot* knot) : wxPanel(parent), knot(knot), h(knot->h), w(knot->w) {
	Hide();

	sizer = new wxGridBagSizer(-1, 0);
	SetSizer(sizer);
	
	initAxisLabels();
	initTiles(parent);

	Show();
}
DisplayGrid::~DisplayGrid() {
	Hide();
}
void DisplayGrid::initAxisLabels() {
	for (int i = 1; i <= w; i++)
		sizer->Add(new AxisLabel(this, i), wxGBPosition(0, i), wxDefaultSpan, wxALIGN_CENTER);
	for (int i = 1; i <= h; i++)
		sizer->Add(new AxisLabel(this, i), wxGBPosition(i, 0), wxDefaultSpan, wxALIGN_CENTER);
}

void DisplayGrid::initTiles(MainWindow* parent) {
	tiles = std::vector<std::vector<Tile*>>(h, std::vector<Tile*>(w, nullptr));
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			wxWindowID id = (i * w) + j;
			const wxColour& colour = Colours::tile[i % 2][j % 2];
			tiles[i][j] = new Tile(this, id, knot->get(i, j), colour);
			tiles[i][j]->Bind(wxEVT_LEFT_DOWN, &MainWindow::left_click_tile, parent);
			tiles[i][j]->Bind(wxEVT_RIGHT_DOWN, &MainWindow::right_click_tile, parent);
			sizer->Add(tiles[i][j], wxGBPosition(i + 1, j + 1)/*, wxDefaultSpan, wxALIGN_CENTER*/);
		}
	}
}

void DisplayGrid::highlightSelection(Selection selection) { // This function highlights the selected area.
	clearHighlight(false);
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			tiles[i][j]->highlight();
	isAnythingHighlighted = true;
	Refresh();
}
void DisplayGrid::clearHighlight(bool refresh) {// This function clears all tiles from being highlighted.
	if (isAnythingHighlighted) {
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				tiles[i][j]->unhighlight();
		if (refresh)
			Refresh();
		isAnythingHighlighted = false;
	}
}

void DisplayGrid::drawKnot() {
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			tiles[i][j]->SetLabelText(knot->get(i, j));
}
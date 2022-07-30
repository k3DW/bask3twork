#include "DisplayGrid.h"
#include "MainWindow.h"

DisplayGrid::DisplayGrid(wxWindow* parent, Knot* knot) : wxPanel(parent), parent(parent), knot(knot), h(knot->h), w(knot->w) {
	Hide();

	sizer = new wxGridBagSizer(-1, 0);
	SetSizer(sizer);
	
	initAxisLabels();
	initTiles();

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
void DisplayGrid::initTiles() {
	tiles = std::vector<std::vector<Tile*>>(h, std::vector<Tile*>(w, nullptr));
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int id = (i * w) + j;
			const wxColour& colour = Colours::tile[i % 2][j % 2];
			tiles[i][j] = new Tile(this, id, knot->get(i, j), colour);
			tiles[i][j]->Bind(wxEVT_LEFT_DOWN, &DisplayGrid::leftClick, this);
			tiles[i][j]->Bind(wxEVT_RIGHT_DOWN, &DisplayGrid::rightClick, this);
			sizer->Add(tiles[i][j], wxGBPosition(i + 1, j + 1)/*, wxDefaultSpan, wxALIGN_CENTER*/);
		}
	}
}

void DisplayGrid::leftClick(wxMouseEvent& evt) {
	int id = evt.GetId();
	static_cast<MainWindow*>(parent)->set_selection_min({ id / w, id % w });
	static_cast<MainWindow*>(parent)->update_selection_display();
	evt.Skip();
}
void DisplayGrid::rightClick(wxMouseEvent& evt) {
	int id = evt.GetId();
	static_cast<MainWindow*>(parent)->set_selection_max({ id / w, id % w });
	static_cast<MainWindow*>(parent)->update_selection_display();
	evt.Skip();
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
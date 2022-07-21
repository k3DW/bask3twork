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
		sizer->Add(new Tile(this, wxID_ANY, i, 240, AXIS_FONT), wxGBPosition(0, i), wxDefaultSpan, wxALIGN_CENTER);
	for (int i = 1; i <= h; i++)
		sizer->Add(new Tile(this, wxID_ANY, i, 240, AXIS_FONT), wxGBPosition(i, 0), wxDefaultSpan, wxALIGN_CENTER);
}
void DisplayGrid::initTiles() {
	tiles = std::vector<std::vector<Tile*>>(h, std::vector<Tile*>(w, nullptr));
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int id = (i * w) + j;
			int grey = 255 - 10 * (i % 2 + j % 2);
			tiles[i][j] = new Tile(this, id, knot->get(i, j), grey, TILE_FONT);
			tiles[i][j]->Bind(wxEVT_LEFT_DOWN, &DisplayGrid::leftClick, this);
			tiles[i][j]->Bind(wxEVT_RIGHT_DOWN, &DisplayGrid::rightClick, this);
			sizer->Add(tiles[i][j], wxGBPosition(i + 1, j + 1)/*, wxDefaultSpan, wxALIGN_CENTER*/);
		}
	}
}

void DisplayGrid::leftClick(wxMouseEvent& evt) {
	int id = evt.GetId();
	static_cast<MainWindow*>(parent)->changeSelectCoord(id / w, id % w, -1, -1);
	evt.Skip();
}
void DisplayGrid::rightClick(wxMouseEvent& evt) {
	int id = evt.GetId();
	static_cast<MainWindow*>(parent)->changeSelectCoord(-1, -1, id / w, id % w);
	evt.Skip();
}

void DisplayGrid::highlightSelection(ijSignature) { // This function highlights the selected area.
	clearHighlight(false);
	for (int i = iMin; i <= iMax; i++)
		for (int j = jMin; j <= jMax; j++)
			tiles[i][j]->highlight(true);
	isAnythingHighlighted = true;
	Refresh();
}
void DisplayGrid::clearHighlight(bool refresh) {// This function clears all tiles from being highlighted.
	if (isAnythingHighlighted) {
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				tiles[i][j]->highlight(false);
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
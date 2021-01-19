#include "DisplayGrid.h"
#include "MainWindow.h"

DisplayGrid::DisplayGrid(wxWindow* parent, Knot* knot) : wxPanel(parent), parent(parent), knot(knot), h(knot->h), w(knot->w) {
	this->Hide();

	sizer = new wxGridBagSizer(-1, 0);
	this->SetSizer(sizer);
	axisFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
	tileFont = wxFont(wxSize(48, 48), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Celtic Knots");
	
	initAxisLabels();
	initTiles();

	this->drawKnot();
	this->Show();
}
DisplayGrid::~DisplayGrid() {
	this->Hide();
}
void DisplayGrid::initAxisLabels() {
	for (int i = 1; i <= w; i++)
		sizer->Add(new Tile(this, wxID_ANY, i, 240, axisFont), wxGBPosition(0, i), wxDefaultSpan, wxALIGN_CENTER);
	for (int i = 1; i <= h; i++)
		sizer->Add(new Tile(this, wxID_ANY, i, 240, axisFont), wxGBPosition(i, 0), wxDefaultSpan, wxALIGN_CENTER);
}
void DisplayGrid::initTiles() {
	tiles = std::vector< std::vector<Tile*> >(h, std::vector<Tile*>(w, nullptr));
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int id = (i * w) + j;
			int grey = 255 - 10 * (i % 2 + j % 2);
			tiles[i][j] = new Tile(this, id, " ", grey, tileFont);
			tiles[i][j]->Bind(wxEVT_LEFT_DOWN, &DisplayGrid::leftClick, this);
			tiles[i][j]->Bind(wxEVT_RIGHT_DOWN, &DisplayGrid::rightClick, this);
			sizer->Add(tiles[i][j], wxGBPosition(i + 1, j + 1)/*, wxDefaultSpan, wxALIGN_CENTER*/);
		}
	}
}

void DisplayGrid::leftClick(wxMouseEvent& evt) {
	int id = evt.GetId();
	static_cast<MainWindow*>(parent)->changeSelectCoord(id / w + 1, id % w + 1, 0, 0);
	evt.Skip();
}
void DisplayGrid::rightClick(wxMouseEvent& evt) {
	int id = evt.GetId();
	static_cast<MainWindow*>(parent)->changeSelectCoord(0, 0, id / w + 1, id % w + 1);
	evt.Skip();
}

void DisplayGrid::highlightSelection(ijSignature) { // This function highlights the selected area.
	clearHighlight(false);
	for (int i = iMin; i <= iMax; i++)
		for (int j = jMin; j <= jMax; j++)
			tiles[i][j]->highlight(true);
	anythingHighlighted = true;
	Refresh();
}
void DisplayGrid::clearHighlight(bool refresh) {// This function clears all tiles from being highlighted.
	if (anythingHighlighted) {
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				tiles[i][j]->highlight(false);
		if (refresh)
			Refresh();
		anythingHighlighted = false;
	}
}

void DisplayGrid::drawKnot() {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			tiles[i][j]->SetLabelText(knot->get(i, j));
		}
	}
}
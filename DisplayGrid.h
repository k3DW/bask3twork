#pragma once
#include "Constants.h"
#include "Tile.h"
#include "Knot.h"

class DisplayGrid : public wxPanel {

public:
	DisplayGrid(wxWindow* parent, Knot* knot);
	~DisplayGrid();

	void leftClick(wxMouseEvent& evt);
	void rightClick(wxMouseEvent& evt);

	void highlightSelection(ijSignature);
	void clearHighlight(bool refresh = true);

	void drawKnot();

private:
	wxWindow* const parent;
	const int h, w;
	Knot* const knot;
	wxFont axisFont, tileFont;

	std::vector<std::vector<Tile*>> tiles;
	wxGridBagSizer* sizer;
	void initAxisLabels();
	void initTiles();
	
	bool anythingHighlighted = false;
};

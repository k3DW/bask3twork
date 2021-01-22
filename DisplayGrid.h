#pragma once
#include "Constants.h"
#include "Tile.h"
#include "KnotOld.h"

class DisplayGrid : public wxPanel {

public:
	DisplayGrid(wxWindow* parent, KnotOld* knot);
	~DisplayGrid();

	void leftClick(wxMouseEvent& evt);
	void rightClick(wxMouseEvent& evt);

	void highlightSelection(ijSignature);
	void clearHighlight(bool refresh = true);

	void drawKnot();

private:
	wxWindow* parent;
	int h, w;
	KnotOld* knot;
	wxFont axisFont, tileFont;
	wxStatusBar* statusBar;

	std::vector<std::vector<Tile*>> tiles;
	wxGridBagSizer* sizer;
	void initAxisLabels();
	void initTiles();
	
	bool anythingHighlighted = false;
};
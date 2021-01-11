#pragma once
#include "wx/wx.h"
#include "Constants.h"
#include "Tile.h"
#include "Knot.h"

class DisplayGrid : public wxPanel {

public:
	DisplayGrid(wxWindow* parent, Knot* knot);
	~DisplayGrid();

	void leftClick(wxMouseEvent& evt);
	void rightClick(wxMouseEvent& evt);

	void highlightSelection(int selectNums[4]);
	void clearHighlight(bool refresh = true);

	void drawKnot();

private:
	wxWindow* parent;
	int h, w;
	Knot* knot;
	wxFont axisFont, tileFont;
	wxStatusBar* statusBar;

	std::vector< std::vector<Tile*> > tiles;
	wxGridBagSizer* sizer;
	void initAxisLabels();
	void initTiles();
	
	bool anythingHighlighted = false;
};
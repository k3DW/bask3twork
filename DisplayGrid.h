#pragma once
#include "Constants.h"
#include "Tile.h"
#include "Knot.h"

/** As a more specialized \c wxPanel object, this class represents the grid to be displayed on the left side of the MainWindow. */
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
	wxWindow* const parent;		///< The parent \c wxWindow object, which is a MainWindow in the case of this program
	const int h;				///< The height of the knot, i.e. the number of rows, taken from the Knot in the constructor
	const int w;				///< The width of the knot, i.e. the number of columns, taken from the Knot in the constructor
	Knot* const knot;			///< The Knot used for this DisplayGrid, as a const pointer to the object since each DisplayGrid corresponds to exacty one Knot object
	wxFont axisFont;			///< The font used for the X axis labels and Y axis labels
	wxFont tileFont;			///< The font used for the Tile objects used to display the Knot

	std::vector<std::vector<Tile*>> tiles;	///< The grid of Tile objects to be used
	wxGridBagSizer* sizer;					///< The sizer used to pack all the components in this \c wxPanel object
	void initAxisLabels();					///< Initializes the X axis labels and Y axis labels by directly adding \c new Tile objects to the sizer
	void initTiles();						///< Initializes the main Tile objects, packing them into the sizer and pulling initial data from the Knot
	
	bool isAnythingHighlighted = false;		///< A flag to determine if any Tile is currently highlighted
};

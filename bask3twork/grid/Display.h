#pragma once
#include <wx/panel.h>
#include "Forward.h"

/// As a more specialized \c wxPanel object, this class represents the grid to be displayed on the left side of the MainWindow.
class DisplayGrid : public wxPanel
{
public:
	DisplayGrid(MainWindow* parent, int h, int w); ///< Creates this \c DisplayGrid object with the given \c parent, with axis labels according to \c h and \c w
	~DisplayGrid();                                ///< Hides this \c DisplayGrid object automatically, so it looks like destruction is immediate

	void highlight(Selection selection); ///< Highlights the given area, unhighlighting the rest
	void unhighlight(bool refresh);      ///< Unhighlights the whole grid, and maybe refreshes the base \c wxPanel

	void draw(const Knot* knot); ///< Pulls data from the Knot object and displays it in the grid

private:
	wxGridBagSizer* sizer; ///< The sizer used to pack all the components in this \c wxPanel object
	Tiles tiles;           ///< The grid of Tile objects to be used
	bool highlighted;      ///< A flag to determine if any \c Tile is currently highlighted

	static void add_axis_labels(DisplayGrid* self, int h, int w);   ///< Creates the X and Y axis labels by directly adding new \c AxisLabel objects to the sizer
	Tiles make_tiles(MainWindow* parent); ///< Creates the \c Tile objects, packing them into the sizer and binding to the \c MainWindow
};

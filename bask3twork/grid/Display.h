#pragma once
#include <wx/panel.h>
#include "Forward.h"

/// As a more specialized \c wxPanel object, this class represents the grid to be displayed on the left side of the MainWindow.
class DisplayGrid : public wxPanel
{
public:
	DisplayGrid(MainWindow* parent, GridSize size); ///< Creates this \c DisplayGrid object with the given \c parent, with axis labels according to \c h and \c w
	~DisplayGrid();                                 ///< Hides this \c DisplayGrid object automatically, so it looks like destruction is immediate

	void highlight(Selection selection); ///< Highlights the given area, unhighlighting the rest
	void unhighlight(bool refresh);      ///< Unhighlights the whole grid, and maybe refreshes the base \c wxPanel

	void lock(Selection selection);           ///< Locks the given area, regardless of whether it was locked already
	void lock(Point point);                   ///< Locks the tile at the given coordinate, regardless of whether it was locked already
	void unlock(Selection selection);         ///< Unlocks the given area, regardless of whether it was locked already
	void unlock(Point point);                 ///< Unlocks the tile at the given coordinate, regardless of whether it was locked already
	void invert_locking(Selection selection); ///< Invert the locking on the selection

	void draw(const Knot* knot); ///< Pulls data from the Knot object and displays it in the grid

	const Tiles& get_tiles() const { return tiles; }

	void reduce_glyph_font_size_by(int i);

private:
	wxGridBagSizer* sizer; ///< The sizer used to pack all the components in this \c wxPanel object
	Tiles tiles;           ///< The grid of Tile objects to be used
	bool highlighted;      ///< A flag to determine if any \c Tile is currently highlighted

	wxFont glyph_font; ///< The glyph font, stored locally per \c DisplayGrid so we can change sizing
	wxFont axis_font;  ///< The axis font, stored locally per \c DisplayGrid so we can change sizing

	std::vector<AxisLabel*> x_axis;
	std::vector<AxisLabel*> y_axis;

	void add_axis_labels(GridSize size);   ///< Creates the X and Y axis labels by directly adding new \c AxisLabel objects to the sizer
	Tiles make_tiles(MainWindow* parent);  ///< Creates the \c Tile objects, packing them into the sizer and binding to the \c MainWindow
};

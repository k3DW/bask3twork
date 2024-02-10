#pragma once
#include <wx/window.h>
#include "Forward.h"
#include "pure/Grid.h"
#include "pure/GridSize.h"
#include "pure/Selection.h"
#include "grid/Tile.h"

/// \c DisplayGrid is the \c wxWindow where the knot gets displayed, on the left side of the \c MainWindow.
class DisplayGrid : public wxWindow
{
public:
	DisplayGrid(MainWindow* parent, GridSize size);
	~DisplayGrid();

	void resize(GridSize size);            ///< This function is effectively an assignment operator, almost everything is reset to the new grid size
	void update_sizes_and_offsets();       ///< This function keeps the same grid size, but refreshes all the offsets and pixel sizes
	void set_glyph_font_size(int i);

private:
	// Event handlers
	void on_lclick(wxMouseEvent& evt);
	void on_rclick(wxMouseEvent& evt);
	void on_paint(wxPaintEvent&);

public:
	// Rendering functions
	void render();
private:
	void render(wxDC& dc);
	void render_axis_labels(wxDC& dc);
	void render_tiles(wxDC& dc);
	void render_knot(wxDC& dc);

public:
	// Modifiers
	void highlight(Selection selection);
	void unhighlight();
	void lock(Point point);
	void lock_no_render(Point point);
	void unlock(Point point);
	void lock(Selection selection);
	void unlock(Selection selection);
	void invert_locking(Selection selection);
	void reset_tiles(); // Unhighlight and unlock

	// Misc functions
	void set_knot(const Knot* knot_) { knot = knot_; }
	const Tiles& get_tiles() const { return tiles; }
	const Tile& get_tile(Point point) const { return tiles[point.i][point.j]; }

private:
	MainWindow* parent;
	GridSize grid_size = {};
	const Knot* knot = nullptr;

	Tiles tiles;
	void make_tiles();
	void update_tile_offsets();

	wxPoint x_label_offset(int pos) const;
	wxPoint y_label_offset(int pos) const;
	wxPoint tile_offset(int i, int j) const;
	Point tile_position(wxPoint offset) const;

	wxFont glyph_font; ///< The glyph font, stored locally per \c DisplayGrid so we can change sizing
	wxFont axis_font;  ///< The axis font, stored locally per \c DisplayGrid so we can change sizing

	wxSize glyph_font_size; ///< Stores the value of \c glyph_font.GetPixelSize()
	wxPoint tiles_offset;   ///< The offset into the window where the tile grid starts
	wxSize tiles_size;      ///< The pixel size of the tile grid
	wxSize window_size;     ///< The pixel size of the entire widget, including the axes
};

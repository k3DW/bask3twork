#pragma once
#include "Constants.h"

class DisplayGrid;

/** Tile represents a glyph display tile within a DisplayGrid object. */
class Tile : public wxStaticText
{
public:
	Tile(DisplayGrid* parent, wxWindowID id, const wxString& label, const wxColour& base);
	
	void highlight();
	void unhighlight();
	
private:
	wxColour base;
};

using Tiles = std::vector<std::vector<Tile*>>;

 /** AxisLabel represents an axis label on the row or column within a DisplayGrid object. */
class AxisLabel : public wxStaticText
{
public:
	AxisLabel(DisplayGrid* parent, int value);
};

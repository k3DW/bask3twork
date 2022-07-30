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

	static const inline wxFont font{ wxSize(48, 48), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Celtic Knots" };
	
private:
	wxColour base;
};

 /** AxisLabel represents an axis label on the row or column within a DisplayGrid object. */
class AxisLabel : public wxStaticText
{
public:
	AxisLabel(DisplayGrid* parent, int value);

	static const inline wxFont font{ 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas" };
};


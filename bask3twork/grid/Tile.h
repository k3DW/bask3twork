#pragma once
#include <wx/stattext.h>
#include "Forward.h"
#include "pure/Enum.h"

/** A bit flag enum representing the possible states of a Tile. */
enum class TileState
{
	none        = 0b00,
	highlighted = 0b01,
	locked      = 0b10,
};

template <> struct opt_into_enum_operations<TileState> : std::true_type {};

/** Tile represents a glyph display tile within a DisplayGrid object. */
class Tile : public wxStaticText
{
public:
	Tile(DisplayGrid* parent, wxWindowID id, const wxString& label, const wxColour& base);
	
	void highlight();
	void unhighlight();

	void lock();
	void unlock();
	
private:
	void set_colour();

	wxColour base;
	TileState state;
};

 /** AxisLabel represents an axis label on the row or column within a DisplayGrid object. */
class AxisLabel : public wxStaticText
{
public:
	AxisLabel(DisplayGrid* parent, int value);
};

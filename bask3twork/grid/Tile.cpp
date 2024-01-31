#include "pch.h"
#include "grid/Tile.h"
#include "grid/Display.h"
#include "Constants.h"

Tile::Tile(DisplayGrid* parent, wxWindowID id, const wxString& label, const wxColour& base)
	: wxStaticText(parent, id, label)
	, base(base)
{
	SetFont(Fonts::glyph);
	unhighlight();
}

void Tile::highlight()   { state |= TileState::highlighted; set_colour(); }
void Tile::unhighlight() { state &= ~TileState::highlighted; set_colour(); }

void Tile::set_colour()
{
	switch (state)
	{
	break; case TileState::none:
		SetBackgroundColour(base);
	break; case TileState::highlighted:
		SetBackgroundColour(Colours::highlight);
	}
}

/** \fn Tile::Tile(DisplayGrid* parent, wxWindowID id, const wxString& label, const wxColour& base)
 * Sets the \c wxStaticText base object, the \c base colour, and the font.
 *
 * \param parent The parent \c DisplayGrid object
 * \param id The id value, used for identifying which tile is clicked with mouse events
 * \param label The displayed text in the \c wxStaticText object
 * \param base The colour for the background, when not highlighted
 */

/** \fn Tile::highlight(bool enable)
 * Sets the background colour to the highlighted colour
 */
/** \fn Tile::unhighlight(bool enable)
 * Sets the background colour to the base colour
 */

AxisLabel::AxisLabel(DisplayGrid* parent, int value)
	: wxStaticText(parent, wxID_ANY, wxString::Format("%i", value))
{
	SetFont(Fonts::axis);
}

/** \fn AxisLabel::AxisLabel(DisplayGrid* parent, int value)
 * Sets the \c wxStaticText base object and the font.
 *
 * \param parent The parent \c DisplayGrid object
 * \param value Displays this number as the value of the axis label
 */

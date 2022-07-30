#include "Tile.h"
#include "DisplayGrid.h"

Tile::Tile(DisplayGrid* parent, wxWindowID id, const wxString& label, int grey)
	: wxStaticText(parent, id, label)
	, base(grey, grey, grey)
{
	SetFont(font);
	unhighlight();
}

void Tile::highlight()   { SetBackgroundColour(highlighted); }
void Tile::unhighlight() { SetBackgroundColour(base); }

/** \fn Tile::Tile(DisplayGrid* parent, wxWindowID id, const wxString& label, int grey)
 * Sets the \c wxStaticText base object, the \c base colour, and the font.
 *
 * \param parent The parent \c DisplayGrid object
 * \param id The id value, used for identifying which tile is clicked with mouse events
 * \param label The displayed text in the \c wxStaticText object
 * \param grey The greyscale value (0-255) for the background colour
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
	SetFont(font);
}

/** \fn AxisLabel::AxisLabel(DisplayGrid* parent, int value)
 * Sets the \c wxStaticText base object and the font.
 *
 * \param parent The parent \c DisplayGrid object
 * \param value Displays this number as the value of the axis label
 */

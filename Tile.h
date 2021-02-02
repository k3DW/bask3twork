#pragma once
#include "Constants.h"

/** As a more specialized \c wxStaticText object, this class represents a tile for a DisplayGrid object. */
class Tile : public wxStaticText {
public:
	Tile(wxWindow* parent, wxWindowID id, const wxString& label, int grey, wxFont tileFont) : wxStaticText(parent, id, label), baseColour(grey, grey, grey) {
		SetBackgroundColour(baseColour);
		SetFont(tileFont);
	}
	Tile(wxWindow* parent, wxWindowID id, int label, int grey, wxFont tileFont) :
		Tile(parent, id, wxString::Format(wxT("%i"), label), grey, tileFont) {}
	Tile(wxWindow* parent, wxWindowID id, int grey, wxFont tileFont) :
		Tile(parent, id, "", grey, tileFont) {}
	
	inline void highlight(bool enable = true) {
		SetBackgroundColour(enable ? HL_COLOUR : baseColour);
	}

private:
	const wxColour baseColour; ///< The background colour of the tile, without highlight
};

/* Tile constructors */
/** \fn Tile::Tile(wxWindow* parent, wxWindowID id, const wxString& label, int grey, wxFont tileFont)
 * The main constructor. Sets the \c wxStaticText base object, the baseColour, and the font.
 * 
 * \param parent The parent \c wxWindow object, which is a DisplayGrid in the case of this program
 * \param id The identification value for this \c wxStaticText object
 * \param label The displayed text in the \c wxStaticText object
 * \param grey The greyscale value (0-255) for the background colour
 * \param tileFont The font to display the given text
 */
/** \fn Tile::Tile(wxWindow* parent, wxWindowID id, int label, int grey, wxFont tileFont)
 * A constructor that calls the main constructor, taking an integer as the displayed text instead of a \c wxString.
 * 
 * See Tile::Tile()
 */
/** \fn Tile::Tile(wxWindow* parent, wxWindowID id, int grey, wxFont tileFont)
 * A constructor that calls the main constructor, taking no value for the displayed text, and forwarding an empty string.
 * 
 * See Tile::Tile()
 */

/* Other */
/** \fn Tile::highlight(bool enable = true)
 * This function turns the background colour to the highlighted colour or back to the base colour, depending on the value of \c enable.
 */
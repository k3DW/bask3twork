#pragma once
#include "Constants.h"

/** Deriving from \c wxStaticText, Tile represents a glyph display tile within a DisplayGrid object. */
class Tile : public wxStaticText
{
public:
	Tile(wxWindow* parent, wxWindowID id, const wxString& label, int grey)
		: wxStaticText(parent, id, label), base(grey, grey, grey)
	{
		SetBackgroundColour(base);
		SetFont(font);
	}
	
	void highlight(bool enable) { SetBackgroundColour(enable ? highlighted : base); }

	static const inline wxColour highlighted{ 150, 200, 255 };
	static const inline wxFont font{ wxSize(48, 48), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Celtic Knots" };
	
private:
	wxColour base;
};

/** \fn Tile::Tile(wxWindow* parent, wxWindowID id, const wxString& label, int grey)
 * The main constructor, sets the \c wxStaticText base object, the \c baseColour, and the font.
 * 
 * \param parent The parent \c wxWindow object, which is a DisplayGrid in the case of this program
 * \param id The identification value for this \c wxStaticText object
 * \param label The displayed text in the \c wxStaticText object
 * \param grey The greyscale value (0-255) for the background colour
 */
/** \fn Tile::highlight(bool enable)
 * Sets the background colour as either the highlighted colour or the base colour, depending on the value of \c enable.
 */

 /** Deriving from \c wxStaticText, AxisLabel represents an axis label on the row or column within a DisplayGrid object. */
class AxisLabel : public wxStaticText
{
public:
	AxisLabel(wxWindow* parent, int value)
		: wxStaticText(parent, wxID_ANY, wxString::Format("%i", value))
	{
		SetFont(font);
	}

	static const inline wxFont font{ 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas" };
};

/** \fn AxisLabel::AxisLabel(wxWindow* parent, int value)
 * The main constructor, sets the \c wxStaticText base object, the \c baseColour, and the font.
 *
 * \param parent The parent \c wxWindow object, which is a DisplayGrid in the case of this program
 * \param value Displays this number as the value of the label
 */

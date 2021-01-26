#pragma once
#include "Constants.h"

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
	const wxColour baseColour;
};

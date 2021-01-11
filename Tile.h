#pragma once
#include "wx/wx.h"
#include "Constants.h"

class Tile : public wxStaticText {

public:
	Tile(wxWindow* parent, wxWindowID id, const wxString& label, int grey, wxFont tileFont) : wxStaticText(parent, id, label) {
		baseColour = wxColour(grey, grey, grey);
		this->SetBackgroundColour(baseColour);
		this->SetFont(tileFont);
	}
	Tile(wxWindow* parent, wxWindowID id, int label, int grey, wxFont tileFont) :
		Tile(parent, id, wxString::Format(wxT("%i"), label), grey, tileFont) {}
	Tile(wxWindow* parent, wxWindowID id, int grey, wxFont tileFont) :
		Tile(parent, id, "", grey, tileFont) {}
	
	void highlight(bool enable = true) {
		this->SetBackgroundColour(enable ? HL_COLOUR : baseColour);
	}

private:
	wxColour baseColour;
};
#pragma once
#include "wx/wx.h"
#include <array>
#include <optional>
#include <vector>

// Converts integer to wxString
#define intWX(x) wxString::Format(wxT("%i"),x)

// Converts a MAJOR, MINOR, and PATCH version number into a total version number as a wxString
#define VERSION(MAJOR, MINOR, PATCH) intWX(MAJOR) + "." + intWX(MINOR) + (PATCH == 0 ? wxString("") : wxString(".") + intWX(PATCH))

const int SCREEN_X = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);	// The screen size in the x direction
const int SCREEN_Y = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);	// The screen size in the y direction
const wxSize MAX_SIZE(SCREEN_X, SCREEN_Y);							// The maximum size of the app window


// All the types of symmetry types to be generated in this program
enum SymmetryType {
	NoSym,			// No symmetry
	HoriSym,		// Mirror symmetry across the horizontal axis
	VertSym,		// Mirror symmetry across the vertical axis
	HoriVertSym,	// Mirror symmetry across both the horizontal and vertical axes
	Rot2Sym,		// 2-way rotational symmetry
	Rot4Sym			// 4-way rotational symmetry
};

// This section is a quicker way of outputting wxMessageBox popups for debugging
extern wxString tempString;
#define msg(x) \
	tempString = ""; \
	tempString << x; \
	wxMessageBox(tempString)
#define msgIter(list) \
	tempString = ""; \
	for (int c = 0; c < list.size(); c++) tempString << " " << list[c]; \
	wxMessageBox(tempString)

/* Begin definitions for `MainWindow` class */ #if true
	#define declareButton(buttonName) \
		void buttonName##Function(wxCommandEvent& evt); \
		wxButton* buttonName##Button
	#define initButton(buttonName, displayText) \
		buttonName##Button = new wxButton(this, wxID_ANY, displayText); \
		buttonName##Button->Bind(wxEVT_BUTTON, &MainWindow::##buttonName##Function, this)

	#define declareGenerateButton(SymType) \
		wxButton* generate##SymType##Button
	#define initGenerateButton(SymType, displayText) \
		generate##SymType##Button = new wxButton(this, SymType, displayText); \
		generate##SymType##Button->Bind(wxEVT_BUTTON, &MainWindow::generateKnot, this); \
		generateRegionSizer->Add(generate##SymType##Button)

	#define GAP_1 20
	#define GAP_2 10
	#define GAP_3 5
#endif /* End definitions for `MainWindow` class */

/* Begin definitions for `Tile` class */ #if true
	const wxColour BACKGROUND_COLOUR(240, 240, 240);
	const wxColour HL_COLOUR(150, 200, 255);
#endif /* End definitions for `Tile` class */

/* Begin definitions for `Knot` class */ #if true
	#define MAX_ATTEMPTS 10000
	#define ATTEMPTS_DISPLAY_INCREMENT 500
	const wxString MAX_ATTEMPTS_STR = intWX(MAX_ATTEMPTS);
	#define ijSignature const int iMin, const int jMin, const int iMax, const int jMax
#endif /* End definitions for `Knot` class */

template<typename T>
inline T pick_random(const std::vector<T>& vec) {
	return vec[rand() % vec.size()];
}

template<typename T>
T pop_random(std::vector<T>& vec) {
	int index = rand() % vec.size();	// Grab a random index
	T output(vec[index]);				// Store the value at that index
	vec.erase(vec.begin() + index);		// Remove the value from the vector
	return output;						// Return the value
}
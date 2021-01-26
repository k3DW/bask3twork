#pragma once
#include "wx/wx.h"
#include <array>
#include <optional>
#include <vector>

// All the types of symmetry which can be generated in this program
enum class Symmetry : unsigned int {
	NoSym,			// No symmetry
	HoriSym,		// Mirror symmetry across the horizontal axis
	VertSym,		// Mirror symmetry across the vertical axis
	HoriVertSym,	// Mirror symmetry across both the horizontal and vertical axes
	Rot2Sym,		// 2-way rotational symmetry
	Rot4Sym			// 4-way rotational symmetry
};

// Converts integer to wxString
#define intWX(x) wxString::Format(wxT("%i"),x)

// Constants for future functionality, changing the sizing of things depending on the screen size
const int SCREEN_X = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);	// The screen size in the x direction
const int SCREEN_Y = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);	// The screen size in the y direction
const wxSize MAX_SIZE(SCREEN_X, SCREEN_Y);							// The maximum size of the app window

// Constants for the colours of the program
const wxColour BACKGROUND_COLOUR(240, 240, 240);	// The default colour of the background for the program
const wxColour HL_COLOUR(150, 200, 255);			// The colour that denotes a "highlighted" tile

// Constants for the iterations of knot generating
static const int MAX_ATTEMPTS = 10000;				// The maximum number of attempts for the knot to try generating
static const int ATTEMPTS_DISPLAY_INCREMENT = 500;	// The interval at which the number of iterations is displayed
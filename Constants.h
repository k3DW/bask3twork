#pragma once
#include "wx/wx.h"
#include "wx/gbsizer.h"
#include "wx/wfstream.h"
#include <array>
#include <optional>
#include <vector>
/// \file

using ull = unsigned long long;

/// The X macro used on the symmetries, to make the code simpler
#define SYMMETRIES \
	XX(NoSym, "No Symmetry") \
	XX(HoriSym, "Horizontal Reflection") \
	XX(VertSym, "Vertical Reflection") \
	XX(HoriVertSym, "Horizontal + Vertical") \
	XX(Rot2Sym, "2-way Rotational") \
	XX(Rot4Sym, "4-way Rotational") \
	XX(FwdDiag, "Forward Diagonal") \
	XX(BackDiag, "Backward Diagonal") \
	XX(FullSym, "Full Symmetry")

/// All the types of symmetry which can be used in this program
enum class Symmetry : unsigned int {
	NoSym,			///< No symmetry
	HoriSym,		///< Mirror symmetry across the horizontal axis
	VertSym,		///< Mirror symmetry across the vertical axis
	HoriVertSym,	///< Mirror symmetry across both the horizontal and vertical axes
	Rot2Sym,		///< 2-way rotational symmetry
	Rot4Sym,		///< 4-way rotational symmetry (square only)
	FwdDiag,		///< Mirror symmetry across the forward diagonal (square only)
	BackDiag,		///< Mirror symmetry across the backward diagonal (square only)
	FullSym,		///< 4-fold dihedral symmetry (square only)
};

/// A shortcut for this function signature, which is used often throughout the code
#define ijSignature const int iMin, const int jMin, const int iMax, const int jMax

/// Converts integer to wxString
#define intWX(x) wxString::Format(wxT("%i"),x)

// Constants for future functionality, changing the sizing of things depending on the screen size
const int SCREEN_X = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);	///< For future functionality, the screen size in the x direction
const int SCREEN_Y = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);	///< For future functionality, the screen size in the y direction
const wxSize MAX_SIZE{ SCREEN_X, SCREEN_Y };						///< For future functionality, the maximum size of the app window

// Constants for the colours of the program
const wxColour BACKGROUND_COLOUR{ 240, 240, 240 };	///< The default colour of the background for the program
const wxColour HL_COLOUR{ 150, 200, 255 };			///< The colour that denotes a "highlighted" tile

// Constants for the fonts of the program, only used in DisplayGrid
// For future functionality, the sizes of these fonts might need to be parametrized
const wxFont AXIS_FONT{ 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas" };					///< The font for the axis labels of the DisplayGrid
const wxFont TILE_FONT{ wxSize(48, 48), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Celtic Knots" };	///< The font for the min Tile objects of the DisplayGrid
const wxFont TEXT_FONT{ 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL };									///< The font for the \c selectCoords and the \c grid text boxes

// Constants for the iterations of knot generating
static const int MAX_ATTEMPTS = 10000;				///< The maximum number of attempts for the Knot to try generating
static const int ATTEMPTS_DISPLAY_INCREMENT = 500;	///< The interval at which the number of iterations is displayed
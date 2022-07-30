#pragma once
#include "wx/wx.h"
#include "wx/clipbrd.h"
#include "wx/gbsizer.h"
#include "wx/textfile.h"
#include "wx/wfstream.h"
#include <array>
#include <map>
#include <optional>
#include <vector>
/// \file

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

/** All the types of symmetry which can be used in this program.
 * The bit flags tell Knot::generate() how to generate.
 * In order from least-significant to most-significant bit, the bits mean the following.
 * (1) This symmetry includes horizontal reflection.
 * (2) This symmetry includes vertical reflection.
 * (3) This symmetry includes 2-fold rotation.
 * (4) This symmetry includes 4-fold rotation.
 * (5) This symmetry includes forward diagonal reflection.
 * (6) This symmetry includes backward diagonal reflection.
 */
enum class Symmetry : unsigned int {
	Off			= 0b0000000,	///< Different from "no symmetry", because this is when there should be nothing at all
	NoSym		= 0b0000001,	///< No symmetry
	HoriSym		= 0b0000011,	///< Mirror symmetry across the horizontal axis
	VertSym		= 0b0000101,	///< Mirror symmetry across the vertical axis
	HoriVertSym = 0b0001111,	///< Mirror symmetry across both the horizontal and vertical axes
	Rot2Sym		= 0b0001001,	///< 2-way rotational symmetry
	Rot4Sym		= 0b0011001,	///< 4-way rotational symmetry (square only)
	FwdDiag		= 0b0100001,	///< Mirror symmetry across the forward diagonal (square only)
	BackDiag	= 0b1000001,	///< Mirror symmetry across the backward diagonal (square only)
	FullSym		= 0b1111111,	///< 4-fold dihedral symmetry (square only)
};

constexpr inline Symmetry operator&(Symmetry lhs, Symmetry rhs) { return static_cast<Symmetry>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs)); }
constexpr inline Symmetry operator|(Symmetry lhs, Symmetry rhs) { return static_cast<Symmetry>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs)); }

constexpr inline Symmetry operator*(Symmetry sym, bool b) { return static_cast<Symmetry>(static_cast<unsigned int>(sym) * b); }

// Constants for future functionality, changing the sizing of things depending on the screen size
const int SCREEN_X = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);	///< For future functionality, the screen size in the x direction
const int SCREEN_Y = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);	///< For future functionality, the screen size in the y direction
const wxSize MAX_SIZE{ SCREEN_X, SCREEN_Y };						///< For future functionality, the maximum size of the app window
constexpr size_t MAX_H = 100;	///< The maximum height of the grid allowed in the program, in terms of tiles.
constexpr size_t MAX_W = 100;	///< The maximum width of the grid allowed in the program, in terms of tiles

namespace Colours
{
	const wxColour background = wxColour(240, 240, 240); ///< The default background colour of the whole program
	const wxColour highlight  = wxColour(150, 200, 255); ///< The highlighted colour of a \c Tile

	/// The colours of the Tile objects in the DisplayGrid, in a repeating 2x2 pattern
	const std::array tile =
	{
		std::array{ wxColour(255, 255, 255), wxColour(245, 245, 245) },
		std::array{ wxColour(245, 245, 245), wxColour(235, 235, 235) },
	};
}

// Constants for the fonts of the program, only used in DisplayGrid
// For future functionality, the sizes of these fonts might need to be parametrized
const wxFont TEXT_FONT{ 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL };									///< The font for the \c selectCoords and the \c grid text boxes

// Constants for the iterations of knot generating
constexpr int MAX_ATTEMPTS = 10000;				///< The maximum number of attempts for the Knot to try generating
constexpr int ATTEMPTS_DISPLAY_INCREMENT = 500;	///< The interval at which the number of iterations is displayed

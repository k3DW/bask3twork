#pragma once
#include <wx/colour.h>
#include <wx/font.h>
#include <array>
/// \file

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

namespace Fonts
{
	const wxFont select     = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); ///< The font for the Select box "(a,b) to (c,d)"
	const wxFont regenerate = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); ///< The font for the Regenerate pop-up text boxes
	const wxFont glyph      = wxFont(wxSize(48, 48), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Celtic Knots"); ///< The font for the glyphs in the \c Tile objects
	const wxFont axis       = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas"); ///< The font for the \c AxisLabel objects
	const wxFont export_    = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas"); ///< The font to be used in the export box
}

// Constants for the iterations of knot generating
constexpr int MAX_ATTEMPTS = 10000;				///< The maximum number of attempts for the Knot to try generating
constexpr int ATTEMPTS_DISPLAY_INCREMENT = 500;	///< The interval at which the number of iterations is displayed

namespace Borders
{
	static constexpr int outside      = 20; ///< The gap from the outside of the window, and between the grid section and panel section
	static constexpr int inter_region = 10; ///< The gap between the regions in the region section
	static constexpr int sub_region   = 5;  ///< The gap between elements within a region or a dialog
}

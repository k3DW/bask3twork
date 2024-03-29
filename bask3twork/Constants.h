#pragma once
#include <wx/colour.h>
#include <wx/font.h>
#include <array>
/// \file

namespace Limits
{
	constexpr size_t rows    = 100; ///< The maximum height of the grid allowed in the program, in terms of tiles.
	constexpr size_t columns = 100; ///< The maximum width of the grid allowed in the program, in terms of tiles
}	

namespace Colours
{
	const wxColour background = wxColour(240, 240, 240); ///< The default background colour of the whole program

	/// The colours of the Tile objects in the DisplayGrid, in a repeating 2x2 pattern
	const std::array tile_base =
	{
		std::array{ wxColour(255, 255, 255), wxColour(245, 245, 245) },
		std::array{ wxColour(245, 245, 245), wxColour(235, 235, 235) },
	};

	/// The colours of a highlighted \c Tile, in a repeating 2x2 pattern
	const std::array tile_highlighted =
	{
		std::array{ wxColour(150, 200, 255), wxColour(150, 200, 255) },
		std::array{ wxColour(150, 200, 255), wxColour(150, 200, 255) },
	};

	/// The colours of a locked \c Tile, in a repeating 2x2 pattern
	const std::array tile_locked =
	{
		std::array{ wxColour(255, 144, 84), wxColour(245, 139, 82) },
		std::array{ wxColour(245, 139, 82), wxColour(235, 134, 80) },
	};

	/// The colours of a highlighted and locked \c Tile, in a repeating 2x2 pattern, see https://meyerweb.com/eric/tools/color-blend/
	const std::array tile_highlighted_locked =
	{
		std::array{ wxColour(193, 172, 176), wxColour(193, 172, 176) },
		std::array{ wxColour(193, 172, 176), wxColour(193, 172, 176) },
	};
}

namespace Sizes
{
	const wxSize button = wxSize(65, 23);
	const wxSize glyph_font_pixel = wxSize(48, 48);
	const int font_point = 12;
}

namespace Fonts
{
	const wxFont select     = wxFont(Sizes::font_point, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); ///< The font for the Select box "(a,b) to (c,d)"
	const wxFont regenerate = wxFont(Sizes::font_point, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); ///< The font for the Regenerate pop-up text boxes
	const wxFont glyph      = wxFont(Sizes::glyph_font_pixel, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Celtic Knots"); ///< The font for the glyphs in the \c Tile objects
	const wxFont axis       = wxFont(Sizes::font_point, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas"); ///< The font for the \c AxisLabel objects
	const wxFont export_    = wxFont(Sizes::font_point, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas"); ///< The font to be used in the export box

	constexpr int reduce_by = 2;
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

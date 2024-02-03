#pragma once
#include <wx/stattext.h>
#include "Forward.h"
#include "pure/UsableEnum.h"

/** A bit flag enum representing the possible states of a Tile. */
enum class TileState
{
	none        = 0b00,
	highlighted = 0b01,
	locked      = 0b10,
};

template <> struct opt_into_enum_operations<TileState> : std::true_type {};



/** TileColours is a struct containing references to the colours a tile will need; its static members represent all possible variations. */
struct TileColours
{
	const wxColour& base;
	const wxColour& highlighted;
	const wxColour& locked;
	const wxColour& highlighted_locked;

	static const TileColours _00;
	static const TileColours _01;
	static const TileColours _10;
	static const TileColours _11;

	static constexpr std::array all
	{
		std::array { std::ref(_00), std::ref(_01) },
		std::array { std::ref(_10), std::ref(_11) },
	};
};

inline constexpr const TileColours TileColours::_00 = { .base = Colours::tile_base[0][0], .highlighted = Colours::tile_highlighted[0][0], .locked = Colours::tile_locked[0][0], .highlighted_locked = Colours::tile_highlighted_locked[0][0] };
inline constexpr const TileColours TileColours::_01 = { .base = Colours::tile_base[0][1], .highlighted = Colours::tile_highlighted[0][1], .locked = Colours::tile_locked[0][1], .highlighted_locked = Colours::tile_highlighted_locked[0][1] };
inline constexpr const TileColours TileColours::_10 = { .base = Colours::tile_base[1][0], .highlighted = Colours::tile_highlighted[1][0], .locked = Colours::tile_locked[1][0], .highlighted_locked = Colours::tile_highlighted_locked[1][0] };
inline constexpr const TileColours TileColours::_11 = { .base = Colours::tile_base[1][1], .highlighted = Colours::tile_highlighted[1][1], .locked = Colours::tile_locked[1][1], .highlighted_locked = Colours::tile_highlighted_locked[1][1] };



/** Tile represents a glyph display tile within a DisplayGrid object. */
class Tile : public wxStaticText
{
public:
	Tile(DisplayGrid* parent, wxWindowID id, const wxString& label, const TileColours& colours);
	
	void highlight();
	void unhighlight();

	void lock();
	void unlock();
	
private:
	void set_colour();

	TileColours colours;
	TileState state;
};

 /** AxisLabel represents an axis label on the row or column within a DisplayGrid object. */
class AxisLabel : public wxStaticText
{
public:
	AxisLabel(DisplayGrid* parent, int value);
};

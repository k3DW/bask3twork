#pragma once
#include <wx/brush.h>
#include <bit>
#include "Forward.h"
#include "pure/UsableEnum.h"

enum class TileHighlighted : bool { no, yes };
enum class TileLocked : bool { no, yes };

/** A bit flag struct representing the possible states of a Tile. */
struct TileState
{
	TileHighlighted highlighted = TileHighlighted::no;
	TileLocked locked = TileLocked::no;

	constexpr operator std::uint16_t() const
	{
		static_assert(sizeof(*this) == sizeof(std::uint16_t));
		return std::bit_cast<std::uint16_t>(*this);
	}
};

constexpr TileState operator|(TileHighlighted highlighted, TileLocked locked)
{
	return { .highlighted = highlighted, .locked = locked };
}



/** TileColours is a struct containing references to the colours a tile will need; its static members represent all possible variations. */
struct TileBrushes
{
	const wxBrush base;
	const wxBrush highlighted;
	const wxBrush locked;
	const wxBrush highlighted_locked;

	static const TileBrushes _00;
	static const TileBrushes _01;
	static const TileBrushes _10;
	static const TileBrushes _11;

	static constexpr std::array all
	{
		std::array { std::ref(_00), std::ref(_01) },
		std::array { std::ref(_10), std::ref(_11) },
	};
};

inline const TileBrushes TileBrushes::_00 = { .base = Colours::tile_base[0][0], .highlighted = Colours::tile_highlighted[0][0], .locked = Colours::tile_locked[0][0], .highlighted_locked = Colours::tile_highlighted_locked[0][0] };
inline const TileBrushes TileBrushes::_01 = { .base = Colours::tile_base[0][1], .highlighted = Colours::tile_highlighted[0][1], .locked = Colours::tile_locked[0][1], .highlighted_locked = Colours::tile_highlighted_locked[0][1] };
inline const TileBrushes TileBrushes::_10 = { .base = Colours::tile_base[1][0], .highlighted = Colours::tile_highlighted[1][0], .locked = Colours::tile_locked[1][0], .highlighted_locked = Colours::tile_highlighted_locked[1][0] };
inline const TileBrushes TileBrushes::_11 = { .base = Colours::tile_base[1][1], .highlighted = Colours::tile_highlighted[1][1], .locked = Colours::tile_locked[1][1], .highlighted_locked = Colours::tile_highlighted_locked[1][1] };



class Tile
{
public:
	Tile(const TileBrushes& brushes, wxPoint offset);

	void set_offset(wxPoint p) { offset = p; }

	void lock() { _locked = TileLocked::yes; }
	void unlock() { _locked = TileLocked::no; }

	bool locked() const { return _locked == TileLocked::yes; }

	void render(wxDC& dc, wxSize size, TileHighlighted highlight) const;

private:
	const TileBrushes& brushes;
	TileLocked _locked = TileLocked::no;
	wxPoint offset;
};

#pragma once
#include <wx/brush.h>
#include "Forward.h"
#include "pure/UsableEnum.h"

/** A bit flag enum representing the possible states of a Tile. */
enum class TileState
{
	none        = 0b00,
	highlighted = 0b01,
	locked      = 0b10,

	highlighted_locked = highlighted | locked,
};

template <> struct opt_into_enum_operations<TileState> : std::true_type {};



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

	void reset_state() { state = TileState::none; }
	void set_offset(wxPoint p) { offset = p; }

	void highlight() { state |= TileState::highlighted; }
	void unhighlight() { state &= ~TileState::highlighted; }

	void lock() { state |= TileState::locked; }
	void unlock() { state &= ~TileState::locked; }

	bool locked() const { return (state & TileState::locked) != TileState::none; }

	void render(wxDC& dc, wxSize size) const;

private:
	const TileBrushes& brushes;
	TileState state = TileState::none;
	wxPoint offset;
};

#include "pch.h"
#include "grid/Tile.h"
#include "grid/Display.h"
#include "Constants.h"

Tile::Tile(DisplayGrid* parent, const TileBrushes& brushes, wxPoint offset)
	: brushes(brushes)
	, offset(offset)
{}

void Tile::render(wxDC& dc, wxSize size) const
{
	switch (state)
	{
	break; case TileState::none:
		dc.SetBrush(brushes.get().base);
	break; case TileState::highlighted:
		dc.SetBrush(brushes.get().highlighted);
	break; case TileState::locked:
		dc.SetBrush(brushes.get().locked);
	break; case TileState::highlighted | TileState::locked:
		dc.SetBrush(brushes.get().highlighted_locked);
	}
	dc.DrawRectangle(offset, size);
}

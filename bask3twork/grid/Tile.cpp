#include "pch.h"
#include "grid/Tile.h"
#include "grid/Display.h"
#include "Constants.h"

Tile::Tile(const TileBrushes& brushes, wxPoint offset)
	: brushes(brushes)
	, offset(offset)
{}

void Tile::render(wxDC& dc, wxSize size) const
{
	switch (state)
	{
	break; case TileState::none:
		dc.SetBrush(brushes.base);
	break; case TileState::highlighted:
		dc.SetBrush(brushes.highlighted);
	break; case TileState::locked:
		dc.SetBrush(brushes.locked);
	break; case TileState::highlighted_locked:
		dc.SetBrush(brushes.highlighted_locked);
	}
	dc.DrawRectangle(offset, size);
}

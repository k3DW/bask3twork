#include "pch.h"
#include "grid/Tile.h"
#include "grid/Display.h"
#include "Constants.h"

Tile::Tile(const TileBrushes& brushes, wxPoint offset)
	: brushes(brushes)
	, offset(offset)
{}

void Tile::render(wxDC& dc, wxSize size, TileHighlighted highlight) const
{
	switch (highlight | _locked)
	{
	break; case TileHighlighted::no | TileLocked::no:
		dc.SetBrush(brushes.base);
	break; case TileHighlighted::yes | TileLocked::no:
		dc.SetBrush(brushes.highlighted);
	break; case TileHighlighted::no | TileLocked::yes:
		dc.SetBrush(brushes.locked);
	break; case TileHighlighted::yes | TileLocked::yes:
		dc.SetBrush(brushes.highlighted_locked);
	}
	dc.DrawRectangle(offset, size);
}

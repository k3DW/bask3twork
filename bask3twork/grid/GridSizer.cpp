#include "pch.h"
#include "grid/GridSizer.h"
#include "grid/Display.h"

GridSizer::GridSizer(DisplayGrid* display)
	: wxBoxSizer(wxVERTICAL)
{
	AddStretchSpacer();
	Add(display);
	AddStretchSpacer();
}

void GridSizer::update(DisplayGrid* display)
{
	Insert(1, display, 0, wxEXPAND);
}

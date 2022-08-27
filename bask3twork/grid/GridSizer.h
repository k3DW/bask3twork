#pragma once
#include "wx/wx.h"

class DisplayGrid;

class GridSizer : public wxBoxSizer
{
public:
	GridSizer(DisplayGrid* display);

	void update(DisplayGrid* display);
};

#pragma once
#include "wx/wx.h"

class GridSizer : public wxBoxSizer
{
public:
	GridSizer(DisplayGrid* display);

	void update(DisplayGrid* display);
};

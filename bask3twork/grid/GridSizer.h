#pragma once
#include <wx/sizer.h>
#include "Forward.h"

class GridSizer : public wxBoxSizer
{
public:
	GridSizer(DisplayGrid* display);

	void update(DisplayGrid* display);
};

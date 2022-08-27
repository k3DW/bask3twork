#pragma once
#include <wx/sizer.h>
#include "Forward.h"

class RegionSizer : public wxBoxSizer
{
public:
	RegionSizer(SelectRegion* select_region, GenerateRegion* generate_region, ExportRegion* export_region);
};

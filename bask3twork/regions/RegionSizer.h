#pragma once
#include "wx/wx.h"

class SelectRegion;
class GenerateRegion;
class ExportRegion;

class RegionSizer : public wxBoxSizer
{
public:
	RegionSizer(SelectRegion* select_region, GenerateRegion* generate_region, ExportRegion* export_region);
};

#include "pch.h"
#include "regions/RegionSizer.h"
#include "regions/Select.h"
#include "regions/Generate.h"
#include "regions/Export.h"
#include "Constants.h"

RegionSizer::RegionSizer(SelectRegion* select_region, GenerateRegion* generate_region, ExportRegion* export_region)
	: wxBoxSizer(wxVERTICAL)
{
	AddStretchSpacer();
	Add(select_region);
	AddSpacer(GAP_2);
	Add(generate_region);
	AddSpacer(GAP_2);
	Add(export_region);
	AddStretchSpacer();
}

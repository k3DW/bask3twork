#include "pch.h"
#include "regions/Generate.h"
#include "pure/Symmetry.h"
#include "MainWindow.h"

GenerateRegion::GenerateRegion(MainWindow* parent)
	: wxStaticBoxSizer(wxVERTICAL, parent, "Generate")
{
	buttons =
	{
		new GenerateRegionButton(parent, Symmetry::Nothing,     "Clear Selection"),
		new GenerateRegionButton(parent, Symmetry::AnySym,      "No Symmetry"),
		new GenerateRegionButton(parent, Symmetry::HoriSym,     "Horizontal Reflection"),
		new GenerateRegionButton(parent, Symmetry::VertSym,     "Vertical Reflection"),
		new GenerateRegionButton(parent, Symmetry::HoriVertSym, "Horizontal + Vertical"),
		new GenerateRegionButton(parent, Symmetry::Rot2Sym,     "2-way Rotational"),
		new GenerateRegionButton(parent, Symmetry::Rot4Sym,     "4-way Rotational"),
		new GenerateRegionButton(parent, Symmetry::FwdDiag,     "Forward Diagonal"),
		new GenerateRegionButton(parent, Symmetry::BackDiag,    "Backward Diagonal"),
		new GenerateRegionButton(parent, Symmetry::FullSym,     "Full Symmetry"),
	};

	for (auto button : buttons)
		Add(button);

	disable_buttons();
}

void GenerateRegion::enable_buttons(Symmetry symmetry)
{
	for (auto button : buttons)
		button->Enable(symmetry % button->symmetry);
}

void GenerateRegion::disable_buttons()
{
	for (auto button : buttons)
		button->Disable();
}

bool GenerateRegion::is_enabled(Symmetry symmetry) const
{
	switch (symmetry)
	{
	case Symmetry::Nothing:     return buttons[0]->IsEnabled();
	case Symmetry::AnySym:      return buttons[1]->IsEnabled();
	case Symmetry::HoriSym:     return buttons[2]->IsEnabled();
	case Symmetry::VertSym:     return buttons[3]->IsEnabled();
	case Symmetry::HoriVertSym: return buttons[4]->IsEnabled();
	case Symmetry::Rot2Sym:     return buttons[5]->IsEnabled();
	case Symmetry::Rot4Sym:     return buttons[6]->IsEnabled();
	case Symmetry::FwdDiag:     return buttons[7]->IsEnabled();
	case Symmetry::BackDiag:    return buttons[8]->IsEnabled();
	case Symmetry::FullSym:     return buttons[9]->IsEnabled();
	default: throw;
	}
}

GenerateRegionButton::GenerateRegionButton(MainWindow* parent, Symmetry symmetry, const char* label)
	: wxButton(parent, static_cast<wxWindowID>(symmetry), label)
	, symmetry(symmetry)
{
	Bind(wxEVT_BUTTON, &MainWindow::generate_knot, parent);
}

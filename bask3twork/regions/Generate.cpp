#include "pch.h"
#include "regions/Generate.h"
#include "pure/Symmetry.h"
#include "MainWindow.h"

GenerateRegion::GenerateRegion(MainWindow* parent)
	: wxStaticBoxSizer(wxVERTICAL, parent, "Generate")
{
	buttons =
	{
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

GenerateRegionButton::GenerateRegionButton(MainWindow* parent, Symmetry symmetry, const char* label)
	: wxButton(parent, static_cast<wxWindowID>(symmetry), label)
	, symmetry(symmetry)
{
	Bind(wxEVT_BUTTON, &MainWindow::generateKnot, parent);
}

#include "GenerateRegion.h"
#include "MainWindow.h"

GenerateRegion::GenerateRegion(MainWindow* parent)
	: wxStaticBoxSizer(wxVERTICAL, parent, "Generate")
{
	#define XX(Sym, desc) \
		Sym##Button = new wxButton(parent, static_cast<int>(Symmetry::Sym), desc); \
		Sym##Button->Bind(wxEVT_BUTTON, &MainWindow::generateKnot, parent); \
		Add(Sym##Button);
	SYMMETRIES
	#undef XX

	disable_buttons();
}

void GenerateRegion::enable_buttons(Symmetry sym)
{
	#define XX(Sym, desc) \
		Sym##Button->Enable((sym & Symmetry::Sym) == Symmetry::Sym);
	SYMMETRIES
	#undef XX
}

void GenerateRegion::disable_buttons()
{
	#define XX(Sym, desc) \
		Sym##Button->Disable();
	SYMMETRIES
	#undef XX
}

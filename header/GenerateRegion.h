#pragma once
#include "wx/wx.h"
#include "Constants.h"
#include "Symmetry.h"

class MainWindow;

/// The buttons inside the GenerateRegion allow the Knot \c generate functions to have no conditional operation, and just assume that the parameters are valid.
/// Instead of checking for symmetry conditions within the generating function itself, the user is disallowed from pressing the button at all, unless the symmetry works.
class GenerateRegion : public wxStaticBoxSizer
{
public:
	GenerateRegion(MainWindow* parent);

	void enable_buttons(Symmetry sym);
	void disable_buttons();

private:
	#define XX(Sym, desc) wxButton* Sym##Button;
	SYMMETRIES
	#undef XX
};

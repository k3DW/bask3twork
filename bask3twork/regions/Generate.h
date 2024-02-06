#pragma once
#include <wx/button.h>
#include <wx/sizer.h>
#include <array>
#include "Forward.h"

/// The buttons inside the GenerateRegion allow the Knot \c generate functions to have no conditional operation, and just assume that the parameters are valid.
/// Instead of checking for symmetry conditions within the generating function itself, the user is disallowed from pressing the button at all, unless the symmetry works.
class GenerateRegion : public wxStaticBoxSizer
{
public:
	GenerateRegion(MainWindow* parent);

	void enable_buttons(Symmetry symmetry);
	void disable_buttons();

private:
	std::array<GenerateRegionButton*, 10> buttons;
};

class GenerateRegionButton : public wxButton
{
private:
	friend class GenerateRegion;

	GenerateRegionButton(MainWindow* parent, Symmetry symmetry, const char* label);

	Symmetry symmetry;
};

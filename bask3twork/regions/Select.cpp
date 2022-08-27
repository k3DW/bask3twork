#include "pch.h"
#include "regions/Select.h"
#include "Constants.h"
#include "MainWindow.h"

SelectRegion::SelectRegion(MainWindow* parent, int h, int w)
	: wxStaticBoxSizer(wxVERTICAL, parent, "Select")
	, selection{ .min{ 0, 0 }, .max{ h - 1, w - 1 } }
{
	toggle_button = new wxButton(parent, wxID_ANY, "Show", wxDefaultPosition, wxSize(65,23));
	toggle_button->Bind(wxEVT_BUTTON, &MainWindow::toggle_selection, parent);

	reset_button = new wxButton(parent, wxID_ANY, "Reset", wxDefaultPosition, wxSize(65,23));
	reset_button->Bind(wxEVT_BUTTON, &MainWindow::reset_selection, parent);

	button_sizer = new wxBoxSizer(wxHORIZONTAL);
	button_sizer->Add(toggle_button);
	button_sizer->Add(reset_button);


	display = new wxStaticText(parent, wxID_ANY, "");
	display->SetFont(Fonts::select);
	update_display();

	Add(display, 0, wxALIGN_CENTER | wxDOWN, GAP_3);
	Add(button_sizer, 0, wxEXPAND);
	Layout();
}

void SelectRegion::update_display()
{
	/// Updates the displayed selection coordinates.
	/// Each of the stored values is zero-indexed, so the displayed value is incremented by 1.
	display->SetLabelText(wxString::Format("(%i,%i) to (%i,%i)", selection.min.i + 1, selection.min.j + 1, selection.max.i + 1, selection.max.j + 1));
	Layout();
}

void SelectRegion::set_min(Point point)
{
	selection.min = point;
}
void SelectRegion::set_max(Point point)
{
	selection.max = point;
}

void SelectRegion::normalize()
{
	if (selection.min.i > selection.max.i)
		std::swap(selection.min.i, selection.max.i);
	if (selection.min.j > selection.max.j)
		std::swap(selection.min.j, selection.max.j);

	update_display();
}

void SelectRegion::set_toggle_show()
{
	toggle_button->SetLabelText("Show");
}
void SelectRegion::set_toggle_hide()
{
	toggle_button->SetLabelText("Hide");
}

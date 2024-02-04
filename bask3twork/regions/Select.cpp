#include "pch.h"
#include "pure/GridSize.h"
#include "regions/Select.h"
#include "Constants.h"
#include "MainWindow.h"

SelectRegion::SelectRegion(MainWindow* parent, GridSize size)
	: wxStaticBoxSizer(wxVERTICAL, parent, "Select")
	, selection{ .min{ 0, 0 }, .max{ size.rows - 1, size.columns - 1 } }
{
	toggle_button = new wxButton(parent, wxID_ANY, "Show", wxDefaultPosition, Sizes::button);
	toggle_button->Bind(wxEVT_BUTTON, &MainWindow::toggle_selection, parent);

	reset_button = new wxButton(parent, wxID_ANY, "Reset", wxDefaultPosition, Sizes::button);
	reset_button->Bind(wxEVT_BUTTON, &MainWindow::reset_selection, parent);

	selecting_button_sizer = new wxBoxSizer(wxHORIZONTAL);
	selecting_button_sizer->Add(toggle_button);
	selecting_button_sizer->Add(reset_button);


	lock_button = new wxButton(parent, wxID_ANY, "Lock", wxDefaultPosition, Sizes::button);
	lock_button->Bind(wxEVT_BUTTON, &MainWindow::lock_selection, parent);
	lock_button->Disable();

	unlock_button = new wxButton(parent, wxID_ANY, "Unlock", wxDefaultPosition, Sizes::button);
	unlock_button->Bind(wxEVT_BUTTON, &MainWindow::unlock_selection, parent);
	unlock_button->Disable();

	locking_button_sizer = new wxBoxSizer(wxHORIZONTAL);
	locking_button_sizer->Add(lock_button);
	locking_button_sizer->Add(unlock_button);

	invert_locking_button = new wxButton(parent, wxID_ANY, "Invert locking");
	invert_locking_button->Bind(wxEVT_BUTTON, &MainWindow::invert_locking, parent);
	invert_locking_button->Disable();


	display = new wxStaticText(parent, wxID_ANY, "");
	display->SetFont(Fonts::select);
	update_display();

	Add(display, 0, wxALIGN_CENTER | wxDOWN, Borders::sub_region);
	Add(selecting_button_sizer, 0, wxEXPAND | wxDOWN, Borders::sub_region);
	Add(locking_button_sizer, 0, wxEXPAND);
	Add(invert_locking_button, 0, wxEXPAND);
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

void SelectRegion::disable_lock_buttons()
{
	lock_button->Disable();
	unlock_button->Disable();
	invert_locking_button->Disable();
}
void SelectRegion::enable_lock_buttons()
{
	lock_button->Enable();
	unlock_button->Enable();
	invert_locking_button->Enable();
}

#include "pch.h"
#include "pure/GridSize.h"
#include "regions/Locking.h"
#include "Constants.h"
#include "MainWindow.h"

LockingRegion::LockingRegion(MainWindow* parent)
	: wxStaticBoxSizer(wxVERTICAL, parent, "Locking")
{
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

	Add(locking_button_sizer, 0, wxEXPAND);
	Add(invert_locking_button, 0, wxEXPAND);
	Layout();
}

void LockingRegion::disable_buttons()
{
	lock_button->Disable();
	unlock_button->Disable();
	invert_locking_button->Disable();
}

void LockingRegion::enable_buttons()
{
	lock_button->Enable();
	unlock_button->Enable();
	invert_locking_button->Enable();
}

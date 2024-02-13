#pragma once
#include <wx/sizer.h>
#include "Forward.h"
#include "pure/Selection.h"

class LockingRegion : public wxStaticBoxSizer
{
public:
	LockingRegion(MainWindow* parent);

	void enable_buttons();
	void disable_buttons();

private:
	wxButton*   lock_button;
	wxButton*   unlock_button;
	wxBoxSizer* locking_button_sizer;

	wxButton*   invert_locking_button;
};

#pragma once
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include "Forward.h"

class ExportDialog : public wxDialog
{
public:
	ExportDialog(const Knot* knot);

private:
	wxTextCtrl* textbox;

	wxButton* copy_button;
	void copy(); ///< This function copies the current text data in the textbox into the clipboard, saving it after closing the program.
	void copy(wxCommandEvent& evt);

	void on_key_press(wxKeyEvent& event);
};

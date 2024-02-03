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
	void copy(wxCommandEvent& evt); ///< This function copies the current text data in the textbox into the clipboard, saving it after closing the program.

	void on_exit(wxKeyEvent& event); ///< This function provides a means of closing the dialog by pressing esc
};

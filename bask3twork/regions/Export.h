#pragma once
#include "wx/wx.h"
#include "Forward.h"

class ExportRegion : public wxStaticBoxSizer
{
public:
	ExportRegion(MainWindow* parent, int h, int w);

	void regenerate(MainWindow* parent, int h, int w); ///< Define the export textbox with a size dependent on the height and width of the knot
	void display(const Knot* knot);                    ///< Output the contents of the knot into the export textbox

private:
	wxTextCtrl* textbox;

	wxButton* copy_button;
	void copy(wxCommandEvent& evt); ///< This function copies the current text data in the textbox into the clipboard, saving it after closing the program.
};

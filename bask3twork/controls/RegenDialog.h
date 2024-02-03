#pragma once
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include "Forward.h"

class RegenDialog : public wxDialog
{
public:
	RegenDialog(MainWindow* parent, GridSize size);

	std::optional<GridSize> get_size() const;

private:
	wxBoxSizer* textbox_sizer;
	RegenDialogTextBox* height_box;
	RegenDialogTextBox* width_box;

	wxBoxSizer* main_sizer;
	wxButton* button;

	void on_exit(wxKeyEvent& event); ///< This function provides a means of closing the dialog by pressing esc
};

class RegenDialogTextBox : public wxTextCtrl
{
private:
	friend class RegenDialog;

	RegenDialogTextBox(RegenDialog* parent, int default_value);

	template <size_t maximum>
	std::optional<int> get_value() const;
};

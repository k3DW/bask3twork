#pragma once
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include "Forward.h"

class RegenDialog : public wxDialog
{
public:
	RegenDialog(MainWindow* parent, int h, int w);

	std::pair<int, int> get_values() const;

private:
	wxBoxSizer* textbox_sizer;
	RegenDialogTextBox* height_box;
	RegenDialogTextBox* width_box;

	wxBoxSizer* main_sizer;
	wxButton* button;
};

class RegenDialogTextBox : public wxTextCtrl
{
private:
	friend class RegenDialog;

	RegenDialogTextBox(RegenDialog* parent, int default_value);

	template <int maximum>
	int get_value() const;
};

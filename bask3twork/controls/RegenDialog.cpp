#include "controls/RegenDialog.h"
#include "Constants.h"
#include "MainWindow.h"

RegenDialog::RegenDialog(MainWindow* parent, int h, int w)
	: wxDialog(nullptr, wxID_ANY, "Grid")
	, textbox_sizer(new wxBoxSizer(wxHORIZONTAL))
	, height_box(new RegenDialogTextBox(this, h))
	, width_box(new RegenDialogTextBox(this, w))
	, main_sizer(new wxBoxSizer(wxVERTICAL))
	, button(new wxButton(this, wxID_ANY, "Regenerate"))
{
	SetIcon(wxIcon(L"AppIcon"));

	textbox_sizer->Add(height_box, 0, wxEXPAND);
	textbox_sizer->Add(new wxStaticText(this, wxID_ANY, " by "), 0, wxALIGN_CENTER);
	textbox_sizer->Add(width_box, 0, wxEXPAND);

	main_sizer->Add(textbox_sizer, 0, wxEXPAND | wxALL, GAP_3);
	main_sizer->Add(button, 0, wxEXPAND | (wxALL ^ wxUP), GAP_3);
	SetSizer(main_sizer);

	SetMinSize(wxDefaultSize);
	SetMinSize(GetBestSize());
	SetSize(GetBestSize());
}

std::pair<int, int> RegenDialog::get_values() const
{
	static constexpr std::pair<int, int> default_pair{ -1, -1 };

	int height = height_box->get_value<MAX_H>();
	if (height == -1)
		return default_pair;
	
	int width = width_box->get_value<MAX_W>();
	if (width == -1)
		return default_pair;

	return { height, width };
}



RegenDialogTextBox::RegenDialogTextBox(RegenDialog* parent, int default_value)
	: wxTextCtrl(parent, wxID_ANY, wxString::Format("%i", default_value), wxDefaultPosition, wxSize(42, 24), wxTE_CENTER)
{
	SetMaxLength(2);
	SetFont(Fonts::regenerate);
}

template <int maximum>
int RegenDialogTextBox::get_value() const
{
	wxString string = GetValue();

	int number;
	if (string.ToInt(&number) == false)
	{
		wxMessageBox("You can only enter whole numbers for the new grid size.", "Error: Non-integer grid size");
		return -1;
	}

	else if (number < 1)
	{
		wxMessageBox("You can only enter positive numbers for the new grid size.", "Error: Non-positive grid size");
		return -1;
	}

	return number;
}


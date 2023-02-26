#include "pch.h"
#include "Constants.h"
#include "MainWindow.h"
#include "controls/RegenDialog.h"
#include "pure/GridSize.h"
#include "pure/Selection.h"

RegenDialog::RegenDialog(MainWindow* parent, GridSize size)
	: wxDialog(nullptr, wxID_ANY, "Grid")
	, textbox_sizer(new wxBoxSizer(wxHORIZONTAL))
	, height_box(new RegenDialogTextBox(this, size.rows))
	, width_box(new RegenDialogTextBox(this, size.columns))
	, main_sizer(new wxBoxSizer(wxVERTICAL))
	, button(new wxButton(this, wxID_ANY, "Regenerate"))
{
	SetIcon(wxIcon(L"AppIcon"));

	textbox_sizer->Add(height_box, 0, wxEXPAND);
	textbox_sizer->Add(new wxStaticText(this, wxID_ANY, " by "), 0, wxALIGN_CENTER);
	textbox_sizer->Add(width_box, 0, wxEXPAND);

	main_sizer->Add(textbox_sizer, 0, wxEXPAND | wxALL, Borders::sub_region);
	main_sizer->Add(button, 0, wxEXPAND | (wxALL ^ wxUP), Borders::sub_region);
	SetSizer(main_sizer);

	SetMinSize(wxDefaultSize);
	SetMinSize(GetBestSize());
	SetSize(GetBestSize());
}

std::optional<GridSize> RegenDialog::get_size() const
{
	auto height = height_box->get_value<Limits::rows>();
	if (not height)
		return std::nullopt;
	
	auto width = width_box->get_value<Limits::columns>();
	if (not width)
		return std::nullopt;

	return GridSize{ .rows = *height, .columns = *width };
}



RegenDialogTextBox::RegenDialogTextBox(RegenDialog* parent, int default_value)
	: wxTextCtrl(parent, wxID_ANY, wxString::Format("%i", default_value), wxDefaultPosition, wxSize(42, 24), wxTE_CENTER)
{
	SetMaxLength(2);
	SetFont(Fonts::regenerate);
}

template <size_t maximum>
std::optional<int> RegenDialogTextBox::get_value() const
{
	wxString string = GetValue();

	int number;
	if (string.ToInt(&number) == false)
	{
		wxMessageBox("You can only enter whole numbers for the new grid size.", "Error: Non-integer grid size");
		return std::nullopt;
	}

	else if (number < 1)
	{
		wxMessageBox("You can only enter positive numbers for the new grid size.", "Error: Non-positive grid size");
		return std::nullopt;
	}

	return number;
}


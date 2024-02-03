#include "pch.h"
#include "controls/ExportDialog.h"
#include "grid/Knot.h"
#include "Constants.h"

ExportDialog::ExportDialog(const Knot* knot)
	: wxDialog(nullptr, wxID_ANY, "Export")
	, sizer(new wxBoxSizer(wxVERTICAL))
{
	textbox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(9 * knot->size.columns + 14, 19 * knot->size.rows + 7), wxTE_MULTILINE | wxTE_NO_VSCROLL | wxTE_READONLY);
	textbox->SetFont(Fonts::export_);
	sizer->Add(textbox, 0, wxALIGN_CENTER | wxUP | wxDOWN, Borders::sub_region);

	copy_button = new wxButton(this, wxID_ANY, "Copy", wxDefaultPosition, wxSize(54, 23));
	copy_button->Bind(wxEVT_BUTTON, &ExportDialog::copy, this);
	sizer->Add(copy_button, 0, wxEXPAND);

	textbox->SetLabel(knot->plaintext());
	copy_button->SetFocus();

	SetSizer(sizer);

	SetMinSize(wxDefaultSize);
	SetMinSize(GetBestSize());
	SetSize(GetBestSize());
}

void ExportDialog::copy(wxCommandEvent& evt)
{
	// Open the clipboard
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(textbox->GetLabel())); // Copy the knot into the clipboard
		wxTheClipboard->Flush();                                            // Keep the knot in the clipboard after closing the program
		wxTheClipboard->Close();                                            // Close the clipboard
	}
	evt.Skip();
}

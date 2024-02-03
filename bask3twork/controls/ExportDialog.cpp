#include "pch.h"
#include "controls/ExportDialog.h"
#include "grid/Knot.h"
#include "Constants.h"

ExportDialog::ExportDialog(const Knot* knot)
	: wxDialog(nullptr, wxID_ANY, "Export")
{
	SetIcon(wxIcon(L"AppIcon"));
	Bind(wxEVT_CHAR_HOOK, &ExportDialog::on_exit, this);

	wxBoxSizer* inner_sizer = new wxBoxSizer(wxVERTICAL);

	textbox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(9 * knot->size.columns + 14, 19 * knot->size.rows + 7), wxTE_MULTILINE | wxTE_NO_VSCROLL | wxTE_READONLY);
	textbox->SetFont(Fonts::export_);
	inner_sizer->Add(textbox, 0, wxALIGN_CENTER | wxDOWN, Borders::sub_region);

	copy_button = new wxButton(this, wxID_ANY, "Copy", wxDefaultPosition, Sizes::button);
	copy_button->Bind(wxEVT_BUTTON, &ExportDialog::copy, this);
	inner_sizer->Add(copy_button, 0, wxALIGN_CENTER);

	textbox->SetLabel(knot->plaintext());
	copy_button->SetFocus();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(inner_sizer, 0, wxALL, Borders::inter_region);
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

void ExportDialog::on_exit(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_ESCAPE)
	{
		EndModal(0);
	}
	event.Skip();
}

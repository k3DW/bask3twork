#include "pch.h"
#include "pure/GridSize.h"
#include "regions/Export.h"
#include "grid/Knot.h"
#include "Constants.h"
#include "MainWindow.h"

ExportRegion::ExportRegion(MainWindow* parent, GridSize size)
	: wxStaticBoxSizer(wxVERTICAL, parent, "Export")
{
	regenerate(parent, size);
	copy_button = new wxButton(parent, wxID_ANY, "Copy", wxDefaultPosition, wxSize(54, 23));
	copy_button->Bind(wxEVT_BUTTON, &ExportRegion::copy, this);
	Add(copy_button, 0, wxEXPAND);
}

void ExportRegion::regenerate(MainWindow* parent, GridSize size)
{
	if (textbox)
		textbox->Destroy();
	textbox = new wxTextCtrl(parent, wxID_ANY, "", wxDefaultPosition, wxSize(9 * size.columns + 14, 19 * size.rows + 7), wxTE_MULTILINE | wxTE_NO_VSCROLL | wxTE_READONLY);
	textbox->SetFont(Fonts::export_);
	Prepend(textbox, 0, wxALIGN_CENTER | wxUP | wxDOWN, Borders::sub_region);
}

void ExportRegion::display(const Knot* knot)
{
	textbox->SetLabel(knot->plaintext());
}

void ExportRegion::copy(wxCommandEvent& evt)
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

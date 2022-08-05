#include "MainWindow.h"
#include "DisplayGrid.h"
#include "Knot.h"

#include "SelectRegion.h"
#include "GenerateRegion.h"
#include "ExportRegion.h"
#include "MenuBar.h"
#include "RegenDialog.h"

MainWindow::MainWindow(int h, int w, wxString title)
	: wxFrame(nullptr, wxID_ANY, title), h(h), w(w)
	, select_region(new SelectRegion(this, h, w))
	, generate_region(new GenerateRegion(this))
	, export_region(new ExportRegion(this, h, w))
	, menu_bar(new MenuBar(this))
{
	CreateStatusBar();
	SetBackgroundColour(Colours::background);
	initSizerLayout();
	RefreshMinSize();
}
MainWindow::~MainWindow() {
	Hide();
}
void MainWindow::initSizerLayout() {
	initDispSizer();

	buttonSizer = new wxBoxSizer(wxVERTICAL);
	buttonSizer->AddStretchSpacer();
	buttonSizer->Add(select_region, 0, wxDOWN, GAP_2);
	buttonSizer->Add(generate_region, 0, wxDOWN, GAP_2);
	buttonSizer->Add(export_region);
	buttonSizer->AddStretchSpacer();

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(dispSizer, 0, wxEXPAND | wxALL, GAP_1);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(buttonSizer, 0, wxEXPAND | (wxALL ^ wxLEFT), GAP_1);
	SetSizer(mainSizer);
}
void MainWindow::initDispSizer() {
	/// \b Method

	/// First, check to see if the Knot has been initialized already.
	/// If it has not, then neither has the DisplayGrid or its sizer, so the sizer is initialized with 2 stretch spacers.
	/// It the Knot has been initialized, then \c delete the Knot and \c Destroy() the DisplayGrid.
	if (!knot) { 
		dispSizer = new wxBoxSizer(wxVERTICAL);
		dispSizer->AddStretchSpacer();
		dispSizer->AddStretchSpacer();
	}
	else {
		delete knot;
		disp->Destroy();
	}

	/// Regardless of the above, initialize the Knot with the member variables \c h and \c w and the status bar.
	/// Then initialize the DisplayGrid with the newly generated Knot, and insert it between the stretch spacers in its sizer.
	knot = new Knot(h, w, GetStatusBar());
	disp = new DisplayGrid(this, h, w);
	dispSizer->Insert(1, disp, 0, wxEXPAND);
}

void MainWindow::show_selection()
{
	select_region->normalize();
	select_region->set_toggle_hide();
	disp->highlight(select_region->get_selection());
	generate_region->enable_buttons(current_symmetry());
	showing_selection = true;
}
void MainWindow::hide_selection()
{
	select_region->set_toggle_show();
	disp->unhighlight(true);
	generate_region->disable_buttons();
	showing_selection = false;
}
void MainWindow::toggle_selection(wxCommandEvent& evt)
{
	if (showing_selection)
		hide_selection();
	else
		show_selection();

	evt.Skip();
}
void MainWindow::reset_selection()
{
	select_region->set_min({ 0, 0 });
	select_region->set_max({ h - 1, w - 1 });
	select_region->update_display();
	hide_selection();
}
void MainWindow::reset_selection(wxCommandEvent& evt)
{
	reset_selection();
	evt.Skip();
}

void MainWindow::left_click_tile(wxMouseEvent& evt)
{
	wxWindowID id = evt.GetId();
	select_region->set_min({ id / w, id % w });
	select_region->update_display();
	hide_selection();
	evt.Skip();
}
void MainWindow::right_click_tile(wxMouseEvent& evt)
{
	wxWindowID id = evt.GetId();
	select_region->set_max({ id / w, id % w });
	select_region->update_display();
	hide_selection();
	evt.Skip();
}

void MainWindow::menu_event_handler(wxCommandEvent& evt)
{
	std::invoke(MenuBar::functions[evt.GetId()], this);
	evt.Skip();
}

void MainWindow::openFile() {
	// Open a wxFileDialog to get the name of the file.
	wxFileDialog openFileDialog(this, "Open Knot file", "", "", "k3DW Knot Files (*.k3knot)|*.k3knot|Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	// If the wxFileDialog gets closed, stop the function.
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	// Uses a wxTextFile to read from this file; could also use wxFileInputStream.
	wxTextFile file(openFileDialog.GetPath());
	file.Open();

	// Check the row count, must be 1 <= rows <= MAX_H
	// Get the column count of the first row, must be 1 <= columns <= MAX_W
	const size_t rowCount = file.GetLineCount();
	const size_t colCount = file.GetFirstLine().size();
	if (rowCount > MAX_H || colCount > MAX_W) {
		wxMessageBox(wxString::Format("Please choose a smaller file. The file can only be %i rows by %i columns.", MAX_H, MAX_W), "Error: File is too large");
		return;
	}
	if (rowCount < 1) {
		wxMessageBox("Please choose a non-empty file.", "Error: File is empty");
		return;
	}
	if (colCount < 1) {
		wxMessageBox("Please choose a file with non-empty rows.", "Error: First row is empty");
		return;
	}

	// Make a 2D vector of Glyphs, to be later written to the new Knot
	std::vector<std::vector<const Glyph*>> glyphs;
	glyphs.reserve(rowCount);

	// For each line in the file
	for (wxString line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) {
		// If the width has a different number of columns than it should, output an error
		if (line.size() != colCount) {
			wxMessageBox("Please choose a valid knot file. The number of characters in every row must be equal.", "Error: File has jagged rows");
			return;
		}

		// Make a 1D vector of Glyphs, to be written to the 2D vector
		std::vector<const Glyph*> glyphRow;
		glyphRow.reserve(colCount);

		// Take each character from the line, and push the corresponding Glyph to the vector
		for (wxUniChar c : line)
			glyphRow.push_back(UnicharToGlyph.at(c));
		glyphs.push_back(glyphRow);
	}

	// Delete the current knot and destroy the DisplayGrid.
	delete knot;
	disp->Destroy();

	// Set the height and width, then set the grid regen textbox values.
	h = glyphs.size();
	w = glyphs[0].size();

	// Next, initialize the Knot with the variable \c glyphs and the status bar.
	// Initialize the DisplayGrid with the newly generated Knot, and insert it between the stretch spacers in its sizer.
	knot = new Knot(std::move(glyphs), GetStatusBar());
	disp = new DisplayGrid(this, h, w);
	dispSizer->Insert(1, disp, 0, wxEXPAND);

	// Then, reset the select coordinates with MainWindow::reset_selection()
	// and regenerate and export textbox.
	reset_selection();
	export_region->regenerate(this, h, w);
	export_region->display(knot);

	// Reset the wrapping checkboxes
	menu_bar->reset_wrapping();

	// Lastly, refresh the minimum size of the window.
	RefreshMinSize();
	
	file.Close();
}
void MainWindow::saveFile() {
	// Open a wxFileDialog to get the name of the file.
	wxFileDialog saveFileDialog(this, "Save Knot file", "", "", "k3DW Knot Files (*.k3knot)|*.k3knot|Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);

	// If the wxFileDialog gets closed, stop the function.
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	// Uses a wxTextFile to write to this file; could also use wxFileOutputStream
	wxTextFile file(saveFileDialog.GetPath());

	// Clear the file if it exists, or create a file if it doesn't exist
	if (file.Exists()) {
		file.Open();
		file.Clear();
	}
	else file.Create();

	// Write the knot to the file, line by line
	for (int i = 0; i < knot->h; i++) {
		wxString line = "";
		for (int j = 0; j < knot->w; j++)
			line << knot->get(i, j);
		file.AddLine(line);
	}

	// Save changes, then close
	file.Write();
	file.Close();
}
void MainWindow::update_wrap_x()
{
	knot->wrapXEnabled = menu_bar->is_wrap_x();
	if (showing_selection)
		generate_region->enable_buttons(current_symmetry());
}
void MainWindow::update_wrap_y()
{
	knot->wrapYEnabled = menu_bar->is_wrap_y();
	if (showing_selection)
		generate_region->enable_buttons(current_symmetry());
}

auto MainWindow::get_regen_dialog_handler(RegenDialog* regen_dialog)
{
	return [this, regen_dialog](wxCommandEvent& evt)
	{
		auto [new_h, new_w] = regen_dialog->get_values();
		if (new_h == -1 || new_w == -1)
			return;

		h = new_h;
		w = new_w;

		initDispSizer();

		// / Then, reset the select coordinates with MainWindow::reset_selection(),
		// / regenerate and export textbox,
		// / and reset the knot wrapping \c wxMenuItem objects.
		reset_selection();
		export_region->regenerate(this, h, w);
		menu_bar->reset_wrapping();

		// / Lastly, refresh the minimum size of the window.
		RefreshMinSize();

		regen_dialog->EndModal(0);
		evt.Skip();
	};
}

void MainWindow::regenerate_grid()
{
	RegenDialog* regen_dialog = new RegenDialog(this, h, w);
	regen_dialog->Bind(wxEVT_BUTTON, get_regen_dialog_handler(regen_dialog));

	regen_dialog->ShowModal();
	regen_dialog->Destroy();
}

void MainWindow::RefreshMinSize() {
	/// \b Method

	/// Change the size and minimum size of the window to fit the content.
	/// To do this, the minimum size must first be set to \c wxDefaultSize, before finding the new size with \c GetBestSize().
	/// The minimum size is set to this new size, but the current size is only changed if the window is not maixmized.
	SetMinSize(wxDefaultSize);
	wxSize newSize = GetBestSize();
	SetMinSize(newSize);
	if (!IsMaximized())
		SetSize(newSize);
	Layout();
}

Symmetry MainWindow::current_symmetry() const
{
	return knot->symmetry_of(select_region->get_selection()) * knot->checkWrapping(select_region->get_selection());
}

void MainWindow::generateKnot(wxCommandEvent& evt) {
	/// The Knot::generate() function uses the status bar, so first store the current displayed message.
	const wxString oldStatus = GetStatusBar()->GetStatusText();

	/// Each of the generating buttons has its symmetry type as its ID value. Get this symmetry from the event call.
	/// Then, forward on this symmetry to Knot::generate() with the selection coordinates to be generated.
	/// If this function returns \c true, then the Knot has been generated successfully,
	/// so update the DisplayGrid with DisplayGrid::draw() and update the export text box with MainWindow::showExportBox().
	/// If the generate function returns \c false, then display an error message as a \c wxMessageBox.
	Symmetry sym = static_cast<Symmetry>(evt.GetId());
	if (knot->generate(sym, select_region->get_selection())) {
		disp->draw(knot);
		export_region->display(knot);
	}
	else
		wxMessageBox(wxString::Format("The specified knot was not able to be generated in %i attempts.", MAX_ATTEMPTS), "Error: Knot failed");

	/// At the end, set the status bar back to the message which was displayed at the beginning of the function,
	/// and re-enable the generate buttons.
	GetStatusBar()->SetStatusText(oldStatus);
	evt.Skip();
}

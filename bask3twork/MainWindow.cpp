#include "pch.h"
#include "Constants.h"
#include "MainWindow.h"
#include "grid/Display.h"
#include "grid/Knot.h"
#include "pure/Enum.h"
#include "pure/Glyph.h"
#include "pure/GridSize.h"
#include "pure/Symmetry.h"
#include "regions/Select.h"
#include "regions/Generate.h"
#include "regions/Export.h"
#include "controls/MenuBar.h"
#include "controls/RegenDialog.h"

MainWindow::MainWindow(GridSize size, wxString title)
	: wxFrame(nullptr, wxID_ANY, title)
	, size(size)

	, select_region(new SelectRegion(this, size))
	, showing_selection(false)
	, generate_region(new GenerateRegion(this))
	, export_region(new ExportRegion(this, size))
	, region_sizer(make_region_sizer(select_region, generate_region, export_region))

	, menu_bar(new MenuBar(this))

	, disp(new DisplayGrid(this, size))
	, knot(new Knot(size, GetStatusBar())) // Apparently you can call GetStatusBar() before CreateStatusBar()
	, grid_sizer(make_grid_sizer(disp))

	, main_sizer(new MainSizer(grid_sizer, region_sizer))
{
	CreateStatusBar();
	SetBackgroundColour(Colours::background);
	SetSizer(main_sizer);
	refresh_min_size();
}
MainWindow::~MainWindow()
{
	Hide();
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
	select_region->set_max({ size.rows - 1, size.columns - 1 });
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
	select_region->set_min({ id / size.columns, id % size.columns });
	select_region->update_display();
	hide_selection();
	evt.Skip();
}
void MainWindow::right_click_tile(wxMouseEvent& evt)
{
	wxWindowID id = evt.GetId();
	select_region->set_max({ id / size.columns, id % size.columns });
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
	if (rowCount > Limits::rows || colCount > Limits::columns) {
		wxMessageBox(wxString::Format("Please choose a smaller file. The file can only be %i rows by %i columns.", Limits::rows, Limits::columns), "Error: File is too large");
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
	size = { .rows = (int)glyphs.size(), .columns = (int)glyphs[0].size() };

	// Next, initialize the Knot with the variable \c glyphs and the status bar.
	// Initialize the DisplayGrid with the newly generated Knot, and insert it between the stretch spacers in its sizer.
	knot = new Knot(std::move(glyphs), GetStatusBar());
	disp = new DisplayGrid(this, size);
	disp->draw(knot);
	grid_sizer->Insert(1, disp, 0, wxEXPAND);

	// Then, reset the select coordinates with MainWindow::reset_selection()
	// and regenerate and export textbox.
	reset_selection();
	export_region->regenerate(this, size);
	export_region->display(knot);

	// Reset the wrapping checkboxes
	menu_bar->reset_wrapping();

	// Lastly, refresh the minimum size of the window.
	refresh_min_size();
	
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
	for (int i = 0; i < size.rows; i++) {
		wxString line = "";
		for (int j = 0; j < size.columns; j++)
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
		std::optional<GridSize> opt_size = regen_dialog->get_size();
		if (not opt_size)
			return;

		size = *opt_size;

		delete knot;
		knot = new Knot(size, GetStatusBar());

		disp->Destroy();
		disp = new DisplayGrid(this, size);
		grid_sizer->Insert(1, disp, 0, wxEXPAND);

		// / Then, reset the select coordinates with MainWindow::reset_selection(),
		// / regenerate and export textbox,
		// / and reset the knot wrapping \c wxMenuItem objects.
		reset_selection();
		export_region->regenerate(this, size);
		menu_bar->reset_wrapping();

		// / Lastly, refresh the minimum size of the window.
		refresh_min_size();

		regen_dialog->EndModal(0);
		evt.Skip();
	};
}

void MainWindow::regenerate_grid()
{
	RegenDialog* regen_dialog = new RegenDialog(this, size);
	regen_dialog->Bind(wxEVT_BUTTON, get_regen_dialog_handler(regen_dialog));

	regen_dialog->ShowModal();
	regen_dialog->Destroy();
}

void MainWindow::refresh_min_size()
{
	// To change the minimum size of the window to fit the content,
	// the minimum size must first be set to \c wxDefaultSize, before finding the new size with \c GetBestSize().
	SetMinSize(wxDefaultSize);
	const wxSize newSize = GetBestSize();
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



wxBoxSizer* MainWindow::make_region_sizer(SelectRegion* select_region, GenerateRegion* generate_region, ExportRegion* export_region)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddStretchSpacer();
	sizer->Add(select_region);
	sizer->AddSpacer(Borders::inter_region);
	sizer->Add(generate_region);
	sizer->AddSpacer(Borders::inter_region);
	sizer->Add(export_region);
	sizer->AddStretchSpacer();
	return sizer;
}

wxBoxSizer* MainWindow::make_grid_sizer(DisplayGrid* display)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddStretchSpacer();
	sizer->Add(display);
	sizer->AddStretchSpacer();
	return sizer;
}



MainSizer::MainSizer(wxBoxSizer* grid_sizer, wxBoxSizer* region_sizer)
	: wxBoxSizer(wxHORIZONTAL)
{
	AddStretchSpacer();
	Add(grid_sizer, 0, wxEXPAND | wxALL, Borders::outside);
	AddStretchSpacer();
	Add(region_sizer, 0, wxEXPAND | (wxALL ^ wxLEFT), Borders::outside);
}

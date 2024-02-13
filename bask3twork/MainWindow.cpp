#include "pch.h"
#include "Constants.h"
#include "File.h"
#include "MainWindow.h"
#include "grid/Display.h"
#include "grid/Knot.h"
#include "grid/Tile.h"
#include "pure/Glyph.h"
#include "pure/GridSize.h"
#include "pure/Symmetry.h"
#include "pure/UsableEnum.h"
#include "regions/Generate.h"
#include "regions/Locking.h"
#include "controls/ExportDialog.h"
#include "controls/MenuBar.h"
#include "controls/RegenDialog.h"

MainWindow::MainWindow(GridSize size, wxString title)
	: wxFrame(nullptr, wxID_ANY, title)
	, size(size)

	, buttons_enabled(false)
	, locking_region(new LockingRegion(this))
	, generate_region(new GenerateRegion(this))
	, region_sizer(make_region_sizer(locking_region, generate_region))

	, menu_bar(new MenuBar(this))

	, disp(new DisplayGrid(this, size))
	, knot(new Knot(size, CreateStatusBar()))
	, grid_sizer(make_grid_sizer(disp))

	, main_sizer(make_main_sizer(grid_sizer, region_sizer))
{
	Bind(wxEVT_CHAR_HOOK, &MainWindow::on_key_press, this);
	SetBackgroundColour(Colours::background);
	SetSizer(main_sizer);
	update_sizing();
}
MainWindow::~MainWindow()
{
	Hide();
}

void MainWindow::lock_selection(wxCommandEvent& evt)
{
	disp->lock();
	generate_region->enable_buttons(current_symmetry());
	evt.Skip();
}

void MainWindow::unlock_selection(wxCommandEvent& evt)
{
	disp->unlock();
	generate_region->enable_buttons(current_symmetry());
	evt.Skip();
}

void MainWindow::invert_locking(wxCommandEvent& evt)
{
	disp->invert_locking();
	generate_region->enable_buttons(current_symmetry());
	evt.Skip();
}

void MainWindow::enable_buttons()
{
	buttons_enabled = true;
	locking_region->enable_buttons();
	generate_region->enable_buttons(current_symmetry());
}

void MainWindow::disable_buttons()
{
	buttons_enabled = false;
	locking_region->disable_buttons();
	generate_region->disable_buttons();
}

void MainWindow::menu_event_handler(wxCommandEvent& evt)
{
	std::invoke(MenuBar::functions[evt.GetId()], this);
	evt.Skip();
}

void MainWindow::open_file()
{
	// Open a wxFileDialog to get the name of the file.
	wxFileDialog dialog(this, "Open Knot file", "", "", File::ext, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	// If the wxFileDialog gets closed, stop the function.
	if (dialog.ShowModal() == wxID_CANCEL)
		return;

	auto opt = File::read(dialog.GetPath());
	if (!opt) // The function validates, and sends error messages with a messagebox, so no need for a message here
		return;

	auto& [new_size, glyphs, locking] = *opt;

	size = new_size;

	// Knot section
	{
		delete knot;
		knot = new Knot(std::move(glyphs), GetStatusBar());
	}

	// DisplayGrid and Tile section
	{
		disp->resize(size);
		disp->set_knot(knot);
		std::size_t running_index = 0;
		for (int i = 0; i < size.rows; ++i)
		{
			for (int j = 0; j < size.columns; ++j)
			{
				if (locking[running_index++])
					disp->lock_no_render(Point{ i, j });
			}
		}
	}

	menu_bar->reset_wrapping(); // Reset the wrapping checkboxes,
	update_sizing();            // Update the window sizing.
}

void MainWindow::save_file()
{
	// Open a wxFileDialog to get the name of the file.
	wxFileDialog dialog(this, "Save Knot file", "", "", File::ext, wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);

	// If the wxFileDialog gets closed, stop the function.
	if (dialog.ShowModal() == wxID_CANCEL)
		return;

	File::write(dialog.GetPath(), knot, disp);
}

void MainWindow::export_grid()
{
	ExportDialog* export_dialog = new ExportDialog(knot);
	export_dialog->ShowModal();
	export_dialog->Destroy();
}

void MainWindow::update_wrap_x()
{
	knot->wrapXEnabled = menu_bar->is_wrap_x();
	if (buttons_enabled)
		generate_region->enable_buttons(current_symmetry());
}
void MainWindow::update_wrap_y()
{
	knot->wrapYEnabled = menu_bar->is_wrap_y();
	if (buttons_enabled)
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

		disp->resize(size);         // Resize the DisplayGrid,
		menu_bar->reset_wrapping(); // Reset the wrapping checkboxes,
		update_sizing();            // Update the window sizing.

		regen_dialog->EndModal(0);
		evt.Skip();
	};
}

void MainWindow::regenerate_grid()
{
	RegenDialog* regen_dialog = new RegenDialog(size);
	regen_dialog->Bind(wxEVT_BUTTON, get_regen_dialog_handler(regen_dialog));

	regen_dialog->ShowModal();
	regen_dialog->Destroy();
}

void MainWindow::update_min_size()
{
	// To change the minimum size of the window to fit the content,
	// the minimum size must first be set to \c wxDefaultSize, before finding the new size with \c GetBestSize().
	SetMinSize(wxDefaultSize);
	const wxSize newSize = GetBestSize();
	SetMinSize(newSize);
}

void MainWindow::update_sizing()
{
	for (int i = Sizes::glyph_font_pixel.x; i > 0; i -= Fonts::reduce_by)
	{
		disp->set_glyph_font_size(i);
		const wxSize display_size = active_display_size();
		const wxSize best_size = GetBestSize();
		if (best_size.x <= display_size.x && best_size.y <= display_size.y)
			break;
	}

	update_min_size();
	if (!IsMaximized())
		SetSize(GetMinSize());

	Layout();
	Refresh();
}

wxSize MainWindow::active_display_size() const
{
	wxRect rect = wxDisplay(this).GetClientArea();
	return wxSize(rect.width, rect.height);
}

Symmetry MainWindow::current_symmetry() const
{
	if (knot->checkWrapping(disp->get_selection()))
		return knot->symmetry_of(disp->get_selection(), disp->get_tiles());
	else
		return Symmetry::Nothing;
}

void MainWindow::generate_knot(wxCommandEvent& evt)
{
	generate_knot(static_cast<Symmetry>(evt.GetId()));
	evt.Skip();
}

void MainWindow::generate_knot(Symmetry sym)
{
	/// The Knot::generate() function uses the status bar, so first store the current displayed message.
	const wxString oldStatus = GetStatusBar()->GetStatusText();

	/// Each of the generating buttons has its symmetry type as its ID value. Get this symmetry from the event call.
	/// Then, forward on this symmetry to Knot::generate() with the selection coordinates to be generated.
	/// If this function returns \c true, then the Knot has been generated successfully,
	/// so update the DisplayGrid with DisplayGrid::draw() and update the export text box with MainWindow::showExportBox().
	/// If the generate function returns \c false, then display an error message as a \c wxMessageBox.
	
	if (sym == Symmetry::Nothing)
	{
		knot->clear(disp->get_selection(), disp->get_tiles());
		disp->render();
		return;
	}

	if (knot->generate(sym, disp->get_selection(), disp->get_tiles())) {
		disp->set_knot(knot);
		disp->render();
	}
	else
		wxMessageBox(wxString::Format("The specified knot was not able to be generated in %i attempts.", MAX_ATTEMPTS), "Error: Knot failed");

	/// At the end, set the status bar back to the message which was displayed at the beginning of the function,
	/// and re-enable the generate buttons.
	GetStatusBar()->SetStatusText(oldStatus);
}



wxBoxSizer* MainWindow::make_region_sizer(LockingRegion* locking_region, GenerateRegion* generate_region)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddStretchSpacer();
	sizer->Add(locking_region);
	sizer->AddSpacer(Borders::inter_region);
	sizer->Add(generate_region);
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

wxBoxSizer* MainWindow::make_main_sizer(wxBoxSizer* grid_sizer, wxBoxSizer* region_sizer)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddStretchSpacer();
	sizer->Add(grid_sizer, 0, wxEXPAND | wxALL, Borders::outside);
	sizer->AddStretchSpacer();
	sizer->Add(region_sizer, 0, wxEXPAND | (wxALL ^ wxLEFT), Borders::outside);
	return sizer;
}

void MainWindow::on_key_press(wxKeyEvent& event)
{
	const int code = event.GetKeyCode();
	switch (code)
	{

	case WXK_ESCAPE:
		disp->hide_highlight();
		break;

	case WXK_DELETE:
	case '0':
		if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::Nothing))     generate_knot(Symmetry::Nothing); break;

	case '1': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::AnySym))      generate_knot(Symmetry::AnySym); break;
	case '2': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::HoriSym))     generate_knot(Symmetry::HoriSym); break;
	case '3': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::VertSym))     generate_knot(Symmetry::VertSym); break;
	case '4': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::HoriVertSym)) generate_knot(Symmetry::HoriVertSym); break;
	case '5': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::Rot2Sym))     generate_knot(Symmetry::Rot2Sym); break;
	case '6': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::Rot4Sym))     generate_knot(Symmetry::Rot4Sym); break;
	case '7': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::FwdDiag))     generate_knot(Symmetry::FwdDiag); break;
	case '8': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::BackDiag))    generate_knot(Symmetry::BackDiag); break;
	case '9': if (event.GetModifiers() == wxMOD_CONTROL && generate_region->is_enabled(Symmetry::FullSym))     generate_knot(Symmetry::FullSym); break;

	case WXK_NUMPAD0: if (generate_region->is_enabled(Symmetry::Nothing))     generate_knot(Symmetry::Nothing); break;
	case WXK_NUMPAD1: if (generate_region->is_enabled(Symmetry::AnySym))      generate_knot(Symmetry::AnySym); break;
	case WXK_NUMPAD2: if (generate_region->is_enabled(Symmetry::HoriSym))     generate_knot(Symmetry::HoriSym); break;
	case WXK_NUMPAD3: if (generate_region->is_enabled(Symmetry::VertSym))     generate_knot(Symmetry::VertSym); break;
	case WXK_NUMPAD4: if (generate_region->is_enabled(Symmetry::HoriVertSym)) generate_knot(Symmetry::HoriVertSym); break;
	case WXK_NUMPAD5: if (generate_region->is_enabled(Symmetry::Rot2Sym))     generate_knot(Symmetry::Rot2Sym); break;
	case WXK_NUMPAD6: if (generate_region->is_enabled(Symmetry::Rot4Sym))     generate_knot(Symmetry::Rot4Sym); break;
	case WXK_NUMPAD7: if (generate_region->is_enabled(Symmetry::FwdDiag))     generate_knot(Symmetry::FwdDiag); break;
	case WXK_NUMPAD8: if (generate_region->is_enabled(Symmetry::BackDiag))    generate_knot(Symmetry::BackDiag); break;
	case WXK_NUMPAD9: if (generate_region->is_enabled(Symmetry::FullSym))     generate_knot(Symmetry::FullSym); break;

	default:;

	}

	event.Skip();
}

#pragma once
#include <wx/frame.h>
#include <wx/sizer.h>
#include "Forward.h"
#include "pure/GridSize.h"

///< As a more specialized \c wxFrame object, this class represents the main window of the application.
class MainWindow : public wxFrame
{
public:
	MainWindow(GridSize size, wxString title);
	~MainWindow(); ///< Hides this MainWindow object automatically, so the destruction is not visible

	void lock_selection(wxCommandEvent& evt);
	void unlock_selection(wxCommandEvent& evt);
	void invert_locking(wxCommandEvent& evt);

	void enable_buttons();
	void disable_buttons();

	GridSize size;

public:
	bool            buttons_enabled;
	LockingRegion*  locking_region;
	GenerateRegion* generate_region;
	wxBoxSizer*     region_sizer;

	MenuBar* menu_bar;

public:
	void menu_event_handler(wxCommandEvent& evt); ///< Handles all events for menu presses
	
	void open_file();       ///< Opens a \c .k3knot file or a \c .txt file, loading it into the grid
	void save_file();       ///< Saves the current knot as a \c .k3knot file or a \c .txt file
	void export_grid();     ///< Open the "Export" dialog pop-up, giving the user the option to copy to the clipboard
	void update_wrap_x();   ///< Grab the x wrapping from the menu bar, and refresh the buttons
	void update_wrap_y();   ///< Grab the y wrapping from the menu bar, and refresh the buttons
	void regenerate_grid(); ///< Open the "Regenerate" dialog pop-up, and regenerate the grid if successful

	auto get_regen_dialog_handler(RegenDialog* regen_dialog); ///< The function bound to the \c RegenDialog button

private:
	void update_min_size();             ///< Set the minimum size of the window to fit the content
	void update_sizing();               ///< Updates the size of the \c DisplayGrid and its \c Tile children so that this \c MainWindow fits in the current display
	wxSize active_display_size() const; ///< Grabs the screen size of the active display

	DisplayGrid* disp; ///< The DisplayGrid for this program, i.e. the \c wxWindow that displays the Knot.
	Knot* knot;        ///< The Knot object belonging to this program.
	wxBoxSizer* grid_sizer;

	wxBoxSizer* main_sizer;

	Symmetry current_symmetry() const;

public:
	void generate_knot(wxCommandEvent& evt); ///< This function checks which of the generating buttons was pressed and calls the appropriate Knot function.
	void generate_knot(Symmetry sym);        ///< This function checks which of the generating buttons was pressed and calls the appropriate Knot function.

private:
	static wxBoxSizer* make_region_sizer(LockingRegion* locking_region, GenerateRegion* generate_region);
	static wxBoxSizer* make_grid_sizer(DisplayGrid* display);
	static wxBoxSizer* make_main_sizer(wxBoxSizer* grid_sizer, wxBoxSizer* region_sizer);

private:
	void on_key_press(wxKeyEvent& event);
};

/* MainWindow constructor */
/** \fn MainWindow::MainWindow(GridSize size, wxString title)
 * Sets the \c wxFrame base object with the proper title, sets the initial height and width of the grid and the initial selection coordinates.
 *
 * Also creates the status bar, sets the background colour, initializes all WX objects, and sets the window size and minimum size.
 *
 * \param size The initial size of the grid, which can be changed later when MainWindow::gridRegenFunction() is called
 * \param title The title of the whole window
 */

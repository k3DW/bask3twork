#pragma once
#include "Constants.h"
#include "Forward.h"

/** As a more specialized \c wxFrame object, this class represents main window of the application;
	most of the WX object member variables are not documented here. */
class MainWindow : public wxFrame
{
public:
	MainWindow(int h, int w, wxString title);
	~MainWindow(); ///< Hides this MainWindow object automatically, so the destruction is not visible

	void show_selection();
	void hide_selection();
	void toggle_selection(wxCommandEvent& evt);
	void reset_selection();
	void reset_selection(wxCommandEvent& evt);

	void left_click_tile(wxMouseEvent& evt);  ///< Sets the left displayed coordinate, based on which Tile the left click takes place
	void right_click_tile(wxMouseEvent& evt); ///< Sets the right displayed coordinate, based on which Tile the right click takes place

	int h,		///< The height of the knot, i.e. the number of rows.
		w;		///< The width of the knot, i.e. the number of columns.

public:
	SelectRegion*   select_region;
	bool            showing_selection;
	GenerateRegion* generate_region;
	ExportRegion*   export_region;
	RegionSizer*    region_sizer;

	MenuBar* menu_bar;

public:
	void menu_event_handler(wxCommandEvent& evt); ///< Handles all events for menu presses
	
	void openFile();        ///< Opens a \c .k3knot file or a \c .txt file, loading it into the grid
	void saveFile();        ///< Saves the current knot as a \c .k3knot file or a \c .txt file
	void update_wrap_x();   ///< Grab the x wrapping from the menu bar, and refresh the buttons
	void update_wrap_y();   ///< Grab the y wrapping from the menu bar, and refresh the buttons
	void regenerate_grid(); ///< Open the "Regenerate" dialog pop-up, and regenerate the grid if successful

	auto get_regen_dialog_handler(RegenDialog* regen_dialog); ///< The function bound to the \c RegenDialog button

private:
	void initSizerLayout();
	void RefreshMinSize();	///< Sets the minimum size of the window, and sets the size of the window if not maximized.

	DisplayGrid* disp;		///< The DisplayGrid for this program, i.e. the \c wxPanel that displays the Knot.
	Knot* knot;				///< The Knot object belonging to this program.
	GridSizer* grid_sizer;

	wxBoxSizer* mainSizer;

	Symmetry current_symmetry() const;

public:
	void generateKnot(wxCommandEvent& evt);			///< This function checks which of the generating buttons was pressed and calls the appropriate Knot function.
};

/* MainWindow constructor */
/** \fn MainWindow::MainWindow(int h, int w, wxString title)
 * Sets the \c wxFrame base object with the proper title, sets the initial height and width of the grid and the initial selection coordinates.
 *
 * Also creates the status bar, sets the background colour, initializes all WX objects, and sets the window size and minimum size.
 *
 * \param h The initial height of the grid, but can be changed later when MainWindow::gridRegenFunction() is called
 * \param w The initial width of the grid, but can be changed later when MainWindow::gridRegenFunction() is called
 * \param title The title of the whole window
 */

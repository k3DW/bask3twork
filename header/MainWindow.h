#pragma once
#include "Constants.h"
#include "DisplayGrid.h"
#include "Knot.h"
#include "SelectRegion.h"

// Declare and initialize a button with its corresponding function
#define declareButton(buttonName) \
	wxButton* buttonName##Button; \
	void buttonName##Function(wxCommandEvent& evt)

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
	void do_reset();
	void reset_selection(wxCommandEvent& evt);

	void left_click_tile(wxMouseEvent& evt);  ///< Sets the left displayed coordinate, based on which Tile the left click takes place
	void right_click_tile(wxMouseEvent& evt); ///< Sets the right displayed coordinate, based on which Tile the right click takes place

private:
	int h,		///< The height of the knot, i.e. the number of rows.
		w;		///< The width of the knot, i.e. the number of columns.

public:
	SelectRegion* select_region;
	bool showing_selection;

private:
	void initMenuBar();
	void menuEventHandler(wxCommandEvent& evt); ///< Handles all events for menu presses
	wxMenuBar* menuBar;
	wxMenu* menuFile;
		void openFile();	///< Opens a \c .k3knot file or a \c .txt file, loading it into the grid
		void saveFile();	///< Saves the current knot as a \c .k3knot file or a \c .txt file
	wxMenu* menuGenerate;
		wxMenuItem* menuWrapX;
		wxMenuItem* menuWrapY;
		void toggleWrap(bool inXDirection);	///< Toggles the knot wrapping in the direction specified
		void refreshGrid();

	enum class MenuID : int {
		OPEN,
		SAVE,
		WRAP_X,
		WRAP_Y,
		REFRESH_GRID,
	};

	void initSizerLayout();
	void initDispSizer();	///< One of 6 \c init functions which chunk the initializing process, but the only one documented. 
	void RefreshMinSize();	///< Sets the minimum size of the window, and sets the size of the window if not maximized.
	DisplayGrid* disp;		///< The DisplayGrid for this program, i.e. the \c wxPanel that displays the Knot.
	Knot* knot;				///< The Knot object belonging to this program.
	wxBoxSizer* mainSizer;
	wxBoxSizer* dispSizer;
	wxBoxSizer* buttonSizer;

	void initGenerateRegion();
	void enableGenerateButtons(bool enable = true); ///< This function conditionally enables or fully disables the generating buttons.
	void generateKnot(wxCommandEvent& evt);			///< This function checks which of the generating buttons was pressed and calls the appropriate Knot function.
	wxStaticBoxSizer* generateRegionSizer;
		#define XX(Sym, desc) wxButton* generate##Sym##Button;
		SYMMETRIES
		#undef XX

	void initExportRegion();
	void showExportBox();		///< Loops through the Knot and grabs each character, then outputs the contents into the export textbox
	void regenExportBox();		///< Defines the export textbox with a size dependent on the height and width of the knot
	wxStaticBoxSizer* exportRegionSizer;
		wxTextCtrl* exportBox;
		wxFont exportFont;
		declareButton(exportCopy);	///< This function copies the current text data in the exportBox into the clipboard, saving it after closing the program.
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

#pragma once
#include "Constants.h"
#include "DisplayGrid.h"
#include "Knot.h"

// Declare and initialize a button with its corresponding function
#define declareButton(buttonName) \
	wxButton* buttonName##Button; \
	void buttonName##Function(wxCommandEvent& evt)
#define initButton(buttonName, displayText) \
	buttonName##Button = new wxButton(this, wxID_ANY, displayText); \
	buttonName##Button->Bind(wxEVT_BUTTON, &MainWindow::##buttonName##Function, this)

/** As a more specialized \c wxFrame object, this class represents main window of the application;
	most of the WX object member variables are not documented here. */
class MainWindow : public wxFrame {

public:
	MainWindow(int h, int w, wxString title);
	~MainWindow(); ///< Hides this MainWindow object automatically, so the destruction is not visible

	void updateSelectCoord();				///< Updates the displayed selection coordinates, also has other effects.
	void changeSelectCoord(ijSignature);
	void fixSelectCoord();
	void resetSelectCoord();				///< Resets the values of \c iMin, \c jMin, \c iMax, and \c jMax to the default values, by calling MainWindow::changeSelectCoord()

private:
	int h,		///< The height of the knot, i.e. the number of rows.
		w;		///< The width of the knot, i.e. the number of columns.
	int iMin,	///< The zero-indexed upper row of the selection visually (lower numerically)
		jMin,	///< The zero-indexed leftmost column of the selection
		iMax,	///< The zero-indexed lower row of the selection visually (higher numerically)
		jMax;	///< The zero-indexed rightmost column of the selection

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

	void initSelectRegion();
	wxStaticBoxSizer* selectRegionSizer;
		wxStaticText* selectCoord;		// The display of "selection coordinates", i.e. top-left to bottom-right
		wxBoxSizer* selectButtonSizer;	// The selection buttons
			declareButton(selectToggle);	///< This function is bound to the \c show/hide button, so it highlights and unhighlights the selection
				// This button is a show/hide button, which highlights the selection
			declareButton(selectReset);		///< This function resets the selection coordinates by calling MainWindow::resetSelectCoord(), but this function takes in a button event so it can be bound to the \c reset button.
				// This button resets the selection coordinates

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

	static constexpr int GAP_1 = 20; ///< The gap from the outside of the window, and between the grid section and panel section
	static constexpr int GAP_2 = 10; ///< The gap between the panels in the panel section
	static constexpr int GAP_3 =  5; ///< The gap between elements within the panels
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

/* Select Coord functions */
/** \fn MainWindow::changeSelectCoord(ijSignature)
 * Changes the stored values of \c iMin, \c jMin, \c iMax, and \c jMax to the supplied values
 * 
 * Changes the stored values of \c iMin, \c jMin, \c iMax, and \c jMax to the supplied values,
 * unless the supplied value to any of these is \c -1.
 * This function then calls MainWindow::updateSelectionCoord() to automatically update the display every time the values change.
 *
 * \param iMin The zero-indexed upper row of the selection visually (lower numerically)
 * \param jMin The zero-indexed leftmost column of the selection
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMax The zero-indexed rightmost column of the selection
 */
/** \fn MainWindow::fixSelectCoord()
 * Swaps the stored values of \c iMin, \c jMin, \c iMax, and \c jMax such that \c iMin \c <= \c iMax and \c jMin \c <= \c jMax.
 * 
 * Swaps the stored values of \c iMin, \c jMin, \c iMax, and \c jMax such that \c iMin \c <= \c iMax and \c jMin \c <= \c jMax.
 * This function then calls MainWindow::updateSelectionCoord() to automatically update the display every time the values change.
 */
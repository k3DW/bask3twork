#pragma once
#include "Constants.h"
#include "Tile.h"
#include "Knot.h"

class MainWindow;

/** As a more specialized \c wxPanel object, this class represents the grid to be displayed on the left side of the MainWindow. */
class DisplayGrid : public wxPanel {

public:
	DisplayGrid(MainWindow* parent, Knot* knot);
	~DisplayGrid(); ///< Hides this DisplayGrid object automatically, so the destruction is not visible

	void highlightSelection(Selection selection);
	void clearHighlight(bool refresh = true);

	void drawKnot();	///< Pulls data from the Knot object and displays it in the Tile grid

private:
	const int h,			///< The height of the knot, i.e. the number of rows, taken from the Knot in the constructor
			  w;			///< The width of the knot, i.e. the number of columns, taken from the Knot in the constructor
	Knot* const knot;		///< The Knot used for this DisplayGrid, as a constant pointer to the object since each DisplayGrid corresponds to exacty one Knot object

	std::vector<std::vector<Tile*>> tiles;	///< The grid of Tile objects to be used
	wxGridBagSizer* sizer;					///< The sizer used to pack all the components in this \c wxPanel object
	void initAxisLabels();					///< Initializes the X axis labels and Y axis labels by directly adding \c new Tile objects to the sizer
	void initTiles(MainWindow* parent);		///< Initializes the main Tile objects, packing them into the sizer and pulling initial data from the Knot
	
	bool isAnythingHighlighted = false;		///< A flag to determine if any Tile is currently highlighted
};

/* DisplayGrid constructor */
/** \fn DisplayGrid::DisplayGrid(wxWindow* parent, Knot* knot)
 * Sets the \c wxPanel base object, the Knot object, and the size of the Knot as h and w
 * 
 * \param parent The parent \c wxWindow object, which is a DisplayGrid in the case of this program
 * \param knot The Knot object that this DisplayGrid displays
 */

/* Highlight functions */
/** \fn DisplayGrid::highlightSelection(Selection selection)
 * Highlights the Tile objects within the given selection
 * 
 * Only highlights from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * 
 * This function first calls DisplayGrid::clearHighlight() to ensure that there is only one highlighted selection at once.
 * Then it sets \c isAnythingHighlighted to \c true, before calling \c Refresh() on the DisplayGrid object.
 * 
 * \param selection The pair of zero-indexed coordinates representing the top left (lower numerically) and bottom right (higher numerically) of the current selection
 */
/** \fn DisplayGrid::clearHighlight(bool refresh = true)
 * Clears all highlighting of the Tile objects in the whole grid
 * 
 * This function only does anything if \c isAnythingHighlighted . This is to save on computation if there is nothing already highlighted. 
 * If there is anything highlighted at all, this function loops through the entire grid to unhighlights all Tile objects, even if they are not highlighted.
 * Then \c isAnythingHighlighted is set to \c false.
 *
 * \param refresh This parameter has a default value of \c true, and tells the function whether or not to call \c Refresh() on the DisplayGrid object
 */
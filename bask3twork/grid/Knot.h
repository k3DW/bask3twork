#pragma once
#include <optional>
#include "Forward.h"
#include "pure/GridSize.h"

/** This class represents a knot object as a grid of glyphs, with corresponding public functions to generate various symmetries. */
class Knot
{
public:
	Knot(GridSize size, wxStatusBar* statusBar);
	Knot(Glyphs&& glyphs, wxStatusBar* statusBar);
	wxUniChar get(const int i, const int j) const;
	CodePoint code_point(const int i, const int j) const;

	GridSize size;                  ///< The size of the knot
	wxStatusBar* const statusBar;	///< The \c wxStatusBar object from the MainWindow class where the knot should output its progress while generating a knot
	bool wrapXEnabled = false;		///< Is wrapping enabled in the X direction
	bool wrapYEnabled = false;		///< Is wrapping enabled in the Y direction

	void clear(Selection selection, const Tiles& tiles);
	bool generate(Symmetry sym, Selection selection, const Tiles& tiles);

	bool checkWrapping(Selection selection) const;

	Symmetry symmetry_of(Selection selection, const Tiles& tiles) const;

	wxString plaintext() const;

private:
	Glyphs glyphs;	///< The current state of the Knot

	std::optional<Glyphs> tryGenerating(Glyphs glyphGrid, Symmetry sym, Selection selection) const;

	const wxString& status_prefix(Symmetry sym) const;
};

/* Knot::Knot */
/** \fn Knot::Knot(GridSize size, wxStatusBar* statusBar)
 * Constructor for a Knot object with default data.
 * 
 * \param size The size of the knot, i.e. the number of rows and columns.
 * \param statusBar The \c wxStatusBar object from the MainWindow class where the knot should output its progress while generating a knot. This will usually be passed as GetStatusBar() when called.
 */
/** \fn Knot::Knot(Glyphs&& glyphs, wxStatusBar* statusBar)
 * Constructor for a Knot object with provided data.
 * 
 * \param glyphs The data with which to construct the Knot, not checking if any of the values are \c nullptr.
 * \param statusBar The \c wxStatusBar object from the MainWindow class where the knot should output its progress while generating a knot. This will usually be passed as GetStatusBar() when called.
 */
/** \fn Knot::get(const int i, const int j) const
 * Accessor for the \c wxString representation of a Glyph.
 *
 * \param i The row index
 * \param j The column index
 * \return The \c wxString of the Glyph object at the (i,j) location of the knot 
 */

/** \fn Knot::checkWrapping(Selection selection)
 * Check if any of the previous wrapping conditions made it so that non wrapped knots cannot be generated.
 * 
 * This function is different from the other \c check functions, since this function returning \c false skips evaluation of the others.
 */

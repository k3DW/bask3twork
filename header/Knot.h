#pragma once
#include "Constants.h"
#include "Glyph.h"
#include "Glyphs.h"

/** This class represents a knot object as a grid of glyphs, with corresponding public functions to generate various symmetries. */
class Knot {
public:
	Knot(int h, int w, wxStatusBar* statusBar);
	Knot(Glyphs&& glyphs, wxStatusBar* statusBar);
	wxString get(const int i, const int j) const;

	const int h, ///< The height of the knot, i.e. the number of rows. This is publicly accessible but not mutable
			  w; ///< The width of the knot, i.e. the number of columns. This is publicly accessible but not mutable
	wxStatusBar* const statusBar;	///< The \c wxStatusBar object from the MainWindow class where the knot should output its progress while generating a knot
	bool wrapXEnabled = false;		///< Is wrapping enabled in the X direction
	bool wrapYEnabled = false;		///< Is wrapping enabled in the Y direction

	bool generate(Symmetry sym, Selection selection);

	bool checkHoriSym(Selection selection) const;
	bool checkVertSym(Selection selection) const;
	bool checkRot2Sym(Selection selection) const;
	bool checkRot4Sym(Selection selection) const;
	bool checkFwdDiag(Selection selection) const;
	bool checkBackDiag(Selection selection) const;
	bool checkWrapping(Selection selection) const;

private:
	using iterator_pair_fn = std::pair<GlyphIterator, GlyphIterator>(Glyphs::*)(Selection) const;
	using move_fn_pair = std::pair<GlyphIterator& (GlyphIterator::*)(), GlyphIterator& (GlyphIterator::*)()>;
	using connection_pair = std::pair<Connection Glyph::*, Connection Glyph::*>;
	using transform_fn = Connection(*)(Connection);
	bool check_glyph_pair(iterator_pair_fn starting_pair, move_fn_pair moves, connection_pair connections, transform_fn transform, Selection selection) const;

	Glyphs glyphs;	///< The current state of the Knot

	std::optional<Glyphs> tryGenerating(Glyphs glyphGrid, Symmetry sym, Selection selection) const;
};

/* Knot::Knot */
/** \fn Knot::Knot(int h, int w, wxStatusBar* statusBar)
 * Constructor for a Knot object with default data.
 * 
 * \param h The height of the knot, i.e. the number of rows.
 * \param w The width of the knot, i.e. the number of columns.
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

/* Knot::check____Sym */
/** \fn Knot::checkHoriSym(Selection selection)
 * Check if the boundary around the selection is symmetric about the horizontal axis of the selection.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * \param iMin The zero-indexed upper row of the selection visually (lower numerically)
 * \param jMin The zero-indexed leftmost column of the selection
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMax The zero-indexed rightmost column of the selection
 * \return Returns \c true if boundaries have this symmetry.
 */
/** \fn Knot::checkVertSym(Selection selection)
 * Check if the boundary around the selection is symmetric about the vertical axis of the selection.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkRot2Sym(Selection selection)
 * Check if the boundary around the selection is 2-way rotationally symmetric about the central point of the selection.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkRot4Sym(Selection selection)
 * Check if the boundary around the selection is 4-way rotationally symmetric about the central point of the selection.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkFwdDiag(Selection selection)
 * Check if the boundary around the selection is symmetric about the forward diagonal of the selection, meaning it must be square.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkBackDiag(Selection selection)
 * Check if the boundary around the selection is symmetric about the backward diagonal of the selection, meaning it must be square.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkWrapping(Selection selection)
 * Check if any of the previous wrapping conditions made it so that non wrapped knots cannot be generated.
 * 
 * This function is different from the other \c check functions, since this function returning \c false skips evaluation of the others.
 */

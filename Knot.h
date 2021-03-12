#pragma once
#include "Constants.h"
#include "Glyph.h"

/** This class represents a knot object as a grid of glyphs, with corresponding public functions to generate various symmetries. */
class Knot {
public:
	Knot(int h, int w, wxStatusBar* statusBar);
	Knot(GlyphVec2 glyphs, wxStatusBar* statusBar);
	wxString get(const int i, const int j) const;

	const int h, ///< The height of the knot, i.e. the number of rows. This is publicly accessible but not mutable
			  w; ///< The width of the knot, i.e. the number of columns. This is publicly accessible but not mutable
	wxStatusBar* const statusBar;	///< The \c wxStatusBar object from the MainWindow class where the knot should output its progress while generating a knot
	bool wrapXEnabled = false;		///< Is wrapping enabled in the X direction
	bool wrapYEnabled = false;		///< Is wrapping enabled in the Y direction

	bool generate(Symmetry sym, ijSignature);

	bool checkHoriSym(ijSignature) const;
	bool checkVertSym(ijSignature) const;
	bool checkRot2Sym(ijSignature) const;
	bool checkRot4Sym(ijSignature) const;
	bool checkFwdDiag(ijSignature) const;
	bool checkBackDiag(ijSignature) const;
	bool checkWrapping(ijSignature) const;

private:
	GlyphVec2 glyphs;	///< The current state of the Knot, as a 2D std::vector of Glyph pointers

	#define XX(Sym, desc) bool generate##Sym##(GlyphVec2& glyphGrid, ijSignature) const;
	SYMMETRIES
	#undef XX

	static inline bool inSelection(ijSignature, const int i, const int j);
	static inline bool isEvenSegments(const int min, const int max);
};

/* Knot::Knot */
/** \fn Knot::Knot(int h, int w, wxStatusBar* statusBar)
 * Constructor for a Knot object with default data.
 * 
 * \param h The height of the knot, i.e. the number of rows.
 * \param w The width of the knot, i.e. the number of columns.
 * \param statusBar The \c wxStatusBar object from the MainWindow class where the knot should output its progress while generating a knot. This will usually be passed as GetStatusBar() when called.
 */
/** \fn Knot::Knot(GlyphVec2 glyphs, wxStatusBar* statusBar)
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

/* Knot::generate____Sym */
/** \fn Knot::generateNoSym(ijsignature)
 * Generate a knot with no symmetry. 
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * 
 * \param iMin The zero-indexed upper row of the selection visually (lower numerically)
 * \param jMin The zero-indexed leftmost column of the selection
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMax The zero-indexed rightmost column of the selection
 * \return A boolean value denoting whether or not the generating was successful
 */
/** \fn Knot::generateHoriSym(ijsignature)
 * Generate a knot with symmetry across the horizontal axis of the selection.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * The symmtery of the boundary conditions are not checked, so if a selection is chosen with non-symmetric boundaries, the full knot will have broken connections.
 * 
 * See Knot::generateNoSym() for parameters and return value of the selection.
 */
/** \fn Knot::generateVertSym(ijsignature)
 * Generate a knot with symmetry across the vertical axis of the selection.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * The symmtery of the boundary conditions are not checked, so if a selection is chosen with non-symmetric boundaries, the full knot will have broken connections.
 * 
 * See Knot::generateNoSym() for parameters and return value.
 */
/** \fn Knot::generateHoriVertSym(ijsignature)
 * Generate a knot with symmetry across both the horizontal and vertical axes of the selection.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * The symmtery of the boundary conditions are not checked, so if a selection is chosen with non-symmetric boundaries, the full knot will have broken connections.
 * 
 * See Knot::generateNoSym() for parameters and return value.
 */
/** \fn Knot::generateRot2Sym(ijsignature)
 * Generate a knot with 2-way rotational symmetry about the center of the selection.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * The symmtery of the boundary conditions are not checked, so if a selection is chosen with non-symmetric boundaries, the full knot will have broken connections.
 * 
 * See Knot::generateNoSym() for parameters and return value.
 */
/** \fn Knot::generateRot4Sym(ijsignature)
 * Only works on a \b square selection, generate a knot with 4-way rotational symmetry about the center of the selection.
 * 
 * This function will only work properly for \b square selections, and will do nothing if a non-square selection is passed.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * The symmtery of the boundary conditions are not checked, so if a selection is chosen with non-symmetric boundaries, the full knot will have broken connections.
 * 
 * See Knot::generateNoSym() for parameters and return value.
 */
/** \fn Knot::generateFwdDiag(ijsignature)
 * Only works on a \b square selection, generate a knot with mirror symmetry across the forward diagonal of the selection.
 * 
 * This function will only work properly for \b square selections, and will do nothing if a non-square selection is passed.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * The symmtery of the boundary conditions are not checked, so if a selection is chosen with non-symmetric boundaries, the full knot will have broken connections.
 * 
 * See Knot::generateNoSym() for parameters and return value.
 */
/** \fn Knot::generateBackDiag(ijsignature)
 * Only works on a \b square selection, generate a knot with mirror symmetry across the backward diagonal of the selection.
 * 
 * This function will only work properly for \b square selections, and will do nothing if a non-square selection is passed.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * The symmtery of the boundary conditions are not checked, so if a selection is chosen with non-symmetric boundaries, the full knot will have broken connections.
 * 
 * See Knot::generateNoSym() for parameters and return value.
 */
/** \fn Knot::generateFullSym(ijsignature)
 * Only works on a \b square selection, generate a knot with full D4 symmetry, essentially a combination of Knot::generateHoriVertSym() and Knot::generateRot4Sym() concurrently.
 * 
 * This function will only work properly for \b square selections, and will do nothing if a non-square selection is passed.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained, with the outside being treated as an empty connection.
 * The symmtery of the boundary conditions are not checked, so if a selection is chosen with non-symmetric boundaries, the full knot will have broken connections.
 * 
 * See Knot::generateNoSym() for parameters and return value.
 */

/* Knot::check____Sym */
/** \fn Knot::checkHoriSym(ijsignature)
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
/** \fn Knot::checkVertSym(ijsignature)
 * Check if the boundary around the selection is symmetric about the vertical axis of the selection.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkRot2Sym(ijsignature)
 * Check if the boundary around the selection is 2-way rotationally symmetric about the central point of the selection.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkRot4Sym(ijsignature)
 * Check if the boundary around the selection is 4-way rotationally symmetric about the central point of the selection.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkFwdDiag(ijsignature)
 * Check if the boundary around the selection is symmetric about the forward diagonal of the selection, meaning it must be square.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkBackDiag(ijsignature)
 * Check if the boundary around the selection is symmetric about the backward diagonal of the selection, meaning it must be square.
 * 
 * Only checks from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 *
 * See Knot::checkHoriSym() for parameters.
 */
/** \fn Knot::checkWrapping(ijSignature)
 * Check if any of the previous wrapping conditions made it so that non wrapped knots cannot be generated.
 * 
 * This function is different from the other \c check functions, since this function returning false skips evaluation of the others.
 */

/* Private generating helper functions */
/** \fn Knot::inSelection(ijSignature, const int i, const int j)
 * This is a helper function that tells you whether a specific tile location is within the given selection.
 * 
 * \param iMin The zero-indexed upper row of the selection visually (lower numerically)
 * \param jMin The zero-indexed leftmost column of the selection
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMax The zero-indexed rightmost column of the selection
 * \param i The row index being checked
 * \param j The column index being checked
 * \return A boolean value denoting whether or not the location is within the given selection
 */
/** \fn Knot::isEvenSegments(const int min, const int max)
 * This is a helper function that tells you whether the range from \c min to \c max, inclusive, has an even number of segments.
 * 
 * This function assumes \c min \c <= \c max but does not check this, or give an error.
 * 
 * \param min The minimum value of the range, inclusive
 * \param max The maximum value of the range, inclusive
 * \return A boolean value denoting whether or not this range has an even number of segments
 */

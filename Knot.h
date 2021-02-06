#pragma once
#include "Constants.h"
#include "Glyph.h"

#define ijSignature const int iMin, const int jMin, const int iMax, const int jMax

/** This class represents a knot object as a grid of glyphs, with corresponding public functions to generate various symmetries. */
class Knot {
public:
	Knot(int h, int w, wxStatusBar* statusBar);
	wxString get(const int i, const int j) const;

	const int h; ///< The height of the knot, i.e. the number of rows. This is publicly accessible but not mutable
	const int w; ///< The width of the knot, i.e. the number of columns. This is publicly accessible but not mutable
	wxStatusBar* const statusBar; ///< The \c wxStatusBar object from the MainWindow class where the knot should output its progress while generating a knot

	bool generateNoSym(ijSignature);
	bool generateHoriSym(ijSignature);
	bool generateVertSym(ijSignature);
	bool generateHoriVertSym(ijSignature);
	bool generateRot2Sym(ijSignature);
	bool generateRot4Sym(ijSignature);

	bool checkHoriSym(ijSignature) const;
	bool checkVertSym(ijSignature) const;
	bool checkRot2Sym(ijSignature) const;
	bool checkRot4Sym(ijSignature) const;

private:
	GlyphVec2 glyphs; ///< The current state of the Knot, as a 2D std::vector of Glyph pointers

	void mirrorUpToDown(GlyphVec2& glyphGrid, ijSignature) const;
	void mirrorLeftToRight(GlyphVec2& glyphGrid, ijSignature) const;
	void rotate180UpToDown(GlyphVec2& glyphGrid, ijSignature) const;
	void rotate180LeftToRight(GlyphVec2& glyphGrid, ijSignature) const;
	void rotate90FromUpLeft(GlyphVec2& glyphGrid, ijSignature) const;

	void tryGenerating(std::optional<GlyphVec2>& glyphGrid, ijSignature, const Side ignoreSides = Side::NONE, const GlyphFlag boolFlags = GlyphFlag::NONE) const;
	static inline bool inSelection(ijSignature, const int i, const int j);
	static inline bool isEvenSegments(const int min, const int max);
};

/* Knot::Knot */
/** \fn Knot::Knot(int h, int w, wxStatusBar* statusBar)
 * Constructor for a Knot object

 * \param h The height of the knot, i.e. the number of rows.
 * \param w The width of the knot, i.e. the number of columns.
 * \param statusBar The \c wxStatusBar object from the MainWindow class where the knot should output its progress while generating a knot. This will usually be passed as GetStatusBar() when called.
 */
/* Knot::get */
/** \fn Knot::get(const int i, const int j) const
 * Accessor for the \c wxString representations of the Knot

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
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMin The zero-indexed leftmost column of the selection
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
 * This function will only work properly for \b square selections, but will not give any error if a non-square selection is passed.
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
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMin The zero-indexed leftmost column of the selection
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

/* Knot::[doCopy] */
/** \fn Knot::mirrorUpToDown(GlyphVec2& glyphGrid, ijSignature) const
 * Copy the upper half of the selection to the lower half by mirroring.
 *
 * Only mirrors from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax. This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * There is no error checking on the boundaries of the mirrored sections. This will execute regardless.
 * 
 * This function is used in the "generate" functions. In the case of a selection with an odd number of rows, the middle row is unaffected. This function does not affect Knot::glyphs.
 *
 * \param glyphGrid The state of the knot on which to operate
 * \param iMin The zero-indexed upper row of the selection visually (lower numerically)
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMin The zero-indexed leftmost column of the selection
 * \param jMax The zero-indexed rightmost column of the selection
 */
/** \fn Knot::mirrorLeftToRight(GlyphVec2& glyphGrid, ijSignature) const
 * Copy the left half of the selection to the right half by mirroring.
 *
 * Only mirrors from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax. This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * There is no error checking on the boundaries of the mirrored sections. This will execute regardless.
 * 
 * This function is used in the "generate" functions. In the case of a selection with an odd number of columns, the middle column is unaffected. This function does not affect Knot::glyphs.
 *
 * See Knot::mirrorUpToDown() for parameters.
 */
/** \fn Knot::rotate180UpToDown(GlyphVec2& glyphGrid, ijSignature) const
 * Copy the upper half of the selection to the lower half by rotating 180 degrees about the center of the selection.
 *
 * Only mirrors from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax. This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * There is no error checking on the boundaries of the mirrored sections. This will execute regardless.
 * 
 * This function is used in the "generate" functions. In the case of a selection with an odd number of rows, the middle rows is unaffected. This function does not affect Knot::glyphs.
 *
 * See Knot::mirrorUpToDown() for parameters.
 */
/** \fn Knot::rotate180LeftToRight(GlyphVec2& glyphGrid, ijSignature) const
 * Copy the left half of the selection to the right half by rotating 180 degrees about the center of the selection.
 *
 * Only mirrors from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax. This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * There is no error checking on the boundaries of the mirrored sections. This will execute regardless.
 * 
 * This function is used in the "generate" functions. In the case of a selection with an odd number of columns, the middle column is unaffected. This function does not affect Knot::glyphs.
 *
 * See Knot::mirrorUpToDown() for parameters.
 */
/** \fn Knot::rotate90FromUpLeft(GlyphVec2& glyphGrid, ijSignature) const
 * Only works on a \b square selection, copy the upper left quadrant of the selection to the other quadrants by rotating 90 degrees about the center of the selection.
 * 
 * This function will only work properly for \b square selections, and will do \b nothing if a non-square selection is passed.
 *
 * Only mirrors from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax. This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * There is no error checking on the boundaries of the mirrored sections. This will execute regardless.
 * 
 * This function is used in the "generate" functions. In the case of a selection with an odd number of rows and/or columns, the middle row and/or column will be unaffected.
 * This function does not affect Knot::glyphs.
 *
 * See Knot::mirrorUpToDown() for parameters.
 */

/* Private generating helper functions */
/** \fn Knot::tryGenerating(std::optional<GlyphVec2>& glyphGrid, ijSignature, const Side ignoreSides = Side::NONE, const GlyphFlag boolFlags = GlyphFlag::NONE) const
 * This function tries to generate with no symmetry, only once with no looping, and sets \c glyphGrid to \c std::nullopt if it cannot be generated.
 * 
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * 
 * The \c ignoreSides flag only affects the side-most Glyphs. For example, if \c ignoreSides includes \c Side::LEFT,
 * then the left side connection will only be ignored on the leftmost Glyphs of the selection.
 * 
 * The \c boolFlags flag affects all Glyphs in the selection. All flags which are not specified are ignored,
 * instead of being checked for a value of zero.
 *
 * \param glyphGrid The 2D std::vector of Glyph pointers to pass into the function to be edited, as an \c optional
 * \param iMin The zero-indexed upper row of the selection visually (lower numerically)
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMin The zero-indexed leftmost column of the selection
 * \param jMax The zero-indexed rightmost column of the selection
 * \param ignoreSides The sides to ignore, as an or-composition of Side values
 * \param boolFlags The conditions which need to be met, as an or-composition of GlyphFlag values
 *
 * \b Methodology
 */
/** \fn Knot::inSelection(ijSignature, const int i, const int j)
 * This is a helper function that tells you whether a specific tile location is within the given selection.
 * 
 * \param iMin The zero-indexed upper row of the selection visually (lower numerically)
 * \param iMax The zero-indexed lower row of the selection visually (higher numerically)
 * \param jMin The zero-indexed leftmost column of the selection
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

#include "Knot.h"

Knot::Knot(int h, int w, wxStatusBar* statusBar) : h(h), w(w), statusBar(statusBar), glyphs(GlyphVec2(h, GlyphVec1(w, DefaultGlyph))) {}
Knot::Knot(GlyphVec2 glyphs, wxStatusBar* statusBar) : h(glyphs.size()), w(glyphs[0].size()), statusBar(statusBar), glyphs(glyphs) {}
wxString Knot::get(const int i, const int j) const {
	return glyphs[i][j]->chr;
}

bool Knot::generate(Symmetry sym, ijSignature)
/** Generate a knot with the given symmetry in the given selection.
 *
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained.
 * This function does not check the boundary symmetries to determine whether the specified symmetry is allowed.
 *
 * \param sym The symmetry of the knot to be generated
 * \param iMin The zero-indexed upper row of the selection visually(lower numerically)
 * \param jMin The zero-indexed leftmost column of the selection
 * \param iMax The zero-indexed lower row of the selection visually(higher numerically)
 * \param jMax The zero-indexed rightmost column of the selection
 * \return A boolean value denoting whether or not the generating was successful
 * 
 * \b Method
 */
{
	/// First, make a copy of \c glyphs, and set all members in the selection to \c nullptr to denote that they are unassigned.
	GlyphVec2 baseGlyphs = glyphs;
	for (int i = iMin; i <= iMax; i++)
		for (int j = jMin; j <= jMax; j++)
			baseGlyphs[i][j] = nullptr;

	/// Then, instantiate a pointer-to-Knot-member-function and set it to the appropriate generating funtion depending on the symmetry.
	/// This pointer and be dereferenced and called the same way, regardless of the generating function.
	bool (Knot:: * generateFunction)(GlyphVec2&, int, int, int, int) const;
	wxString statusBeginning = "Generating ";
	switch (sym) {
	case Symmetry::NoSym:
		generateFunction = &Knot::generateNoSym;
		statusBeginning += "no symmetry... ";
		break;
	case Symmetry::HoriSym:
		generateFunction = &Knot::generateHoriSym;
		statusBeginning += "horizontal symmetry... ";
		break;
	case Symmetry::VertSym:
		generateFunction = &Knot::generateVertSym;
		statusBeginning += "vertical symmetry... ";
		break;
	/*
	case Symmetry::HoriVertSym:
		generateFunction = &Knot::generateHoriVertSym;
		statusBeginning += "horizontal + vertical symmetry... ";
		break;
	//*/
	default:
		return false;
	}

	/// Next, enter a loop, counting the number of attempts made at generating this knot. The steps are as follows.
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		/// (1) At certain intervals of numbers of attempts, update the status bar with the number of attempts made.
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText(statusBeginning + "Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		/// (2) Make a copy of the previously copied and modified version of \c glyphs, and then call the generating function on it.
		///		If the generating function returns \c false, then the generating has failed, so \c continue the loop.
		GlyphVec2 newGlyphs = baseGlyphs;
		if (!(this->*generateFunction)(newGlyphs, iMin, jMin, iMax, jMax))
			continue;

		/// (3) If the knot has been successfully generated, set \c glyphs equal to this generated version,
		///		then return \c true.
		glyphs = newGlyphs;
		return true;
	}
	/// (4) If this loop has been completed, then the maximum number of attempts have been tried. Therefore return \c false.
	return false;
}

inline bool Knot::generateNoSym(GlyphVec2& glyphGrid, ijSignature) const
/** Generate a knot with no symmetry, for the given selection.
 * 
 * The same conditions apply as in the first paragraph of Knot::generate().
 * 
 * The \c glyphGrid variable is supplied from Knot::generate(), already initialized with \c nullptr entries in the selection.
 * 
 * \b Method
 */
{
	/// For each location in the selection, do the following.
	for (int i = iMin; i <= iMax; i++) {
		for (int j = jMin; j <= jMax; j++) {
			/// \b (1) If the Glyph in this location is already set, \c continue the loop.
			if (glyphGrid[i][j]) continue;

			/// \b (2) If the Glyph has yet to be set, generate a RandomGlyph() for this location, with the \c GlyphFlag::NONE flag.
			///	For each of the 4 \c Connection parameters, there are many possible cases, implemented in a large nested ternary operation:
			glyphGrid[i][j] = RandomGlyph(
				/** (a) If this Glyph location is on the outer edge of the selection on this particular side, branch to condition B, otherwise branch to condition E.
				 *
				 *	(b) If wrapping is not enabled in this direction, then the parameter should be \c Connection::EMPTY.
				 *	(c) If the Glyph directly on the other end of the wrap is not assigned, then the parameter should be \c Connection::DO_NOT_CARE.
				 *	(d) In the general wrapping case, where none of the other special wrapping cases apply, the parameter should be
				 *		the connection on the opposite side of the glyph on the other end of the wrap.
				 *
				 *	(e) If the next glyph on this particular side is not yet assigned, the parameter should be \c Connection::DO_NOT_CARE.
				 *	(f) In the general case, where none of the other special cases apply, the parameter should be
				 *		the connection on the opposite side, from the neighbouring glyph on this particular side.
				 */
				i == 0		? (!wrapYEnabled ? Connection::EMPTY : !glyphGrid[h - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[h - 1][j]->down	) : (!glyphGrid[i - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i - 1][j]->down	) ,
				i == h - 1  ? (!wrapYEnabled ? Connection::EMPTY : !glyphGrid[0][j]		? Connection::DO_NOT_CARE : glyphGrid[0][j]->up			) : (!glyphGrid[i + 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i + 1][j]->up	) ,
				j == 0		? (!wrapXEnabled ? Connection::EMPTY : !glyphGrid[i][w - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][w - 1]->right	) : (!glyphGrid[i][j - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j - 1]->right) ,
				j == w - 1  ? (!wrapXEnabled ? Connection::EMPTY : !glyphGrid[i][0]		? Connection::DO_NOT_CARE : glyphGrid[i][0]->left		) : (!glyphGrid[i][j + 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j + 1]->left	) ,
				GlyphFlag::NONE
			);

			/// \b (3) If this newly generated Glyph turns out to be \c nullptr, then there were no options for this location. Return \c false.
			if (!glyphGrid[i][j]) return false;
		}
	}
	/// \b (4) If the loop finishes, then the Knot has been successfully generated. Return \c true.
	return true;
}
inline bool Knot::generateHoriSym(GlyphVec2& glyphGrid, ijSignature) const
/** Generate a knot with horizontal symmetry, for the given selection.
 *
 * The same conditions apply as in the first paragraph of Knot::generate().
 *
 * The \c glyphGrid variable is supplied from Knot::generate(), already initialized with \c nullptr entries in the selection.
 *
 * \b Method
 */
{
	/// First, initialize some variables, like the index of the middle row,
	/// the GlyphFlag for all Glyphs in this middle row, and the boolean for whether or not the selection encompasses all rows.
	const int iMid = (iMin + iMax) / 2;
	const GlyphFlag midRowFlag = isEvenSegments(iMin, iMax) ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const bool isAllRows = iMin == 0 && iMax == h - 1;

	/// For each location in the selection, do the following.
	for (int i = iMin, iDecr = iMax; i <= iDecr; i++, iDecr--) {
		for (int j = jMin; j <= jMax; j++) {
			/// \b (1) If the Glyph in this location is already set, \c continue the loop.
			if (glyphGrid[i][j]) continue;

			/// \b (2) If the Glyph has yet to be set, generate a RandomGlyph() for this location, with the \c GlyphFlag::NONE flag.
			///	For each of the 4 \c Connection parameters, there are many possible cases, implemented in a large nested ternary operation,
			/// which is described in Knot::generateNoSym().
			/// The \c boolFlags argument is dependent on the row,
			/// such that (a) the Glyph connects to it's Y-mirrored image on its top side on the top row if the selection spans all rows,
			/// (b) the Glyph is appropriately flagged for the middle row if it is in the middle row,
			/// or (c) there are no further restrictions otherwise.
			glyphGrid[i][j] = RandomGlyph(
				i == 0		? (!wrapYEnabled ? Connection::EMPTY : !glyphGrid[h - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[h - 1][j]->down	) : (!glyphGrid[i - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i - 1][j]->down	) ,
				i == h - 1  ? (!wrapYEnabled ? Connection::EMPTY : !glyphGrid[0][j]		? Connection::DO_NOT_CARE : glyphGrid[0][j]->up			) : (!glyphGrid[i + 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i + 1][j]->up	) ,
				j == 0		? (!wrapXEnabled ? Connection::EMPTY : !glyphGrid[i][w - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][w - 1]->right	) : (!glyphGrid[i][j - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j - 1]->right) ,
				j == w - 1  ? (!wrapXEnabled ? Connection::EMPTY : !glyphGrid[i][0]		? Connection::DO_NOT_CARE : glyphGrid[i][0]->left		) : (!glyphGrid[i][j + 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j + 1]->left	) ,
				(isAllRows && i == 0) ? GlyphFlag::CT_MIRU : i == iMid ? midRowFlag : GlyphFlag::NONE
			);

			/// \b (3) If this newly generated Glyph turns out to be \c nullptr, then there were no options for this location. Return \c false.
			if (!glyphGrid[i][j]) return false;

			/// \b (4) The corresponding Glyph across the horizontal line is set to the mirrored version of the Glyph which was just generated.
			glyphGrid[iDecr][j] = glyphGrid[i][j]->mirroredX;
		}
	}

	/// \b (5) If the loop finishes, then the Knot has been successfully generated. Return \c true.
	return true;
}
inline bool Knot::generateVertSym(GlyphVec2& glyphGrid, ijSignature) const
/** Generate a knot with vertical symmetry, for the given selection.
 *
 * The same conditions apply as in the first paragraph of Knot::generate().
 *
 * The \c glyphGrid variable is supplied from Knot::generate(), already initialized with \c nullptr entries in the selection.
 *
 * \b Method
 */
{
	/// First, initialize some variables, like the index of the middle column,
	/// the GlyphFlag for all Glyphs in this middle column, and the boolean for whether or not the selection encompasses all columns.
	const int jMid = (jMin + jMax) / 2;
	const GlyphFlag midColFlag = isEvenSegments(jMin, jMax) ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	const bool isAllCols = jMin == 0 && jMax == w - 1;

	/// For each location in the selection, do the following.
	for (int i = iMin; i <= iMax; i++) {
		for (int j = jMin, jDecr = jMax; j <= jDecr; j++, jDecr--) {
			/// \b (1) If the Glyph in this location is already set, \c continue the loop.
			if (glyphGrid[i][j]) continue;

			/// \b (2) If the Glyph has yet to be set, generate a RandomGlyph() for this location, with the \c GlyphFlag::NONE flag.
			///	For each of the 4 \c Connection parameters, there are many possible cases, implemented in a large nested ternary operation,
			/// which is described in Knot::generateNoSym().
			/// The \c boolFlags argument is dependent on the column,
			/// such that (a) the Glyph connects to it's X-mirrored image on its left side on the left column if the selection spans all columns,
			/// (b) the Glyph is appropriately flagged for the middle column if it is in the middle column,
			/// or (c) there are no further restrictions otherwise.
			glyphGrid[i][j] = RandomGlyph(
				i == 0		? (!wrapYEnabled ? Connection::EMPTY : !glyphGrid[h - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[h - 1][j]->down	) : (!glyphGrid[i - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i - 1][j]->down	) ,
				i == h - 1  ? (!wrapYEnabled ? Connection::EMPTY : !glyphGrid[0][j]		? Connection::DO_NOT_CARE : glyphGrid[0][j]->up			) : (!glyphGrid[i + 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i + 1][j]->up	) ,
				j == 0		? (!wrapXEnabled ? Connection::EMPTY : !glyphGrid[i][w - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][w - 1]->right	) : (!glyphGrid[i][j - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j - 1]->right) ,
				j == w - 1  ? (!wrapXEnabled ? Connection::EMPTY : !glyphGrid[i][0]		? Connection::DO_NOT_CARE : glyphGrid[i][0]->left		) : (!glyphGrid[i][j + 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j + 1]->left	) ,
				(isAllCols && j == 0) ? GlyphFlag::CT_MIRL : j == jMid ? midColFlag : GlyphFlag::NONE
			);

			/// \b (3) If this newly generated Glyph turns out to be \c nullptr, then there were no options for this location. Return \c false.
			if (!glyphGrid[i][j]) return false;

			/// \b (4) The corresponding Glyph across the vertical line is set to the mirrored version of the Glyph which was just generated.
			glyphGrid[i][jDecr] = glyphGrid[i][j]->mirroredY;
		}
	}

	/// \b (5) If the loop finishes, then the Knot has been successfully generated. Return \c true.
	return true;
}
//inline bool Knot::generateHoriVertSym(GlyphVec2& glyphGrid, ijSignature) const
/** Generate a knot with vertical symmetry, for the given selection.
 *
 * The same conditions apply as in the first paragraph of Knot::generate().
 *
 * The \c glyphGrid variable is supplied from Knot::generate(), already initialized with \c nullptr entries in the selection.
 *
 * \b Method
 */
/*{
	/// First, initialize the same initial variables from both Knot::generateHoriSym() and Knot::generateVertSym().
	const int iMid = (iMin + iMax) / 2;
	const int jMid = (jMin + jMax) / 2;
	const GlyphFlag midRowFlag = isEvenSegments(iMin, iMax) ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const GlyphFlag midColFlag = isEvenSegments(jMin, jMax) ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	const bool isAllRows = iMin == 0 && iMax == h - 1;
	const bool isAllCols = jMin == 0 && jMax == w - 1;

	/// For each location in the selection, do the following.
	for (int i = iMin, iDecr = iMax; i <= iDecr; i++, iDecr--) {
		for (int j = jMin, jDecr = jMax; j <= jDecr; j++, jDecr--) {
			/// \b (1) If the Glyph in this location is already set, \c continue the loop.
			if (glyphGrid[i][j]) continue;

			/// \b (2) If the Glyph has yet to be set, generate a RandomGlyph() for this location, with the \c GlyphFlag::NONE flag.
			///	For each of the 4 \c Connection parameters, there are many possible cases, implemented in a large nested ternary operation,
			/// which is described in Knot::generateNoSym().
			/// The \c boolFlags argument is dependent on the row and the column,
			/// as a union of the logic done in Knot::generateHoriSym() and Knot::generateVertSym().
			glyphGrid[i][j] = RandomGlyph(
				i == 0		? (!wrapYEnabled ? Connection::EMPTY : !glyphGrid[h - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[h - 1][j]->down	) : (!glyphGrid[i - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i - 1][j]->down	) ,
				i == h - 1  ? (!wrapYEnabled ? Connection::EMPTY : !glyphGrid[0][j]		? Connection::DO_NOT_CARE : glyphGrid[0][j]->up			) : (!glyphGrid[i + 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i + 1][j]->up	) ,
				j == 0		? (!wrapXEnabled ? Connection::EMPTY : !glyphGrid[i][w - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][w - 1]->right	) : (!glyphGrid[i][j - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j - 1]->right) ,
				j == w - 1  ? (!wrapXEnabled ? Connection::EMPTY : !glyphGrid[i][0]		? Connection::DO_NOT_CARE : glyphGrid[i][0]->left		) : (!glyphGrid[i][j + 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j + 1]->left	) ,
				((isAllRows && i == 0) ? GlyphFlag::CT_MIRU : i == iMid ? midRowFlag : GlyphFlag::NONE) | ((isAllCols && j == 0) ? GlyphFlag::CT_MIRL : j == jMid ? midColFlag : GlyphFlag::NONE)
			);

			/// \b (3) If this newly generated Glyph turns out to be \c nullptr, then there were no options for this location. Return \c false.
			if (!glyphGrid[i][j]) return false;

			/// \b (4) The corresponding mirrored Glyph pointers are set. Across the horizontal line, vertical line, and both.
			glyphGrid[iDecr][j] = glyphGrid[i][j]->mirroredX;
			glyphGrid[i][jDecr] = glyphGrid[i][j]->mirroredY;
			glyphGrid[iDecr][jDecr] = glyphGrid[i][j]->rotated2;
		}
	}

	/// \b (5) If the loop finishes, then the Knot has been successfully generated. Return \c true.
	return true;
}//*/

bool Knot::checkHoriSym(ijSignature) const {
	if (iMin == 0 && iMax == h - 1 && jMin == 0 && jMax == w - 1)
		return true;

	Connection upConnection, downConnection;

	// Checking the left and right sides
	for (int iIncr = iMin, iDecr = iMax; iIncr <= iDecr; iIncr++, iDecr--) { // for each row
		// Left side
		upConnection	= glyphs[iIncr][jMin]->left;
		downConnection	= glyphs[iDecr][jMin]->left;
		if (upConnection != mirXConnection(downConnection)) return false;
		// Right side
		upConnection	= glyphs[iIncr][jMax]->right;
		downConnection	= glyphs[iDecr][jMax]->right;
		if (upConnection != mirXConnection(downConnection)) return false;
	}

	// Checking the up and down sides
	for (int j = jMin; j <= jMax; j++) { // for each column
		upConnection	= glyphs[iMin][j]->up;
		downConnection	= glyphs[iMax][j]->down;
		if (upConnection != mirXConnection(downConnection)) return false;
	}

	return true;
}
bool Knot::checkVertSym(ijSignature) const {
	if (iMin == 0 && iMax == h - 1 && jMin == 0 && jMax == w - 1)
		return true;

	Connection leftConnection, rightConnection;

	// Checking the up and down sides
	for (int jIncr = jMin, jDecr = jMax; jIncr <= jDecr; jIncr++, jDecr--) { // for each column
		// Up side
		leftConnection	= glyphs[iMin][jIncr]->up;
		rightConnection = glyphs[iMin][jDecr]->up;
		if (leftConnection != mirYConnection(rightConnection)) return false;
		// Down side
		leftConnection	= glyphs[iMax][jIncr]->down;
		rightConnection = glyphs[iMax][jDecr]->down;
		if (leftConnection != mirYConnection(rightConnection)) return false;
	}

	// Checking the left and right sides
	for (int i = iMin; i <= iMax; i++) { // for each row
		leftConnection	= glyphs[i][jMin]->left;
		rightConnection = glyphs[i][jMax]->right;
		if (leftConnection != mirYConnection(rightConnection)) return false;
	}

	return true;
}
bool Knot::checkRot2Sym(ijSignature) const {
	if (iMin == 0 && iMax == h - 1 && jMin == 0 && jMax == w - 1)
		return true;

	Connection upConnection, downConnection;
	// Checking the up and down sides
	for (int jIncr = jMin, jDecr = jMax; jIncr <= jMax; jIncr++, jDecr--) { // for each column
		upConnection	= glyphs[iMin][jIncr]->up;
		downConnection	= glyphs[iMax][jDecr]->down;
		if (upConnection != rot2Connection(downConnection)) return false;
	}

	Connection leftConnection, rightConnection;
	// Checking the left and right sides
	for (int iIncr = iMin, iDecr = iMax; iIncr <= iMax; iIncr++, iDecr--) { // for each row
		leftConnection	= glyphs[iIncr][jMin]->left;
		rightConnection = glyphs[iDecr][jMax]->right;
		if (leftConnection != rot2Connection(rightConnection)) return false;
	}

	return true;
}
bool Knot::checkRot4Sym(ijSignature) const {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square

	if (iMin == 0 && iMax == h - 1 && jMin == 0 && jMax == w - 1)
		return true;

	Connection upConnection, downConnection, leftConnection, rightConnection;
	for (int offset = 0; offset <= iMax - iMin; offset++) {
		upConnection	= glyphs[iMin][jMin + offset]->up;		// Top row, from left to right
		downConnection	= glyphs[iMax][jMax - offset]->down;	// Bottom row, from right to left
		leftConnection	= glyphs[iMax - offset][jMin]->left;	// Left column, from bottom to top
		rightConnection = glyphs[iMin + offset][jMax]->right;	// Right column, from top to bottom
		if (upConnection != rot4Connection(leftConnection) ||
			upConnection != rot2Connection(downConnection) ||
			rot4Connection(upConnection) != rightConnection ) return false;
	}

	return true;
}
bool Knot::checkFwdDiag(ijSignature) const {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square

	if (iMin == 0 && iMax == h - 1 && jMin == 0 && jMax == w - 1)
		return true;

	Connection upConnection, downConnection, leftConnection, rightConnection;
	for (int offset = 0; offset <= iMax - iMin; offset++) {
		upConnection	= glyphs[iMin][jMin + offset]->up;		// Top row, from left to right
		downConnection	= glyphs[iMax][jMin + offset]->down;	// Bottom row, from left to right
		leftConnection	= glyphs[iMax - offset][jMin]->left;	// Left column, from bottom to top
		rightConnection = glyphs[iMax - offset][jMax]->right;	// Right column, from bottom to top
		if (upConnection != mirFDConnection(rightConnection) || leftConnection != mirFDConnection(downConnection))
			return false;
	}

	return true;
}
bool Knot::checkBackDiag(ijSignature) const {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square

	if (iMin == 0 && iMax == h - 1 && jMin == 0 && jMax == w - 1)
		return true;

	Connection upConnection, downConnection, leftConnection, rightConnection;
	for (int offset = 0; offset <= iMax - iMin; offset++) {
		upConnection	= glyphs[iMin][jMin + offset]->up;		// Top row, from left to right
		downConnection	= glyphs[iMax][jMin + offset]->down;	// Bottom row, from left to right
		leftConnection	= glyphs[iMin + offset][jMin]->left;	// Left column, from top to bottom
		rightConnection = glyphs[iMin + offset][jMax]->right;	// Right column, from top to bottom
		if (upConnection != mirBDConnection(leftConnection) || rightConnection != mirBDConnection(downConnection))
			return false;
	}

	return true;
}
bool Knot::checkWrapping(ijSignature) const {
	// If the wrap in the Y direction is not enabled
	if (!wrapYEnabled) {
		// Only do the check if the selection either includes the top or bottom row, but not both
		if (iMin == 0 && iMax != h - 1) {
			for (int j = jMin; j <= jMax; j++)
				if (glyphs[iMin][j]->up != Connection::EMPTY)
					return false;
		}
		else if (iMin != 0 && iMax == h - 1) {
			for (int j = jMin; j <= jMax; j++)
				if (glyphs[iMax][j]->down != Connection::EMPTY)
					return false;
		}
	}

	// If the wrap in the X direction is not enabled
	if (!wrapXEnabled) {
		// Only do the check if the selection either includes the left or right column, but not both
		if (jMin == 0 && jMax != w - 1) {
			for (int i = iMin; i <= iMax; i++)
				if (glyphs[i][jMin]->left != Connection::EMPTY)
					return false;
		}
		else if (jMin != 0 && jMax == w - 1) {
			for (int i = iMin; i <= iMax; i++)
				if (glyphs[i][jMax]->right != Connection::EMPTY)
					return false;
		}
	}

	return true;
}

void Knot::mirrorUpToDown(GlyphVec2& glyphGrid, ijSignature) const {
	for (int iIncr = iMin, iDecr = iMax; iIncr < iDecr; iIncr++, iDecr--)
		for (int j = jMin; j <= jMax; j++)
			glyphGrid[iDecr][j] = glyphGrid[iIncr][j]->mirroredX;
}
void Knot::mirrorLeftToRight(GlyphVec2& glyphGrid, ijSignature) const {
	for (int i = iMin; i <= iMax; i++)
		for (int jIncr = jMin, jDecr = jMax; jIncr < jDecr; jIncr++, jDecr--)
			glyphGrid[i][jDecr] = glyphGrid[i][jIncr]->mirroredY;
}
void Knot::rotate180UpToDown(GlyphVec2& glyphGrid, ijSignature) const {
	for (int iIncr = iMin, iDecr = iMax; iIncr < iDecr; iIncr++, iDecr--)
		for (int jIncr = jMin, jDecr = jMax; jIncr <= jMax; jIncr++, jDecr--)
			glyphGrid[iDecr][jDecr] = glyphGrid[iIncr][jIncr]->rotated2;
}
void Knot::rotate180LeftToRight(GlyphVec2& glyphGrid, ijSignature) const {
	for (int iIncr = iMin, iDecr = iMax; iIncr <= iMax; iIncr++, iDecr--)
		for (int jIncr = jMin, jDecr = jMax; jIncr < jDecr; jIncr++, jDecr--)
			glyphGrid[iDecr][jDecr] = glyphGrid[iIncr][jIncr]->rotated2;
}
void Knot::rotate90FromUpLeft(GlyphVec2& glyphGrid, ijSignature) const {
	if (iMax - iMin != jMax - jMin) return; // The selection must be square for this to work

	// This function uses the up-half of the middle column(s)
	const int iMid = (iMin + iMax) / 2;
	const int jMid = (jMin + jMax) / 2;
	const bool isEven = isEvenSegments(iMin, iMax);

	for (int iOffset = 0; iOffset <= iMid - iMin - !isEven; iOffset++) {
		for (int jOffset = 0; jOffset <= jMid - jMin; jOffset++) {
			glyphGrid[iMin + jOffset][jMax - iOffset] = glyphGrid[iMin + iOffset][jMin + jOffset]->rotated4;
			glyphGrid[iMax - iOffset][jMax - jOffset] = glyphGrid[iMin + iOffset][jMin + jOffset]->rotated2; // Same as ->rotated4->rotated4
			glyphGrid[iMax - jOffset][jMin + iOffset] = glyphGrid[iMin + iOffset][jMin + jOffset]->rotated2->rotated4; // Same as ->rotated4->rotated4->rotated4
		}
	}
}

void Knot::tryGenerating(std::optional<GlyphVec2>& glyphGrid, ijSignature, const Side ignoreSides, const GlyphFlag boolFlags) const {
	/// First, make a copy of \c glyphGrid to make changes without affecting the input.
	GlyphVec2 newGlyphs = *glyphGrid;
	
	/// Then, set the \c const \c Glyph* pointers of the copy to \c nullptr, if they are within the selection.
	/// This signifies that these values are not set, since they will be regenerated in this function.
	for (int i = iMin; i <= iMax; i++)
		for (int j = jMin; j <= jMax; j++)
			if (inSelection(iMin, jMin, iMax, jMax, i, j))
				newGlyphs[i][j] = nullptr;

	// As an intermediary step, turn the \c ignoreSides parameter into its individual 4 \c bool value, one for each side.
	const bool ignoreUp		= (ignoreSides & Side::UP)		!= Side::NONE;
	const bool ignoreDown	= (ignoreSides & Side::DOWN)	!= Side::NONE;
	const bool ignoreLeft	= (ignoreSides & Side::LEFT)	!= Side::NONE;
	const bool ignoreRight	= (ignoreSides & Side::RIGHT)	!= Side::NONE;
	
	/// Next, generate the glyphs by iterating over i,j coordinates within the selection.
	/// For each i,j pair, do the following 3 steps.
	for (int i = iMin; i <= iMax; i++) {
		for (int j = jMin; j <= jMax; j++) {
			GlyphVec1 possibilities = PossibleGlyphs(
				/** \b (1) Generate the \c vector of all possible glyphs that can fit into this spot, using the \c PossibleGlyphs() function,
				 *	passing on the \c boolFlags parameter directly.
				 *	For each of the 4 \c Connection parameters, there are many possible cases, implemented in a large nested ternary operation: 
				 *	
				 *	(a) The side should be ignored based on \c ignoreSide, therefore the parameter should be \c Connection::DO_NOT_CARE.
				 *	(b) This glyph is on the outer edge of the selection on this particular side, so branch to condition C, otherwise branch to condition F.
				 *	
				 *	(c) Wrapping is not enabled in this direction, therefore the parameter should be \c Connection::EMPTY.
				 *	(d) The glyph directly on the other end of the wrap is not assigned, therefore the parameter should be \c Connection::DO_NOT_CARE.
				 *	(e) The general wrapping case, where none of the other special wrapping cases apply, therefore the parameter should be
				 *		the connection on the opposite side of the glyph on the other end of the wrap.
				 *	
				 *	(f) The next glyph on this particular side is not yet assigned, therefore the parameter should be \c Connection::DO_NOT_CARE.
				 *	(g) The general case, where none of the other special cases apply, therefore the parameter should be
				 *		the connection on the opposite side, from the neighbouring glyph on this particular side.
				 */

				ignoreUp	&& i == iMin ? Connection::DO_NOT_CARE : i == 0		? (!wrapYEnabled ? Connection::EMPTY : !newGlyphs[h - 1][j]	? Connection::DO_NOT_CARE : newGlyphs[h - 1][j]->down	) : (!newGlyphs[i - 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i - 1][j]->down	) ,
				ignoreDown	&& i == iMax ? Connection::DO_NOT_CARE : i == h - 1	? (!wrapYEnabled ? Connection::EMPTY : !newGlyphs[0][j]		? Connection::DO_NOT_CARE : newGlyphs[0][j]->up			) : (!newGlyphs[i + 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i + 1][j]->up	) ,
				ignoreLeft	&& j == jMin ? Connection::DO_NOT_CARE : j == 0		? (!wrapXEnabled ? Connection::EMPTY : !newGlyphs[i][w - 1]	? Connection::DO_NOT_CARE : newGlyphs[i][w - 1]->right	) : (!newGlyphs[i][j - 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j - 1]->right) ,
				ignoreRight	&& j == jMax ? Connection::DO_NOT_CARE : j == w - 1	? (!wrapXEnabled ? Connection::EMPTY : !newGlyphs[i][0]		? Connection::DO_NOT_CARE : newGlyphs[i][0]->left		) : (!newGlyphs[i][j + 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j + 1]->left	) ,
				boolFlags
			);
			
			/// \b (2) If there are no possible glyphs for this space, set \c glyphGrid to the null \c optional and end the function.
			if (possibilities.size() == 0) {
				glyphGrid = std::nullopt;
				return;
			}
			
			/// \b (3) If step 2 did not end the function, set the glyph in the copy as a random choice from the generated possibilities.
			newGlyphs[i][j] = possibilities[rand() % possibilities.size()]; // Pick a random glyph, now that we know it has a nonzero size
		}
	}

	/// At the end of the function, set \c glyphGrid equal to the created copy.
	glyphGrid = newGlyphs;
}
void Knot::tryGeneratingDiag(std::optional<GlyphVec2>& glyphGrid, ijSignature, const bool fwdDiag, const Side ignoreSides) const {
	/// First, make sure that the selection is square. If is it not, end the function immediately.
	if (iMax - iMin != jMax - jMin) { // The selection must be square
		glyphGrid = std::nullopt;
		return;
	}

	/// Next, make a copy of \c glyphGrid to make changes without affecting the input.
	GlyphVec2 newGlyphs = *glyphGrid;
	
	/// Then, set the \c const \c Glyph* pointers of the copy to \c nullptr, if they are within the selection.
	/// This signifies that these values are not set, since they will be regenerated in this function.
	for (int i = iMin; i <= iMax; i++)
		for (int j = jMin; j <= jMax; j++)
			if (inSelection(iMin, jMin, iMax, jMax, i, j))
				newGlyphs[i][j] = nullptr;

	// As an intermediary step, turn the \c ignoreSides parameter into its individual 4 \c bool value, one for each side.
	const bool ignoreUp		= (ignoreSides & Side::UP)		!= Side::NONE;
	const bool ignoreDown	= (ignoreSides & Side::DOWN)	!= Side::NONE;
	const bool ignoreLeft	= (ignoreSides & Side::LEFT)	!= Side::NONE;
	const bool ignoreRight	= (ignoreSides & Side::RIGHT)	!= Side::NONE;

	/// Next, generate the glyphs by iterating over i,j coordinates within the selection.
	/// This is done similarly to Knot::tryGenerating(), with a few modifications.
	/// Every time a glyph is added, its mirror across the diagonal is also added, unless it is along the diagonal itself.
	/// This requires an extra check at the beginning to see if the current glyph is assigned yet, and a check at the end to see if mirroring should occur.
	for (int i = (fwdDiag ? iMax : iMin); (fwdDiag ? i >= iMin : i <= iMax); (fwdDiag ? i-- : i++)) {
		bool firstInRow = true;
		for (int j = jMin; j <= jMax; j++) {
			if (newGlyphs[i][j]) continue;

			GlyphVec1 possibilities = PossibleGlyphs(
				ignoreUp	&& i == iMin ? Connection::DO_NOT_CARE : i == 0		? (!wrapYEnabled ? Connection::EMPTY : !newGlyphs[h - 1][j]	? Connection::DO_NOT_CARE : newGlyphs[h - 1][j]->down	) : (!newGlyphs[i - 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i - 1][j]->down	) ,
				ignoreDown	&& i == iMax ? Connection::DO_NOT_CARE : i == h - 1	? (!wrapYEnabled ? Connection::EMPTY : !newGlyphs[0][j]		? Connection::DO_NOT_CARE : newGlyphs[0][j]->up			) : (!newGlyphs[i + 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i + 1][j]->up	) ,
				ignoreLeft	&& j == jMin ? Connection::DO_NOT_CARE : j == 0		? (!wrapXEnabled ? Connection::EMPTY : !newGlyphs[i][w - 1]	? Connection::DO_NOT_CARE : newGlyphs[i][w - 1]->right	) : (!newGlyphs[i][j - 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j - 1]->right) ,
				ignoreRight	&& j == jMax ? Connection::DO_NOT_CARE : j == w - 1	? (!wrapXEnabled ? Connection::EMPTY : !newGlyphs[i][0]		? Connection::DO_NOT_CARE : newGlyphs[i][0]->left		) : (!newGlyphs[i][j + 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j + 1]->left	) ,
				firstInRow ? (fwdDiag ? GlyphFlag::SA_MIRFD : GlyphFlag::SA_MIRBD) : GlyphFlag::NONE
			);

			if (possibilities.size() == 0) {
				glyphGrid = std::nullopt;
				return;
			}

			newGlyphs[i][j] = possibilities[rand() % possibilities.size()]; // Pick a random glyph, now that we know it has a nonzero size
			if (!firstInRow) {
				if (fwdDiag) newGlyphs[-j + jMin + iMax][-i + iMax + jMin] = newGlyphs[i][j]->mirroredFD;
				else		 newGlyphs[ j - jMin + iMin][ i - iMin + jMin] = newGlyphs[i][j]->mirroredBD;
			}
			else firstInRow = false;
		}
	}

	/// At the end of the function, set \c glyphGrid equal to the created copy.
	glyphGrid = newGlyphs;
}
inline bool Knot::inSelection(ijSignature, const int i, const int j) {
	return (i >= iMin) && (i <= iMax) && (j >= jMin) && (j <= jMax);
}
inline bool Knot::isEvenSegments(const int min, const int max) {
	return (max - min + 1) % 2 == 0;
}
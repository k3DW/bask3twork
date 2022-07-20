#include "Knot.h"

Knot::Knot(int h, int w, wxStatusBar* statusBar) : h(h), w(w), statusBar(statusBar), glyphs(GlyphVec2(h, GlyphVec1(w, DefaultGlyph))) {}
Knot::Knot(GlyphVec2 glyphs, wxStatusBar* statusBar) : h(glyphs.size()), w(glyphs[0].size()), statusBar(statusBar), glyphs(glyphs) {}
wxString Knot::get(const int i, const int j) const { return glyphs[i][j]->chr; }

bool Knot::generate(Symmetry sym, ijSignature)
/** Generate a knot with the given symmetry in the given selection.
 *
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained.
 * This function does not check the boundary symmetries to determine whether the specified symmetry is allowed.
 * The knot will generate with undefined behaviour if the proper symmetry conditions are not met in the first place.
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
	/// First, instantiate a \c wxString to show in the status bar,
	/// then add to it in a \c switch statement for the specific symmetry used.
	/// If the value for \c sym is not valid, this \c switch statement will cause the function to return \c false.
	wxString statusBeginning = "Generating ";
	switch (sym) {
		case Symmetry::NoSym:		{ statusBeginning += "no symmetry... ";						break; }
		case Symmetry::HoriSym:		{ statusBeginning += "horizontal symmetry... ";				break; }
		case Symmetry::VertSym:		{ statusBeginning += "vertical symmetry... ";				break; }
		case Symmetry::HoriVertSym: { statusBeginning += "horizontal + vertical symmetry... ";	break; }
		case Symmetry::Rot2Sym:		{ statusBeginning += "2-way rotational symmetry... ";		break; }
		case Symmetry::Rot4Sym:		{ statusBeginning += "4-way rotational symmetry... ";		break; }
		case Symmetry::FwdDiag:		{ statusBeginning += "forward diagonal symmetry... ";		break; }
		case Symmetry::BackDiag:	{ statusBeginning += "backward diagonal symmetry... ";		break; }
		case Symmetry::FullSym:		{ statusBeginning += "full symmetry... ";					break; }
		default:
			return false;
	}

	/// Then, make a copy of \c glyphs, and set all members in the selection to \c nullptr to denote that they are unassigned.
	GlyphVec2 baseGlyphs = glyphs;
	for (int i = iMin; i <= iMax; i++)
		for (int j = jMin; j <= jMax; j++)
			baseGlyphs[i][j] = nullptr;

	/// Next, enter a loop, counting the number of attempts made at generating this knot. The steps are as follows.
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		/// \b (1) At certain intervals of numbers of attempts, update the status bar with the number of attempts made.
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText(statusBeginning + "Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		/// \b (2) Call Knot::tryGenerating() using the copy of \c glyphs created above.
		///		If it fails, \c continue the loop and try again.
		std::optional<GlyphVec2> newGlyphs = tryGenerating(baseGlyphs, sym, iMin, jMin, iMax, jMax);
		if (!newGlyphs) continue;

		/// \b (3) If the knot has been successfully generated, set \c glyphs equal to this generated version and return \c true.
		glyphs = *newGlyphs;
		return true;
	}
	/// \b (4) If this loop has been completed, then the maximum number of attempts have been tried. Therefore return \c false.
	return false;
}

std::optional<GlyphVec2> Knot::tryGenerating(GlyphVec2 glyphGrid, Symmetry sym, ijSignature) const
/** Called only from Knot::generate(), try generating a knot with the given symmetry for the given selection.
 * 
 * This function pulls the required logic in Knot::generate() in order to generate the Knot selection once, and places it into its own function. 
 *
 * The same conditions apply as in the first paragraph of Knot::generate().
 *
 * The \c glyphGrid variable is supplied from Knot::generate(), already initialized with \c nullptr entries for the selection.
 *
 * \b Method
 */
{
	/// First, start by creating constant \c bool and \c int values to use in the generating.
	/// This includes deconstructing the \c Symmetry into its component bit flags,
	/// storing whether the selection compasses all rows or columns in the Knot,
	/// storing the middle \c i and \c j indices of the selection,
	/// and storing the wrapping conditions.
	const bool bitHori = static_cast<bool>(sym & 0x01);
	const bool bitVert = static_cast<bool>(sym & 0x02);
	const bool bitRot2 = static_cast<bool>(sym & 0x04);
	const bool bitRot4 = static_cast<bool>(sym & 0x08);
	const bool bitFwDi = static_cast<bool>(sym & 0x10);
	const bool bitBkDi = static_cast<bool>(sym & 0x20);

	const bool isEvenRows = (iMax - iMin + 1) % 2 == 0;
	const bool isEvenCols = (jMax - jMin + 1) % 2 == 0;
	const bool isAllRows = iMin == 0 && iMax == h - 1;
	const bool isAllCols = jMin == 0 && jMax == w - 1;
	const int iMid = (iMin + iMax) / 2;
	const int jMid = (jMin + jMax) / 2;
	const GlyphFlag midHoriFlag = isEvenRows ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const GlyphFlag midVertFlag = isEvenCols ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	const GlyphFlag midRot2Flag = isEvenRows ? (isEvenCols ? GlyphFlag::NONE : GlyphFlag::CT_ROT2D) : (isEvenCols ? GlyphFlag::CT_ROT2R : GlyphFlag::SA_ROT2);
	
	const bool isSquare = bitRot4 || bitFwDi || bitBkDi;
	const GlyphFlag midRot4Flag = isSquare ? (isEvenRows ? GlyphFlag::CT_ROT4R : !isEvenRows ? GlyphFlag::SA_ROT4 : GlyphFlag::NONE) : GlyphFlag::NONE;
	const GlyphFlag selfFlag = (h == 1 ? GlyphFlag::CT_SELFD : GlyphFlag::NONE) | (w == 1 ? GlyphFlag::CT_SELFR : GlyphFlag::NONE); // If this selection is only 1 row in length in either direction, flag appropriately
	
	const bool doWrapX = wrapXEnabled && (!isSquare || wrapYEnabled);
	const bool doWrapY = wrapYEnabled && (!isSquare || wrapXEnabled);

	/// Next, enter a loop over each location in the selection. Do the following.
	for (int i = iMin, iOffset = 0; i <= iMax; i++, iOffset++) {
		for (int j = jMin, jOffset = 0; j <= jMax; j++, jOffset++) {
			/// \b (1) If the Glyph in this location is already set, \c continue the loop.
			if (glyphGrid[i][j]) continue;

			/// \b (2) If the Glyph has yet to be set, generate a RandomGlyph() for this location, with the \c GlyphFlag::NONE flag.
			///	For each of the 4 \c Connection parameters, there are many possible cases, implemented in a large nested ternary operation, described below.
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
				i == 0		? (!doWrapY ? Connection::EMPTY : !glyphGrid[h - 1][j]	? Connection::DO_NOT_CARE : glyphGrid[h - 1][j]->down)	: (!glyphGrid[i - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i - 1][j]->down	) ,
				i == h - 1	? (!doWrapY ? Connection::EMPTY : !glyphGrid[0][j]		? Connection::DO_NOT_CARE : glyphGrid[0][j]->up)		: (!glyphGrid[i + 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i + 1][j]->up		) ,
				j == 0		? (!doWrapX ? Connection::EMPTY : !glyphGrid[i][w - 1]	? Connection::DO_NOT_CARE : glyphGrid[i][w - 1]->right) : (!glyphGrid[i][j - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j - 1]->right	) ,
				j == w - 1	? (!doWrapX ? Connection::EMPTY : !glyphGrid[i][0]		? Connection::DO_NOT_CARE : glyphGrid[i][0]->left)		: (!glyphGrid[i][j + 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j + 1]->left	) ,
				/** The \c boolFlags argument in RandomGlyph() has different components added, under various conditions. 
				 *  (a) If this type of symmetry includes horizontal reflection, then add \c GlyphFlag::CT_MIRU, only if the selection encompasses all rows and if the current location is in the uppermost row of the Knot.
				 *  (b) If this type of symmetry includes horizontal reflection but this time the current operation is in the middle row, then add either \c GlyphFlag::CT_MIRD or \c GlyphFlag::SA_MIRX depending on parity.
				 *  (c) If this type of symmetry includes vertical reflection, then add \c GlyphFlag::CT_MIRL, only if the selection encompasses all columns and if the current location is in the leftmost column of the Knot.
				 *  (d) If this type of symmetry includes vertical reflection but this time the current operation is in the middle column, then add either \c GlyphFlag::CT_MIRR or \c GlyphFlag::SA_MIRY depending on parity.
				 *  (e) If this type of symmetry includes 2-way rotation and the current operation is in the middle row and middle column, then add either \c GlyphFlag::CT_ROT2D, \c GlyphFlag::CT_ROT2R, or \c GlyphFlag::SA_ROT2, depending on the parities.
				 *  (f) If this type of symmetry includes 4-way rotation and the current operation is in the middle row and middle column, then add either \c GlyphFlag::CT_ROT4R or \c GlyphFlag::SA_ROT4 depending on the parity.
				 *  (g) If this type of symmetry includes back diagonal reflection, then add \c GlyphFlag::SA_MIRBD, only if the \c i offset and the \c j offset are equal, meaning that the location is along the diagonal.
				 *  (h) If this selection is only 1 row in height, add GlyphFlag::CT_SELFD.
				 *  (i) If this selection is only 1 column in width, add GlyphFlag::CT_SELFR.
				 */
				(
					(bitHori && isAllRows && i == 0 ? GlyphFlag::CT_MIRU : GlyphFlag::NONE) | 
					(bitHori && i == iMid ? midHoriFlag : GlyphFlag::NONE) | 
					(bitVert && isAllCols && j == 0 ? GlyphFlag::CT_MIRL : GlyphFlag::NONE) |
					(bitVert && j == jMid ? midVertFlag : GlyphFlag::NONE) |
					(bitRot2 && i == iMid && j == jMid ? midRot2Flag : GlyphFlag::NONE) |
					(bitRot4 && i == iMid && j == jMid ? midRot4Flag : GlyphFlag::NONE) |
					(bitBkDi && isSquare && iOffset == jOffset ? GlyphFlag::SA_MIRBD : GlyphFlag::NONE) |
					(selfFlag)
				)
			);

			/// \b (3) If this newly generated Glyph turns out to be \c nullptr, then there were no options for this location. Return \c std::nullopt.
			if (!glyphGrid[i][j]) return std::nullopt;

			/// \b (4) If the function has made it to this point, then reflect and rotate the newly generated Glyph to the appropriate spots given the symmetry required.
			if (bitHori) glyphGrid[iMax - iOffset][j] = glyphGrid[i][j]->mirroredX;
			if (bitVert) glyphGrid[i][jMax - jOffset] = glyphGrid[i][j]->mirroredY;
			if (bitRot2) glyphGrid[iMax - iOffset][jMax - jOffset] = glyphGrid[i][j]->rotated2;
			if (bitRot4) { glyphGrid[iMin + jOffset][jMax - iOffset] = glyphGrid[i][j]->rotated4; glyphGrid[iMax - jOffset][jMin + iOffset] = glyphGrid[i][j]->rotated2->rotated4; }
			if (bitFwDi) glyphGrid[iMax - jOffset][jMax - iOffset] = glyphGrid[i][j]->mirroredFD;
			if (bitBkDi) glyphGrid[iMin + jOffset][jMin + iOffset] = glyphGrid[i][j]->mirroredBD;
		}
	}

	/// \b (5) If the loop finishes, then the Knot has been successfully generated. Return \c true.
	return glyphGrid;
}

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

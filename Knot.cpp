#include "Knot.h"

Knot::Knot(int h, int w, wxStatusBar* statusBar) : h(h), w(w), statusBar(statusBar), glyphs(GlyphVec2(h, GlyphVec1(w, DefaultGlyph))) {}
wxString Knot::get(const int i, const int j) const {
	return glyphs[i][j]->chr;
}

bool Knot::generateNoSym(ijSignature) {
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating no symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;
		
		tryGenerating(newGlyphs, iMin, jMin, iMax, jMax);
		if (!newGlyphs) continue;
		
		glyphs = *newGlyphs;
		return true;
	}
	return false;
}
bool Knot::generateHoriSym(ijSignature) {
	const int iMid = (iMin + iMax) / 2;
	const GlyphFlag rowFlag = isEvenSegments(iMin, iMax) ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating horizontal symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		tryGenerating(newGlyphs, iMin, jMin, iMid - 1, jMax, Side::DOWN);
		if (!newGlyphs) continue;

		tryGenerating(newGlyphs, iMid, jMin, iMid, jMax, Side::DOWN, rowFlag);
		if (!newGlyphs) continue;

		mirrorUpToDown(*newGlyphs, iMin, jMin, iMax, jMax);

		glyphs = *newGlyphs;
		return true;
	}
	return false;
}
bool Knot::generateVertSym(ijSignature) {
	const int jMid = (jMin + jMax) / 2;
	const GlyphFlag colFlag = isEvenSegments(jMin, jMax) ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;
		
		tryGenerating(newGlyphs, iMin, jMin, iMax, jMid - 1, Side::RIGHT);
		if (!newGlyphs) continue;
		
		tryGenerating(newGlyphs, iMin, jMid, iMax, jMid, Side::RIGHT, colFlag);
		if (!newGlyphs) continue;
		
		mirrorLeftToRight(*newGlyphs, iMin, jMin, iMax, jMax);

		glyphs = *newGlyphs;
		return true;
	}
	return false;
}
bool Knot::generateHoriVertSym(ijSignature) {
	const int iMid = (iMin + iMax) / 2;
	const int jMid = (jMin + jMax) / 2;
	const GlyphFlag rowFlag = isEvenSegments(iMin, iMax) ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const GlyphFlag colFlag = isEvenSegments(jMin, jMax) ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical and horizontal symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		tryGenerating(newGlyphs, iMin, jMin, iMid - 1, jMid - 1, Side::DOWN | Side::RIGHT);
		if (!newGlyphs) continue;

		tryGenerating(newGlyphs, iMid, jMin, iMid, jMid - 1, Side::DOWN | Side::RIGHT, rowFlag);
		if (!newGlyphs) continue;

		tryGenerating(newGlyphs, iMin, jMid, iMid - 1, jMid, Side::DOWN | Side::RIGHT, colFlag);
		if (!newGlyphs) continue;

		tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::DOWN | Side::RIGHT, rowFlag | colFlag);
		if (!newGlyphs) continue;
		
		mirrorUpToDown(*newGlyphs, iMin, jMin, iMax, jMax);
		mirrorLeftToRight(*newGlyphs, iMin, jMin, iMax, jMax);

		glyphs = *newGlyphs;
		return true;
	}
	return false;
}
bool Knot::generateRot2Sym(ijSignature) {
	const int iMid = (iMin + iMax) / 2;
	const int jMid = (jMin + jMax) / 2;
	const bool isEvenRows = isEvenSegments(iMin, iMax);
	const bool isEvenCols = isEvenSegments(jMin, jMax);
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating 2-way rotational symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		tryGenerating(newGlyphs, iMin, jMin, iMax, jMid - 1, Side::RIGHT);
		if (!newGlyphs) continue;
		rotate180LeftToRight(*newGlyphs, iMin, jMin, iMax, jMax);
		
		tryGenerating(newGlyphs, iMin, jMid, iMid - 1, jMid + static_cast<int>(isEvenCols), Side::DOWN);
		if (!newGlyphs) continue;
		rotate180UpToDown(*newGlyphs, iMin, jMid, iMax, jMid + static_cast<int>(isEvenCols));

		if (!isEvenRows && !isEvenCols) {
			tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::NONE, GlyphFlag::SA_ROT2);
			if (!newGlyphs) continue;
		}
		else if (isEvenRows && !isEvenCols) {
			tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::DOWN, GlyphFlag::CT_ROT2D);
			if (!newGlyphs) continue;
			rotate180UpToDown(*newGlyphs, iMid, jMid, iMid + 1, jMid);
		}
		else if (!isEvenRows && isEvenCols) {
			tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::RIGHT, GlyphFlag::CT_ROT2R);
			if (!newGlyphs) continue;
			rotate180LeftToRight(*newGlyphs, iMid, jMid, iMid, jMid + 1);
		}
		else if (isEvenRows && isEvenCols) {
			tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::DOWN | Side::RIGHT);
			if (!newGlyphs) continue;
			rotate180UpToDown(*newGlyphs, iMid, jMid, iMid + 1, jMid + 1);
			tryGenerating(newGlyphs, iMid + 1, jMid, iMid + 1, jMid);
			if (!newGlyphs) continue;
			rotate180LeftToRight(*newGlyphs, iMid, jMid, iMid + 1, jMid + 1);
		}

		glyphs = *newGlyphs;
		return true;
	}
	return false;
}
bool Knot::generateRot4Sym(ijSignature) {
	const int iMid = (iMin + iMax) / 2;
	const int jMid = (jMin + jMax) / 2;
	const bool isEven = isEvenSegments(iMin, iMax);
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating 2-way rotational symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		tryGenerating(newGlyphs, iMin, jMin, iMid - 1, jMid - 1, Side::DOWN | Side::RIGHT);
		if (!newGlyphs) continue;
		rotate90FromUpLeft(*newGlyphs, iMin, jMin, iMax, jMax);
		
		tryGenerating(newGlyphs, iMin, jMid, iMid - 1, jMid + static_cast<int>(isEven), Side::DOWN);
		if (!newGlyphs) continue;
		rotate90FromUpLeft(*newGlyphs, iMin, jMin, iMax, jMax);

		if (isEven) {
			tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::DOWN | Side::RIGHT, GlyphFlag::CT_ROT4R);
			if (!newGlyphs) continue;
			rotate90FromUpLeft(*newGlyphs, iMin, jMin, iMax, jMax);
		}
		else {
			tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::NONE, GlyphFlag::SA_ROT4);
			if (!newGlyphs) continue;
		}

		glyphs = *newGlyphs;
		return true;
	}
	return false;
}

bool Knot::checkHoriSym(ijSignature) const {
	Connection upConnection, downConnection;

	// Checking the left and right sides
	if (!(jMin == 0 && jMax == w - 1)) {
		for (int iIncr = iMin, iDecr = iMax; iIncr < iDecr; iIncr++, iDecr--) { // for each row
			// Left side
			upConnection	= jMin == 0 ? Connection::EMPTY : glyphs[iIncr][jMin - 1]->right;
			downConnection	= jMin == 0 ? Connection::EMPTY : glyphs[iDecr][jMin - 1]->right;
			if (upConnection != mirXConnection(downConnection)) return false;
			// Right side
			upConnection	= jMax == w - 1 ? Connection::EMPTY : glyphs[iIncr][jMax + 1]->left;
			downConnection	= jMax == w - 1 ? Connection::EMPTY : glyphs[iDecr][jMax + 1]->left;
			if (upConnection != mirXConnection(downConnection)) return false;
		}
		if (!isEvenSegments(iMin, iMax)) { // checking the middle row, if it exists
			const int iMid = (iMin + iMax) / 2;
			// Left side
			upConnection = jMin == 0 ? Connection::EMPTY : glyphs[iMid][jMin - 1]->right;
			if (upConnection != mirXConnection(upConnection)) return false;
			// Right side
			upConnection = jMax == w - 1 ? Connection::EMPTY : glyphs[iMid][jMax + 1]->left;
			if (upConnection != mirXConnection(upConnection)) return false;
		}
	}

	// Checking the up and down sides
	if (!(iMin == 0 && iMax == h - 1))
		for (int j = jMin; j <= jMax; j++) { // for each column
			upConnection	= iMin == 0		? Connection::EMPTY : glyphs[iMin - 1][j]->down;
			downConnection	= iMax == h - 1	? Connection::EMPTY : glyphs[iMax + 1][j]->up;
			if (upConnection != mirXConnection(downConnection)) return false;
		}

	return true;
}
bool Knot::checkVertSym(ijSignature) const {
	Connection leftConnection, rightConnection;

	// Checking the up and down sides
	if (!(iMin == 0 && iMax == h - 1)) {
		for (int jIncr = jMin, jDecr = jMax; jIncr < jDecr; jIncr++, jDecr--) { // for each column
			// Up side
			leftConnection	= iMin == 0		? Connection::EMPTY : glyphs[iMin - 1][jIncr]->down;
			rightConnection = iMin == 0		? Connection::EMPTY : glyphs[iMin - 1][jDecr]->down;
			if (leftConnection != mirYConnection(rightConnection)) return false;
			// Down side
			leftConnection	= iMax == h - 1 ? Connection::EMPTY : glyphs[iMax + 1][jIncr]->up;
			rightConnection = iMax == h - 1 ? Connection::EMPTY : glyphs[iMax + 1][jDecr]->up;
			if (leftConnection != mirYConnection(rightConnection)) return false;
		}
		if (!isEvenSegments(jMin, jMax)) { // checking the middle column, if it exists
			const int jMid = (jMin + jMax) / 2;
			// Up side
			leftConnection = iMin == 0 ? Connection::EMPTY : glyphs[iMin - 1][jMid]->down;
			if (leftConnection != mirYConnection(leftConnection)) return false;
			// Down side
			leftConnection = iMax == h - 1 ? Connection::EMPTY : glyphs[iMax + 1][jMid]->up;
			if (leftConnection != mirYConnection(leftConnection)) return false;
		}
	}

	// Checking the left and right sides
	if (!(jMin == 0 && jMax == w - 1))
		for (int i = iMin; i <= iMax; i++) { // for each row
			leftConnection	= jMin == 0		? Connection::EMPTY : glyphs[i][jMin - 1]->right;
			rightConnection = jMax == w - 1	? Connection::EMPTY : glyphs[i][jMax + 1]->left;
			if (leftConnection != mirYConnection(rightConnection)) return false;
		}

	return true;
}
bool Knot::checkRot2Sym(ijSignature) const {
	Connection upConnection, downConnection;
	// Checking the up and down sides
	if (!(iMin == 0 && iMax == h - 1))
		for (int jIncr = jMin, jDecr = jMax; jIncr <= jMax; jIncr++, jDecr--) { // for each column
			upConnection	= iMin == 0		? Connection::EMPTY : glyphs[iMin - 1][jIncr]->down;
			downConnection	= iMax == h - 1 ? Connection::EMPTY : glyphs[iMax + 1][jDecr]->up;
			if (upConnection != rot2Connection(downConnection)) return false;
		}

	Connection leftConnection, rightConnection;
	// Checking the left and right sides
	if (!(jMin == 0 && jMax == w - 1))
		for (int iIncr = iMin, iDecr = iMax; iIncr <= iMax; iIncr++, iDecr--) { // for each row
			leftConnection	= jMin == 0		? Connection::EMPTY : glyphs[iIncr][jMin - 1]->right;
			rightConnection = jMax == w - 1 ? Connection::EMPTY : glyphs[iDecr][jMax + 1]->left;
			if (leftConnection != rot2Connection(rightConnection)) return false;
		}

	return true;
}
bool Knot::checkRot4Sym(ijSignature) const {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square

	Connection upConnection, downConnection, leftConnection, rightConnection;
	for (int offset = 0; offset <= iMax - iMin; offset++) {
		upConnection	= iMin == 0		? Connection::EMPTY : glyphs[iMin - 1][jMin + offset]->down;
		downConnection	= iMax == h - 1	? Connection::EMPTY : glyphs[iMax + 1][jMax - offset]->up;
		leftConnection	= jMin == 0		? Connection::EMPTY : glyphs[iMax - offset][jMin - 1]->right;
		rightConnection = jMax == w - 1 ? Connection::EMPTY : glyphs[iMin + offset][jMax + 1]->left;
		if (upConnection != rot4Connection(leftConnection) ||
			upConnection != rot2Connection(downConnection) ||
			upConnection != rot2Connection(rot4Connection(rightConnection))) return false;
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

	for (int iOffset = 0; iOffset <= iMid - iMin - static_cast<int>(isEven); iOffset++)
		for (int jOffset = 0; jOffset <= jMid - jMin - static_cast<int>(isEven); jOffset++) {
			glyphGrid[iMin + jOffset][jMax - iOffset] = glyphGrid[iMin + iOffset][jMin + jOffset]->rotated4;
			glyphGrid[iMax - iOffset][jMax - jOffset] = glyphGrid[iMin + iOffset][jMin + jOffset]->rotated2; // Same as ->rotated4->rotated4
			glyphGrid[iMax - jOffset][jMin + iOffset] = glyphGrid[iMin + iOffset][jMin + jOffset]->rotated2->rotated4; // Same as ->rotated4->rotated4->rotated4
		}

	for (int iOffset = 0; iOffset <= iMid - iMin; iOffset++)
		for (int jOffset = 0; jOffset <= static_cast<int>(isEven); jOffset++) {
			glyphGrid[iMid + jOffset][jMax - iOffset] = glyphGrid[iMin + iOffset][jMid + jOffset]->rotated4;
			glyphGrid[iMax - iOffset][jMid + static_cast<int>(isEven) - jOffset] = glyphGrid[iMin + iOffset][jMid + jOffset]->rotated2; // Same as ->rotated4->rotated4
			glyphGrid[iMid + static_cast<int>(isEven) - jOffset][jMin + iOffset] = glyphGrid[iMin + iOffset][jMid + jOffset]->rotated2->rotated4; // Same as ->rotated4->rotated4->rotated4
		}
}

void Knot::tryGenerating(std::optional<GlyphVec2>& glyphGrid, ijSignature, const Side ignoreSides, const GlyphFlag boolFlags) const {
	/// First, make a copy of \c glyphGrid to make changes without affecting the input.
	GlyphVec2 newGlyphs = *glyphGrid;
	
	/// Then, set the \c const \c Glyph* pointers of the copy to \c nullptr, if they are within the selection.
	/// This signifies that these values are not set, since they are being regenerated in this function.
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
				/** (1) Generate the \c vector of all possible glyphs that can fit into this spot, using the \c PossibleGlyphs() function,
				 *	passing on the \c boolFlags parameter directly.
				 *	For each \c Connection parameter, there are 4 possible cases, in order: 
				 *	(a) The side should be ignored based on \c ignoreSide, therefore the parameter should be \c Connection::DO_NOT_CARE.
				 *	(b) This glyph is on the outer edge of the selection on this particular side, so the parameter should be \c Connection::EMPTY.
				 *	(c) The next glyph on this particular side is not yet assigned, therefore the parameter should be \c Connection::DO_NOT_CARE.
				 *	(d) The general case, where none of the other special cases apply, therefore the parameter should be
				 *		the connection on the opposite side, from the neighbouring glyph on this particular side.
				 */
				ignoreUp	&& i == iMin ? Connection::DO_NOT_CARE : i == 0		? Connection::EMPTY : !newGlyphs[i - 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i - 1][j]->down,
				ignoreDown	&& i == iMax ? Connection::DO_NOT_CARE : i == h - 1	? Connection::EMPTY : !newGlyphs[i + 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i + 1][j]->up,
				ignoreLeft	&& j == jMin ? Connection::DO_NOT_CARE : j == 0		? Connection::EMPTY : !newGlyphs[i][j - 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j - 1]->right,
				ignoreRight && j == jMax ? Connection::DO_NOT_CARE : j == w - 1	? Connection::EMPTY : !newGlyphs[i][j + 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j + 1]->left,
				boolFlags
			);
			
			/// (2) If there are no possible glyphs for this space, set \c glyphGrid to the null \c optional and end the function.
			if (possibilities.size() == 0) {
				glyphGrid = std::nullopt;
				return;
			}
			
			/// (3) If step 2 did not end the function, set the glyph in the copy as a random choice from the generated possibilities.
			newGlyphs[i][j] = possibilities[rand() % possibilities.size()]; // Pick a random glyph, now that we know it has a nonzero size
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
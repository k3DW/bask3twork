#include "Knot.h"

Knot::Knot(int h, int w, wxStatusBar* statusBar) : h(h), w(w), statusBar(statusBar), glyphs(GlyphVec2(h, GlyphVec1(w, DefaultGlyph))) {}
Knot::Knot(GlyphVec2 glyphs, wxStatusBar* statusBar) : h(glyphs.size()), w(glyphs[0].size()), statusBar(statusBar), glyphs(glyphs) {}
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

	const GlyphFlag wrapYFlag = (wrapYEnabled && iMin == 0 && iMax == h - 1) ? GlyphFlag::CT_MIRU : GlyphFlag::NONE;
	const Side wrapYSide = (wrapYEnabled && iMin == 0 && iMax == h - 1) ? Side::UP : Side::NONE;

	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating horizontal symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		// Top row
		tryGenerating(newGlyphs, iMin, jMin, iMin, jMax, Side::DOWN | wrapYSide, wrapYFlag);
		if (!newGlyphs) continue;

		// Intermediate rows
		tryGenerating(newGlyphs, iMin + 1, jMin, iMid - 1, jMax, Side::DOWN);
		if (!newGlyphs) continue;
		
		// Middle row
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

	const GlyphFlag wrapXFlag = (wrapXEnabled && jMin == 0 && jMax == w - 1) ? GlyphFlag::CT_MIRL : GlyphFlag::NONE;
	const Side wrapXSide = (wrapXEnabled && jMin == 0 && jMax == w - 1) ? Side::LEFT : Side::NONE;

	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		// Left column
		tryGenerating(newGlyphs, iMin, jMin, iMax, jMin, Side::RIGHT | wrapXSide, wrapXFlag);
		if (!newGlyphs) continue;

		// Intermediate columns
		tryGenerating(newGlyphs, iMin, jMin + 1, iMax, jMid - 1, Side::RIGHT);
		if (!newGlyphs) continue;

		// Middle column
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
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square
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
bool Knot::generateFwdDiag(ijSignature) {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating forward diagonal symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		tryGeneratingDiag(newGlyphs, iMin, jMin, iMax, jMax, true);
		if (!newGlyphs) continue;

		glyphs = *newGlyphs;
		return true;
	}
	return false;
}
bool Knot::generateBackDiag(ijSignature) {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating backward diagonal symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		tryGeneratingDiag(newGlyphs, iMin, jMin, iMax, jMax, false);
		if (!newGlyphs) continue;

		glyphs = *newGlyphs;
		return true;
	}
	return false;
}
bool Knot::generateFullSym(ijSignature) {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square
	const int iMid = (iMin + iMax) / 2;
	const int jMid = (jMin + jMax) / 2;
	const bool isEven = isEvenSegments(iMin, iMax);
	const GlyphFlag colFlag = isEven ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating full symmetry... Attempt " + intWX(attempts) + "/" + intWX(MAX_ATTEMPTS));

		std::optional<GlyphVec2> newGlyphs = glyphs;

		tryGeneratingDiag(newGlyphs, iMin, jMin, iMid - 1, jMid - 1, false, Side::DOWN | Side::RIGHT);
		if (!newGlyphs) continue;

		tryGenerating(newGlyphs, iMin, jMid, iMid - 1, jMid, Side::DOWN | Side::RIGHT, colFlag);
		if (!newGlyphs) continue;
		mirrorLeftToRight(*newGlyphs, iMin, jMin, iMax, jMax);
		rotate90FromUpLeft(*newGlyphs, iMin, jMin, iMax, jMax);

		if (isEven) {
			tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::DOWN | Side::RIGHT, GlyphFlag::SA_MIRBD | GlyphFlag::CT_MIRR | GlyphFlag::CT_MIRD | GlyphFlag::CT_ROT4R);
			if (!newGlyphs) continue;
			rotate90FromUpLeft(*newGlyphs, iMid, jMid, iMid + 1, jMid + 1);
		}
		else {
			tryGenerating(newGlyphs, iMid, jMid, iMid, jMid, Side::NONE, GlyphFlag::SA_MIRBD | GlyphFlag::SA_ROT4);
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
		upConnection	= iMin == 0		? Connection::EMPTY : glyphs[iMin - 1][jMin + offset]->down;	// Top row, from left to right
		downConnection	= iMax == h - 1	? Connection::EMPTY : glyphs[iMax + 1][jMax - offset]->up;		// Bottom row, from right to left
		leftConnection	= jMin == 0		? Connection::EMPTY : glyphs[iMax - offset][jMin - 1]->right;	// Left column, from bottom to top
		rightConnection = jMax == w - 1 ? Connection::EMPTY : glyphs[iMin + offset][jMax + 1]->left;	// Right column, from top to bottom
		if (upConnection != rot4Connection(leftConnection) ||
			upConnection != rot2Connection(downConnection) ||
			upConnection != rot2Connection(rot4Connection(rightConnection))) return false;
	}

	return true;
}
bool Knot::checkFwdDiag(ijSignature) const {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square

	Connection upConnection, downConnection, leftConnection, rightConnection;
	for (int offset = 0; offset <= iMax - iMin; offset++) {
		upConnection	= iMin == 0		? Connection::EMPTY : glyphs[iMin - 1][jMin + offset]->down;	// Top row, from left to right
		downConnection	= iMax == h - 1	? Connection::EMPTY : glyphs[iMax + 1][jMin + offset]->up;		// Bottom row, from left to right
		leftConnection	= jMin == 0		? Connection::EMPTY : glyphs[iMax - offset][jMin - 1]->right;	// Left column, from bottom to top
		rightConnection = jMax == w - 1	? Connection::EMPTY : glyphs[iMax - offset][jMax + 1]->left;	// Right column, from bottom to top
		if (upConnection != mirFDConnection(rightConnection) || leftConnection != mirFDConnection(downConnection))
			return false;
	}

	return true;
}
bool Knot::checkBackDiag(ijSignature) const {
	if (iMax - iMin != jMax - jMin) return false; // The selection must be square

	Connection upConnection, downConnection, leftConnection, rightConnection;
	for (int offset = 0; offset <= iMax - iMin; offset++) {
		upConnection	= iMin == 0		? Connection::EMPTY : glyphs[iMin - 1][jMin + offset]->down;	// Top row, from left to right
		downConnection	= iMax == h - 1 ? Connection::EMPTY : glyphs[iMax + 1][jMin + offset]->up;		// Bottom row, from left to right
		leftConnection	= jMin == 0		? Connection::EMPTY : glyphs[iMin + offset][jMin - 1]->right;	// Left column, from top to bottom
		rightConnection = jMax == w - 1 ? Connection::EMPTY : glyphs[iMin + offset][jMax + 1]->left;	// Right column, from top to bottom
		if (upConnection != mirBDConnection(leftConnection) || rightConnection != mirBDConnection(downConnection))
			return false;
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
				ignoreUp	&& i == iMin ? Connection::DO_NOT_CARE : i == 0		? Connection::EMPTY : !newGlyphs[i - 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i - 1][j]->down,
				ignoreDown	&& i == iMax ? Connection::DO_NOT_CARE : i == h - 1 ? Connection::EMPTY : !newGlyphs[i + 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i + 1][j]->up,
				ignoreLeft	&& j == jMin ? Connection::DO_NOT_CARE : j == 0		? Connection::EMPTY : !newGlyphs[i][j - 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j - 1]->right,
				ignoreRight && j == jMax ? Connection::DO_NOT_CARE : j == w - 1 ? Connection::EMPTY : !newGlyphs[i][j + 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j + 1]->left,
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
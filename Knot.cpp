#include "Knot.h"

Knot::Knot(int h, int w, wxStatusBar* statusBar) : h(h), w(w), statusBar(statusBar) {
	this->glyphs = GlyphVec2(h, GlyphVec1(w, DefaultGlyph));
}
wxString Knot::get(const int i, const int j) const {
	return glyphs[i][j]->chr;
}

bool Knot::generateNoSym(ijSignature) {
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating no symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);

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
	const unsigned int rowFlag = isEvenSegments(iMin, iMax) ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating horizontal symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);

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
	const unsigned int colFlag = isEvenSegments(jMin, jMax) ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);

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
	const unsigned int rowFlag = isEvenSegments(iMin, iMax) ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const unsigned int colFlag = isEvenSegments(jMin, jMax) ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical and horizontal symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);

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
	const unsigned int rowFlag = isEvenRows ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const unsigned int colFlag = isEvenCols ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating 2-way rotational symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);

		std::optional<GlyphVec2> newGlyphs = glyphs;

		tryGenerating(newGlyphs, iMin, jMin, iMax, jMid - 1, Side::RIGHT);
		if (!newGlyphs) continue;
		rotate180LeftToRight(*newGlyphs, iMin, jMin, iMax, jMax);
		
		tryGenerating(newGlyphs, iMin, jMid, iMid - 1, jMid + (int)isEvenCols, Side::DOWN);
		if (!newGlyphs) continue;
		rotate180UpToDown(*newGlyphs, iMin, jMid, iMax, jMid + (int)isEvenCols);

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

void Knot::tryGenerating(std::optional<GlyphVec2>& glyphGrid, ijSignature, const int ignoreSides, const int boolFlags) const {
	/*	This function generates a knot with no symmetry, lining up with the borders of the selection (unless ignoreSide tells it to be ignored)
	 *	The function changes `glyphIndices`, which may need to be reversed in the outer function that calls it.
	 */
	GlyphVec2 newGlyphs = *glyphGrid;

	// Make the `const Glyph*` pointers into `nullptr` if they're in the selection
	for (int i = iMin; i <= iMax; i++)
		for (int j = jMin; j <= jMax; j++)
			if (inSelection(iMin, jMin, iMax, jMax, i, j))
				newGlyphs[i][j] = nullptr;

	const bool ignoreUp = ignoreSides & Side::UP;
	const bool ignoreDown = ignoreSides & Side::DOWN;
	const bool ignoreLeft = ignoreSides & Side::LEFT;
	const bool ignoreRight = ignoreSides & Side::RIGHT;

	// Generate the glyphs
	for (int i = iMin; i <= iMax; i++) {
		for (int j = jMin; j <= jMax; j++) {
			GlyphVec1 possibilities = PossibleGlyphs(
				/* Cases for each Connection::Type parameter
				 *	(1) The side should be ignored		-> DO_NOT_CARE
				 *	(2) The glyph is on a border		-> EMPTY
				 *	(3) The next glyph is not assigned	-> DO_NOT_CARE
				 *	(4) Catchall default				-> The connection of the next glyph in that direction
				 * This is done for each of the four Connection::Type arguments
				 */
				ignoreUp	&& i == iMin ? Connection::DO_NOT_CARE : i == 0		? Connection::EMPTY : !newGlyphs[i - 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i - 1][j]->down,
				ignoreDown	&& i == iMax ? Connection::DO_NOT_CARE : i == h - 1	? Connection::EMPTY : !newGlyphs[i + 1][j] ? Connection::DO_NOT_CARE : newGlyphs[i + 1][j]->up,
				ignoreLeft	&& j == jMin ? Connection::DO_NOT_CARE : j == 0		? Connection::EMPTY : !newGlyphs[i][j - 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j - 1]->right,
				ignoreRight && j == jMax ? Connection::DO_NOT_CARE : j == w - 1	? Connection::EMPTY : !newGlyphs[i][j + 1] ? Connection::DO_NOT_CARE : newGlyphs[i][j + 1]->left,
				boolFlags
			);
			if (possibilities.size() == 0) {	// If there are no possible glyphs that can go here, return false
				glyphGrid = std::nullopt;
				return;
			}
			newGlyphs[i][j] = pick_random(possibilities);
		}
	}
	glyphGrid = newGlyphs;
}
inline bool Knot::inSelection(ijSignature, const int i, const int j) {
	return (i >= iMin) && (i <= iMax) && (j >= jMin) && (j <= jMax);
}
inline bool Knot::isEvenSegments(const int min, const int max) {
	return (max - min + 1) % 2 == 0;
}
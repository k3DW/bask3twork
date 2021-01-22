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

		std::optional<GlyphVec2> newGlyphs = tryGeneratingNoSym(iMin, jMin, iMax, jMax);
		if (newGlyphs) {
			glyphs = *newGlyphs;
			return true;
		}
	}
	return false;
}

std::optional<GlyphVec2> Knot::tryGeneratingNoSym(ijSignature, const int ignoreSides, const int boolFlags) const {
	/*	This function generates a knot with no symmetry, lining up with the borders of the selection (unless ignoreSide tells it to be ignored)
	 *	The function changes `glyphIndices`, which may need to be reversed in the outer function that calls it.
	 */
	GlyphVec2 currentGlyphs = glyphs;

	// Make the `const Glyph*` pointers into `nullptr` if they're in the selection
	for (int i = iMin; i <= iMax; i++)
		for (int j = jMin; j <= jMax; j++)
			if (inSelection(iMin, jMin, iMax, jMax, i, j))
				currentGlyphs[i][j] = nullptr;

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
				ignoreUp	&& i == iMin ? Connection::DO_NOT_CARE : i == 0		? Connection::EMPTY : !currentGlyphs[i - 1][j] ? Connection::DO_NOT_CARE : currentGlyphs[i - 1][j]->down,
				ignoreDown	&& i == iMax ? Connection::DO_NOT_CARE : i == h - 1	? Connection::EMPTY : !currentGlyphs[i + 1][j] ? Connection::DO_NOT_CARE : currentGlyphs[i + 1][j]->up,
				ignoreLeft	&& j == jMin ? Connection::DO_NOT_CARE : j == 0		? Connection::EMPTY : !currentGlyphs[i][j - 1] ? Connection::DO_NOT_CARE : currentGlyphs[i][j - 1]->right,
				ignoreRight && j == jMax ? Connection::DO_NOT_CARE : j == w - 1	? Connection::EMPTY : !currentGlyphs[i][j + 1] ? Connection::DO_NOT_CARE : currentGlyphs[i][j + 1]->left,
				boolFlags
			);
			if (possibilities.size() == 0)	// If there are no possible glyphs that can go here, return false
				return std::nullopt;
			currentGlyphs[i][j] = pick_random(possibilities);
		}
	}
	return currentGlyphs;
}
inline bool Knot::inSelection(ijSignature, const int i, const int j) {
	return (i >= iMin) && (i <= iMax) && (j >= jMin) && (j <= jMax);
}
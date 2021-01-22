#pragma once
#include "Constants.h"
#include "Glyph.h"

class Knot {
public:
	Knot(int h, int w, wxStatusBar* statusBar);
	wxString get(const int i, const int j) const;

	const int h;
	const int w;
	wxStatusBar* const statusBar;

	bool generateNoSym(ijSignature);

private:
	GlyphVec2 glyphs;

	std::optional<GlyphVec2> tryGeneratingNoSym(ijSignature, const int ignoreSides = 0, const int boolFlags = 0) const;
	static inline bool inSelection(ijSignature, const int i, const int j);
};
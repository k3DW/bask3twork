#pragma once
#include "Constants.h"
#include "Glyph.h"

class Knot {
public:
	Knot(int h, int w, wxStatusBar* statusBar);
	wxString get(const int i, const int j) const;

	const int h, w;
	wxStatusBar* const statusBar;

	bool generateNoSym(ijSignature);
	bool generateHoriSym(ijSignature);
	bool generateVertSym(ijSignature);
	bool generateHoriVertSym(ijSignature);

private:
	GlyphVec2 glyphs;

	void mirrorX(ijSignature);
	void mirrorY(ijSignature);

	void tryGenerating(std::optional<GlyphVec2>& glyphGrid, ijSignature, const int ignoreSides = 0, const int boolFlags = 0) const;
	static inline bool inSelection(ijSignature, const int i, const int j);
	static inline bool isEvenSegments(const int min, const int max);
};
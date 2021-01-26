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
	bool generateRot2Sym(ijSignature);
	bool generateRot4Sym(ijSignature);

	bool checkHoriSym(ijSignature) const;
	bool checkVertSym(ijSignature) const;
	bool checkRot2Sym(ijSignature) const;
	bool checkRot4Sym(ijSignature) const;

private:
	GlyphVec2 glyphs;

	void mirrorUpToDown(GlyphVec2& glyphGrid, ijSignature) const;
	void mirrorLeftToRight(GlyphVec2& glyphGrid, ijSignature) const;
	void rotate180UpToDown(GlyphVec2& glyphGrid, ijSignature) const;
	void rotate180LeftToRight(GlyphVec2& glyphGrid, ijSignature) const;
	void rotate90FromUpLeft(GlyphVec2& glyphGrid, ijSignature) const;

	void tryGenerating(std::optional<GlyphVec2>& glyphGrid, ijSignature, const Side ignoreSides = Side::NONE, const GlyphFlag boolFlags = GlyphFlag::NONE) const;
	static inline bool inSelection(ijSignature, const int i, const int j);
	static inline bool isEvenSegments(const int min, const int max);
};
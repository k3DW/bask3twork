#pragma once
#include "Constants.h"
#include "Glyph.h"

#ifndef ijSignature
#define ijSignature const int iMin, const int jMin, const int iMax, const int jMax
#endif // !ijSignature

class Knot {
public:
	Knot(int h, int w, wxStatusBar* statusBar);
	const int h;
	const int w;
	wxStatusBar* const statusBar;

	inline wxString get(const int i, const int j) const;

	bool generateNoSym(ijSignature);

	// declareGenerateFunction(NoSym);
	// declareGenerateFunction(VertSym);
	// declareGenerateFunction(HoriSym);
	// declareGenerateFunction(VertHoriSym);
	// declareGenerateFunction(Rot2Sym);
	// declareGenerateFunction(Rot4Sym);

	// bool checkVertSym(ijSignature);
	// bool checkHoriSym(ijSignature);

private:
	GlyphVec2 glyphs;

	std::optional<GlyphVec2> tryGeneratingNoSym(ijSignature, const int ignoreSides = 0, const int boolFlags = 0);
	static inline bool inSelection(ijSignature, const int i, const int j);
};
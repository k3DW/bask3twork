#pragma once
#include "wx/wx.h"
#include "Constants.h"
#include "Glyph.h"
#include <vector>

#define ijSignature const int iMin, const int jMin, const int iMax, const int jMax

class Knot {

public:
	const int h;
	const int w;
	Knot() = default;
	Knot(int h, int w, wxStatusBar* statusBar);
	~Knot();

	wxString get(int i, int j);

	#define declareGenerateFunction(SymType) bool generate##SymType##(ijSignature)
	declareGenerateFunction(NoSym);
	declareGenerateFunction(VertSym);
	declareGenerateFunction(HoriSym);
	declareGenerateFunction(VertHoriSym);
	declareGenerateFunction(Rot2Sym);
	declareGenerateFunction(Rot4Sym);

	bool checkVertSym(ijSignature);
	bool checkHoriSym(ijSignature);

	//bool waveCollapseNoSym(const int selectNums[4]);

	//inline bool inSelection(const int selectNums[4], int i, int j);
	//inline bool onBoundary(int i, int j);

private:
	std::vector< std::vector<int> > glyphIndices;
	wxStatusBar* statusBar;

	bool** createAssignedPtr(ijSignature, bool diagonal = false);
	void deleteAssignedPtr(bool** assigned);

	bool tryGeneratingNoSym(ijSignature, const std::vector<int>& startingGlyphList, int ignoreSide = 0);
	void checkSide(std::vector<int>& glyphList, bool** assigned, int i, int j, Side side);
	inline void filterGlyphList(std::vector<int>& glyphList, ConnectionType type, Side side);
	inline std::vector<int> static set_intersection(const std::vector<int>& first, const std::vector<int>& second);
	inline void static set_intersection_inplace(std::vector<int>& first, const std::vector<int>& second);
	inline bool static isEvenSegments(const int min, const int max);
};

#pragma once
#include "wx/wx.h"
#include "Constants.h"
#include <vector>

class Knot {

public:
	Knot() = default;
	Knot(int h, int w, wxStatusBar* statusBar);
	~Knot();
	wxString get(int i, int j);

	#define declareGenerateFunction(SymType) bool generate##SymType##(const int selectNums[4])
	declareGenerateFunction(NoSym);
	declareGenerateFunction(VertSym);
	declareGenerateFunction(HoriSym);
	declareGenerateFunction(VertHoriSym);
	declareGenerateFunction(Rot2Sym);
	declareGenerateFunction(Rot4Sym);

	bool checkVertSym(const int selectNums[4]);
	bool checkHoriSym(const int selectNums[4]);

	bool waveCollapseNoSym(const int selectNums[4]);

	inline bool inSelection(const int selectNums[4], int i, int j);
	inline bool onBoundary(int i, int j);

private:
	int h, w;
	std::vector< std::vector<int> > glyphIndices;
	wxStatusBar* statusBar;

	bool** createAssignedPtr(const int selectNums[4], bool diagonal = false);
	void deleteAssignedPtr(bool** assigned);

	bool tryGeneratingNoSym(const int selectNums[4], const std::vector<int>& startingGlyphList, int ignoreSide = 0);
	void checkSide(std::vector<int>& glyphList, bool** assigned, int i, int j, Side side);
	inline void filterGlyphList(std::vector<int>& glyphList, ConnectionType type, Side side);
	inline std::vector<int> static set_intersection(const std::vector<int>& first, const std::vector<int>& second);
	inline void static set_intersection_inplace(std::vector<int>& first, const std::vector<int>& second);

public:
	const int& getH = h;
	const int& getW = w;

};
#if 0//ndef KNOTS_KNOTOLD
#define KNOTS_KNOTOLD

#include "Constants.h"

class KnotOld {

public:
	KnotOld(int h_, int w_, wxStatusBar* statusBar_);
	const int h;
	const int w;

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
	wxStatusBar* const statusBar;

	//int test;

	//std::vector<std::vector<const Glyph*>> knotGlyphs;

	bool** createAssignedPtr(ijSignature, bool diagonal = false);
	void deleteAssignedPtr(bool** assigned);

	//std::vector<Glyph*> possibleGlyphs(const int flags);

	bool tryGeneratingNoSym(ijSignature, const std::vector<int>& startingGlyphList, int ignoreSide = 0);
	void checkSide(std::vector<int>& glyphList, bool** assigned, int i, int j, SideOLD side);
	inline void filterGlyphList(std::vector<int>& glyphList, ConnectionType type, SideOLD side);
	inline std::vector<int> static set_intersection(const std::vector<int>& first, const std::vector<int>& second);
	inline void static set_intersection_inplace(std::vector<int>& first, const std::vector<int>& second);
	inline bool static isEvenSegments(const int min, const int max);
};

#endif // KNOTS_KNOTOLD
#include "Knot.h"

// Disabling warning C26812, which suggests to change `enum` to `enum class`. I want to keep Side and ConnectionType as `enum`.
#pragma warning( push )
#pragma warning( disable : 26812 )

/*
std::vector<int> selfFwdDiagonal,		 // all glyphs that are the same under a forward diagonal reflection
selfBackDiagonal;     // all glyphs that are the same under a backward diagonal reflection

void init() {
	for (int i = 0; i < NUM_GLYPHS; i++) {
		if (i == reflectYGlyphs[rotateGlyphs[i]]) selfBackDiagonal.push_back(i);
		if (i == reflectXGlyphs[rotateGlyphs[i]]) selfFwdDiagonal.push_back(i);
	}
}//*/

Knot::Knot(int h, int w, wxStatusBar* statusBar) {
	this->h = h;
	this->w = w;
	this->statusBar = statusBar;
	this->glyphIndices = std::vector<std::vector<int>>(h, std::vector<int>(w, 0)); // Set all indices to 0, which is the glyph index of the space "\x20"

	//this->test = std::vector<std::vector<int>>(h, std::vector<int>(w, 0));
	//this->knotGlyphs = std::vector<std::vector<Glyph>>(h, std::vector<Glyph>(w, allGlyphs[0]));
	//std::vector<std::vector<Glyph>> test(h, std::vector<Glyph>(w, allGlyphs[0]));

	/*
	for (int i = 0; i < h; i++) {
		knotGlyphs.push_back(std::vector<Glyph>());
		for (int j = 0; j < w; j++) {
			knotGlyphs[i].push_back(Glyph(allGlyphs[0]));
		}
	}//*/
}
Knot::~Knot() {
	glyphIndices.resize(0);
}
wxString Knot::get(int i, int j) {
	return glyphs[glyphIndices[i][j]];
}

bool Knot::generateNoSym(const int selectNums[4]) {
	const wxString oldStatus = statusBar->GetStatusText();
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating no symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(selectNums, allGlyphIndices))
			success = true;
		else
			glyphIndices = previousGlyphIndices;
	}

	statusBar->SetStatusText(oldStatus);
	return success;
}
bool Knot::generateVertSym(const int selectNums[4]) {
	const wxString oldStatus = statusBar->GetStatusText();
	const int* n = selectNums;
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	const bool isEvenNumberOfColumns = (n[3] - n[1] + 1) % 2 == 0;
	const std::vector<int> middleColumnStartingGlyphList = isEvenNumberOfColumns ? connectToReflectY : sameAfterReflectY;

	const int jMid = (n[3] + n[1] - 2) / 2; // the index of the middle column
	const int selectNumsLeftHalf[4] = { n[0], n[1], n[2], jMid };
	const int selectNumsMiddleColumn[4] = { n[0], jMid + 1, n[2], jMid + 1 };

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(selectNumsLeftHalf, allGlyphIndices, bRIGHT) &&
			tryGeneratingNoSym(selectNumsMiddleColumn, middleColumnStartingGlyphList, bRIGHT))
			success = true;
		else
			glyphIndices = previousGlyphIndices;
	}

	if (success) // success, therefore mirror the glyphs that need mirroring
		for (int i = n[0] - 1; i <= n[2] - 1; i++)
			for (int jIncr = n[1] - 1, jDecr = n[3] - 1; jIncr < jDecr; jIncr++, jDecr--)
				glyphIndices[i][jDecr] = reflectYGlyphs[glyphIndices[i][jIncr]];
	statusBar->SetStatusText(oldStatus);
	return success;
}
bool Knot::generateHoriSym(const int selectNums[4]) {
	const wxString oldStatus = statusBar->GetStatusText();
	const int* n = selectNums;
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	const bool isEvenNumberOfRows = (n[2] - n[0] + 1) % 2 == 0;
	const std::vector<int> middleRowStartingGlyphList = isEvenNumberOfRows ? connectToReflectX : sameAfterReflectX;

	const int iMid = (n[2] + n[0] - 2) / 2; // the index of the middle row
	const int selectNumsTopHalf[4] = { n[0], n[1], iMid, n[3] };
	const int selectNumsMiddleRow[4] = { iMid + 1, n[1], iMid + 1, n[3] };

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating horizontal symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(selectNumsTopHalf, allGlyphIndices, bBOTTOM) &&
			tryGeneratingNoSym(selectNumsMiddleRow, middleRowStartingGlyphList, bBOTTOM))
			success = true;
		else
			glyphIndices = previousGlyphIndices;
	}

	if (success) // success, therefore mirror the glyphs that need mirroring
		for (int iIncr = n[0] - 1, iDecr = n[2] - 1; iIncr < iDecr; iIncr++, iDecr--)
			for (int j = n[1] - 1; j <= n[3] - 1; j++)
				glyphIndices[iDecr][j] = reflectXGlyphs[glyphIndices[iIncr][j]];
	statusBar->SetStatusText(oldStatus);
	return success;
}
bool Knot::generateVertHoriSym(const int selectNums[4]) {
	const wxString oldStatus = statusBar->GetStatusText();
	const int* n = selectNums;
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	const bool isEvenNumberOfColumns = (n[3] - n[1] + 1) % 2 == 0;
	const std::vector<int> middleColumnStartingGlyphList = isEvenNumberOfColumns ? connectToReflectY : sameAfterReflectY;
	const bool isEvenNumberOfRows = (n[2] - n[0] + 1) % 2 == 0;
	const std::vector<int> middleRowStartingGlyphList = isEvenNumberOfRows ? connectToReflectX : sameAfterReflectX;
	const std::vector<int> middleSquareStartingGlyphList = set_intersection(middleColumnStartingGlyphList, middleRowStartingGlyphList);

	const int jMid = (n[3] + n[1] - 2) / 2; // the index of the middle column
	const int iMid = (n[2] + n[0] - 2) / 2; // the index of the middle row
	const int selectNumsMiddleColumn[4] = { n[0], jMid + 1, iMid, jMid + 1 };
	const int selectNumsMiddleRow[4] = { iMid + 1, n[1], iMid + 1, jMid };
	const int selectNumsMiddleSquare[4] = { iMid + 1, jMid + 1, iMid + 1, jMid + 1 };
	const int selectNumsUpperLeftQuarter[4] = { n[0], n[1], iMid, jMid };

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical + horizontal symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(selectNumsUpperLeftQuarter, allGlyphIndices, bRIGHT | bBOTTOM)			&&
			tryGeneratingNoSym(selectNumsMiddleColumn, middleColumnStartingGlyphList, bRIGHT | bBOTTOM)	&&
			tryGeneratingNoSym(selectNumsMiddleRow, middleRowStartingGlyphList, bRIGHT | bBOTTOM)		&&
			tryGeneratingNoSym(selectNumsMiddleSquare, middleSquareStartingGlyphList, bRIGHT | bBOTTOM))
			success = true;
		else
			glyphIndices = previousGlyphIndices;
	}

	if (success) { // success, therefore mirror the glyphs that need mirroring
		for (int i = n[0] - 1; i <= iMid; i++)
			for (int jIncr = n[1] - 1, jDecr = n[3] - 1; jIncr < jDecr; jIncr++, jDecr--)
				glyphIndices[i][jDecr] = reflectYGlyphs[glyphIndices[i][jIncr]];
		for (int iIncr = n[0] - 1, iDecr = n[2] - 1; iIncr < iDecr; iIncr++, iDecr--)
			for (int j = n[1] - 1; j <= n[3] - 1; j++)
				glyphIndices[iDecr][j] = reflectXGlyphs[glyphIndices[iIncr][j]];
	}
	statusBar->SetStatusText(oldStatus);
	return success;
}
bool Knot::generateRot2Sym(const int selectNums[4]) {
	const wxString oldStatus = statusBar->GetStatusText();
	const int* n = selectNums;
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	const bool isEvenNumberOfColumns = (n[3] - n[1] + 1) % 2 == 0;

	const int jMid = (n[3] + n[1] - 2) / 2; // the index of the middle column
	const int iMid = (n[2] + n[0] - 2) / 2; // the index of the middle row
	const int selectNumsLeftHalf[4] = { n[0], n[1], n[2], jMid };
	const int selectNumsMiddle[4] = { n[0], jMid + 1, iMid, jMid + 1 + isEvenNumberOfColumns };

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating 2-way rotational symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(selectNumsLeftHalf, allGlyphIndices, bRIGHT)) { // Generating the left half
			for (int iIncr = n[0] - 1, iDecr = n[2] - 1; iIncr <= n[2] - 1; iIncr++, iDecr--)
				for (int jIncr = n[1] - 1, jDecr = n[3] - 1; jIncr < jMid; jIncr++, jDecr--)
					glyphIndices[iDecr][jDecr] = rotateGlyphs[rotateGlyphs[glyphIndices[iIncr][jIncr]]]; // Rotate-mirroring the left half
			if (tryGeneratingNoSym(selectNumsMiddle, allGlyphIndices, bBOTTOM)) { // Generating the top section of the missing chunk
				for (int iIncr = n[0] - 1, iDecr = n[2] - 1; iIncr < iMid; iIncr++, iDecr--)
					for (int jIncr = jMid, jDecr = jMid + isEvenNumberOfColumns; jIncr <= jMid + isEvenNumberOfColumns; jIncr++, jDecr--)
						glyphIndices[iDecr][jDecr] = rotateGlyphs[rotateGlyphs[glyphIndices[iIncr][jIncr]]]; // Rotate-mirroring the generated top section

				success = true;
				//std::vector<int> glyphList(allGlyphIndices);

			}
		}
		if (!success)
			glyphIndices = previousGlyphIndices;
	}

	if (!success)
		glyphIndices = previousGlyphIndices;
	statusBar->SetStatusText(oldStatus);
	return success;
}

bool Knot::checkVertSym(const int selectNums[4]) {
	const int* n = selectNums;
	int iTop = n[0] - 1, iBottom = n[2] - 1;
	int jLeft = n[1] - 1, jRight = n[3] - 1;

	// Which of the sides of this selection are on the border of the knot
	bool TOPisBorder	= !(iTop - 1 >= 0);
	bool BOTTOMisBorder = !(iBottom + 1 < h);
	bool LEFTisBorder	= !(jLeft - 1 >= 0);
	bool RIGHTisBorder	= !(jRight + 1 < w);

	ConnectionType leftConnection, rightConnection;

	// This `for` loop checks the left and right edges of the selection, from top to bottom.
	for (int i = iTop; i <= iBottom; i++) {
		// If the left or right side is on the border, then it requires `ConnectionType EMPTY`, otherwise it requires the external ConnectionType
		leftConnection  =  LEFTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[i][jLeft - 1]][RIGHT];
		rightConnection = RIGHTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[i][jRight + 1]][LEFT];
		// If the left connection and the right connection are not equivalent, then this selection does not have vertical symmetry
		if (leftConnection != reflectYTypes[rightConnection])
			return false;
	}

	// This `for` loop checks the top and bottom edges of the selection (in that order, for each column), working outside-in
	for (int jIncr = jLeft, jDecr = jRight; jIncr < jDecr; jIncr++, jDecr--) {
		// If the top side is on the border, then they both require `ConnectionType EMPTY`, otherwise they require the external ConnectionType
		leftConnection  = TOPisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iTop - 1][jIncr]][BOTTOM];
		rightConnection = TOPisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iTop - 1][jDecr]][BOTTOM];
		// If the left connection and the right connection are not equivalent, then this selection does not have vertical symmetry
		if (leftConnection != reflectYTypes[rightConnection])
			return false;

		// If the bottom side is on the border, then they both require `ConnectionType EMPTY`, otherwise they require the external ConnectionType
		leftConnection  = BOTTOMisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iBottom + 1][jIncr]][TOP];
		rightConnection = BOTTOMisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iBottom + 1][jDecr]][TOP];
		// If the left connection and the right connection are not equivalent, then this selection does not have vertical symmetry
		if (leftConnection != reflectYTypes[rightConnection])
			return false;
	}

	return true;
}
bool Knot::checkHoriSym(const int selectNums[4]) {
	const int* n = selectNums;
	int iTop = n[0] - 1, iBottom = n[2] - 1;
	int jLeft = n[1] - 1, jRight = n[3] - 1;

	// Which of the sides of this selection are on the border of the knot
	bool TOPisBorder = !(iTop - 1 >= 0);
	bool BOTTOMisBorder = !(iBottom + 1 < h);
	bool LEFTisBorder = !(jLeft - 1 >= 0);
	bool RIGHTisBorder = !(jRight + 1 < w);

	ConnectionType topConnection, bottomConnection;

	// This `for` loop checks the top and bottom edges of the selection, from left to right.
	for (int j = jLeft; j <= jRight; j++) {
		// If the top or bottom side is on the border, then it requires `ConnectionType EMPTY`, otherwise it requires the external ConnectionType
		topConnection    =    TOPisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iTop - 1][j]][BOTTOM];
		bottomConnection = BOTTOMisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iBottom + 1][j]][TOP];
		// If the top connection and the bottom connection are not equivalent, then this selection does not have horizontal symmetry
		if (topConnection != reflectXTypes[bottomConnection])
			return false;
	}

	// This `for` loop checks the left and right edges of the selection (in that order, for each row), working outside-in
	for (int iIncr = iTop, iDecr = iBottom; iIncr < iDecr; iIncr++, iDecr--) {
		// If the left side is on the border, then they both require `ConnectionType EMPTY`, otherwise they require the external ConnectionType
		topConnection    = LEFTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iIncr][jLeft - 1]][RIGHT];
		bottomConnection = LEFTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iDecr][jLeft - 1]][RIGHT];
		// If the top connection and the bottom connection are not equivalent, then this selection does not have horizontal symmetry
		if (topConnection != reflectXTypes[bottomConnection])
			return false;

		// If the right side is on the border, then they both require `ConnectionType EMPTY`, otherwise they require the external ConnectionType
		topConnection    = RIGHTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iIncr][jRight + 1]][LEFT];
		bottomConnection = RIGHTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iDecr][jRight + 1]][LEFT];
		// If the top connection and the bottom connection are not equivalent, then this selection does not have horizontal symmetry
		if (topConnection != reflectXTypes[bottomConnection])
			return false;
	}

	return true;
}

bool** Knot::createAssignedPtr(const int selectNums[4], bool diagonal) {
	// This function gives the array of booleans that shows which glyph spots have been assigned already
	const int* n = selectNums;
	bool** assigned = new bool* [h];
	for (int i = 0; i < h; i++) {
		assigned[i] = new bool[w];
		for (int j = 0; j < w; j++) {
			bool withinSelection = (i >= n[0] - 1) && (i <= n[2] - 1) && (j >= n[1] - 1) && (j <= n[3] - 1);
			bool belowDiagonal = (i - n[0] > j - n[1]);
			assigned[i][j] = !withinSelection || (diagonal && belowDiagonal);
		}
	}
	return assigned;
}
void Knot::deleteAssignedPtr(bool** assigned) {
	for (int i = 0; i < h; i++)
		delete[] assigned[i];
	delete[] assigned;
}

bool Knot::tryGeneratingNoSym(const int selectNums[4], const std::vector<int>& startingGlyphList, int ignoreSide) {
	/*	This function generates a knot with no symmetry, lining up with the borders of the selection (unless ignoreSide tells it to be ignored)
	 *	The function changes `glyphIndices`, which may need to be reversed in the outer function that calls it.
	 */
	const int* n = selectNums;
	bool** assigned = createAssignedPtr(selectNums);

	bool success = true;
	for (int i = n[0] - 1; i <= n[2] - 1 && success; i++) {
		for (int j = n[1] - 1; j <= n[3] - 1 && success; j++) {
			// Sorted vector of the indices of all the glyphs that can fit into this cell. Gets narrowed down for each side's requirements.
			std::vector<int> glyphList = startingGlyphList;

			/*	Ignore a side iff: (1) bSIDE & ignoreSide == true AND (2) it is on a border
				Otherwise check the side	*/
			if (!((bTOP & ignoreSide)    && (i == n[0] - 1)))		checkSide(glyphList, assigned, i, j, TOP);
			if (!((bBOTTOM & ignoreSide) && (i == n[2] - 1)))		checkSide(glyphList, assigned, i, j, BOTTOM);
			if (!((bLEFT & ignoreSide)   && (j == n[1] - 1)))		checkSide(glyphList, assigned, i, j, LEFT);
			if (!((bRIGHT & ignoreSide)  && (j == n[3] - 1)))		checkSide(glyphList, assigned, i, j, RIGHT);

			// After all 4 sides are considered, if there are no possible glyphs remaining then this is an unsuccessful attempt.
			if (glyphList.size() == 0) {
				success = false;
			}
			else {
				glyphIndices[i][j] = glyphList[rand() % glyphList.size()];
				assigned[i][j] = true;
			}
		}
	}

	deleteAssignedPtr(assigned);
	return success;
}
void Knot::checkSide(std::vector<int>& glyphList, bool** assigned, int i, int j, Side side) {
	bool notGridBorder = true; // If this side of this square is not part of the outer border
	int newI = i, newJ = j;
	switch (side) {
		case TOP:		newI--; notGridBorder = newI >= 0;	break;
		case BOTTOM:	newI++; notGridBorder = newI < h;	break;
		case LEFT:		newJ--; notGridBorder = newJ >= 0;	break;
		case RIGHT:		newJ++; notGridBorder = newJ < w;	break;
	}

	if (notGridBorder) {	// If this side of this square is not part of the outer border (i.e. this side is touching another square)
		if (assigned[newI][newJ])				// If the cell in this direction is already assigned, then ensure that this square connects to it
			filterGlyphList(glyphList, connectionForGlyphOnSide[glyphIndices[newI][newJ]][side ^ 1], side); // `side ^ 1` is the opposite side
	}
	else					// Else this square is in an outer border, facing outward on `Side side`
		filterGlyphList(glyphList, EMPTY, side);	// Assume it requires ConnectionType EMPTY by default
}
inline void Knot::filterGlyphList(std::vector<int>& glyphList, ConnectionType type, Side side) {
	set_intersection_inplace(glyphList, glyphsWithTypeOnSide[type][side]);
}
inline std::vector<int> Knot::set_intersection(const std::vector<int>& first, const std::vector<int>& second) {
	std::vector<int> output;
	std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(output));
	return output;
}
inline void Knot::set_intersection_inplace(std::vector<int>& first, const std::vector<int>& second) {
	std::vector<int> output;
	std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(output));
	first = output;
}

#pragma warning( pop )

/*

Steps of WFC algorithm

SETUP:
	S-0) Allocate the knotGlyphs vector, which will be a class member in the future
	S-1) Start with
		S-1a) Vector for all superpositions
		S-1b) Vector for all remaining glyphs left to be determined
	S-2) Eliminate possibilities from the superpositions due to external boundary conditions
	S-3) Eliminate possibilities from the superpositions due to the internal boundary conditions

Loop until all glyphs are determined (toBeDetermined.size() == 0):
	L-1) 
	L-2) 

ENDING:
	E-1) Write data to the knot

*/

inline bool Knot::inSelection(const int selectNums[4], int i, int j) {
	return (i >= selectNums[0] - 1) && (i <= selectNums[2] - 1) && (j >= selectNums[1] - 1) && (j <= selectNums[3] - 1);
}
inline bool Knot::onBoundary(int i, int j) {
	return (i == 0) || (i == h - 1) || (j == 0) || (j == w - 1);
}

bool Knot::waveCollapseNoSym(const int selectNums[4]) {
	const int* n = selectNums;

	/* A general pattern to optimize later:

		ConnectionType requirement = (something);
		for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
			if (superpositions[i][j].glyphList[k].(side) != requirement) {
				superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
			}
		}

	*/

	/* STEP S-0: ALLOCATE THE knotGlyphs VECTOR. THIS WILL BE MOVED INTO THE CTOR LATER, WITH THE GLYPHS ZERO-INITIALIZED. */
	std::vector<std::vector<Glyph>> knotGlyphs;
	for (int i = 0; i < h; i++) {
		knotGlyphs.push_back(std::vector<Glyph>());
		for (int j = 0; j < w; j++) {
			knotGlyphs[i].push_back(allGlyphs[glyphIndices[i][j]]);
		}
	}

	/* STEP S-1: STARTING VECTORS */
	std::vector<std::vector<GlyphSuperPos>> superpositions; // The 2D vector of all superpositions
	std::vector< std::pair<int, int> > toBeDetermined;		// The 2D vector of all locations that need to be determined

	/* STEP S-2: EXTERNAL BOUNDARIES */
	for (int i = 0; i < h; i++) {
		superpositions.push_back(std::vector<GlyphSuperPos>());
		for (int j = 0; j < w; j++) {

			// Check if this tile is in the selection
			if (inSelection(n, i, j)) {
				toBeDetermined.push_back(std::make_pair(i, j));
				superpositions[i].push_back(allGlyphSuperPos);
				if (i == 0) {
					ConnectionType requirement = EMPTY;
					for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
						if (superpositions[i][j].glyphList[k].up != requirement) {
							superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
						}
					}
				}
				else if (i == h - 1) {
					ConnectionType requirement = EMPTY;
					for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
						if (superpositions[i][j].glyphList[k].down != requirement) {
							superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
						}
					}
				}

				if (j == 0) {
					ConnectionType requirement = EMPTY;
					for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
						if (superpositions[i][j].glyphList[k].left != requirement) {
							superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
						}
					}
				}
				else if (j == w - 1) {
					ConnectionType requirement = EMPTY;
					for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
						if (superpositions[i][j].glyphList[k].right != requirement) {
							superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
						}
					}
				}
			}
			else {
				superpositions[i].push_back(GlyphSuperPos(knotGlyphs[i][j]));
			}

		}
	}//*/

	/* STEP S-3: INTERNAL BOUNDARIES */
	if (n[0] - 1 != 0) { // The up selection boundary
		for (int i = n[0] - 1, j = n[1] - 1; j <= n[3] - 1; j++) {
			ConnectionType requirement = static_cast<Glyph>(superpositions[i - 1][j]).down;
			for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
				if (superpositions[i][j].glyphList[k].up != requirement) {
					superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
				}
			}
		}
	}
	if (n[2] - 1 != h - 1) { // The down selection boundary
		for (int i = n[2] - 1, j = n[1] - 1; j <= n[3] - 1; j++) {
			ConnectionType requirement = static_cast<Glyph>(superpositions[i + 1][j]).up;
			for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
				if (superpositions[i][j].glyphList[k].down != requirement) {
					superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
				}
			}
		}
	}
	if (n[1] - 1 != 0) { // The left selection boundary
		for (int j = n[1] - 1, i = n[0] - 1; i <= n[2] - 1; i++) {
			ConnectionType requirement = static_cast<Glyph>(superpositions[i][j - 1]).right;
			for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
				if (superpositions[i][j].glyphList[k].left != requirement) {
					superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
				}
			}
		}
	}
	if (n[3] - 1 != w - 1) { // The right selection boundary
		for (int j = n[3] - 1, i = n[0] - 1; i <= n[2] - 1; i++) {
			ConnectionType requirement = static_cast<Glyph>(superpositions[i][j + 1]).left;
			for (int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
				if (superpositions[i][j].glyphList[k].right != requirement) {
					superpositions[i][j].glyphList.erase(superpositions[i][j].glyphList.begin() + (k--));
				}
			}
		}
	}


	


	/* STEP E-1: SET THE KNOT IN PLACE */
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			glyphIndices[i][j] = superpositions[i][j].glyphList[0].index;
		}
	}//*/

	return true;
}


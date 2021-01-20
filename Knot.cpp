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

Knot::Knot(int h, int w, wxStatusBar* statusBar) : h(h), w(w), statusBar(statusBar) {
	
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

bool Knot::generateNoSym(ijSignature) {
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		//*
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating no symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(iMin, jMin, iMax, jMax, allGlyphIndices)) {
			success = true;
			//wxMessageBox(intWX(attempts));
		}
		else
			glyphIndices = previousGlyphIndices;//*/
	}

	return success;
}
bool Knot::generateVertSym(ijSignature) {
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	const std::vector<int> middleColumnStartingGlyphList = isEvenSegments(jMin, jMax) ? connectToReflectY : sameAfterReflectY;

	const int jMid = (jMin + jMax) / 2; // the index of the middle column

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(iMin, jMin, iMax, jMid - 1, allGlyphIndices, bRIGHT) &&			// Generating left half
			tryGeneratingNoSym(iMin, jMid, iMax, jMid, middleColumnStartingGlyphList, bRIGHT))	// Generating middle column
			success = true;
		else
			glyphIndices = previousGlyphIndices;
	}

	if (success) // success, therefore mirror the glyphs that need mirroring
		for (int i = iMin; i <= iMax; i++)
			for (int jIncr = jMin, jDecr = jMax; jIncr < jDecr; jIncr++, jDecr--)
				glyphIndices[i][jDecr] = reflectYGlyphs[glyphIndices[i][jIncr]];
	return success;
}
bool Knot::generateHoriSym(ijSignature) {
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	const std::vector<int> middleRowStartingGlyphList = isEvenSegments(iMin, iMax) ? connectToReflectX : sameAfterReflectX;

	const int iMid = (iMin + iMax) / 2; // the index of the middle row

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating horizontal symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(iMin, jMin, iMid - 1, jMax, allGlyphIndices, bBOTTOM) &&			// Generating upper half
			tryGeneratingNoSym(iMid, jMin, iMid, jMax, middleRowStartingGlyphList, bBOTTOM))	// Generating middle row
			success = true;
		else
			glyphIndices = previousGlyphIndices;
	}

	if (success) // success, therefore mirror the glyphs that need mirroring
		for (int iIncr = iMin, iDecr = iMax; iIncr < iDecr; iIncr++, iDecr--)
			for (int j = jMin; j <= jMax; j++)
				glyphIndices[iDecr][j] = reflectXGlyphs[glyphIndices[iIncr][j]];
	return success;
}
bool Knot::generateVertHoriSym(ijSignature) {
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	const std::vector<int> middleColumnStartingGlyphList = isEvenSegments(jMin, jMax) ? connectToReflectY : sameAfterReflectY;
	const std::vector<int> middleRowStartingGlyphList = isEvenSegments(iMin, iMax) ? connectToReflectX : sameAfterReflectX;
	const std::vector<int> middleSquareStartingGlyphList = set_intersection(middleColumnStartingGlyphList, middleRowStartingGlyphList);

	const int jMid = (jMin + jMax) / 2; // the index of the middle column
	const int iMid = (iMin + iMax) / 2; // the index of the middle row

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating vertical + horizontal symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(iMin, jMin, iMid - 1, jMid - 1, allGlyphIndices, bRIGHT | bBOTTOM)				&&	// Generating upper left quarter
			tryGeneratingNoSym(iMin, jMid, iMid - 1, jMid, middleColumnStartingGlyphList, bRIGHT | bBOTTOM)		&&	// Generating middle column
			tryGeneratingNoSym(iMid, jMin, iMid, jMid - 1, middleRowStartingGlyphList, bRIGHT | bBOTTOM)		&&	// Generating middle row
			tryGeneratingNoSym(iMid, jMid, iMid, jMid, middleSquareStartingGlyphList, bRIGHT | bBOTTOM))			// Generating middle piece
			success = true;
		else
			glyphIndices = previousGlyphIndices;
	}

	if (success) { // success, therefore mirror the glyphs that need mirroring
		for (int i = iMin; i <= iMid; i++)
			for (int jIncr = jMin, jDecr = jMax; jIncr < jDecr; jIncr++, jDecr--)
				glyphIndices[i][jDecr] = reflectYGlyphs[glyphIndices[i][jIncr]];
		for (int iIncr = iMin, iDecr = iMax; iIncr < iDecr; iIncr++, iDecr--)
			for (int j = jMin; j <= jMax; j++)
				glyphIndices[iDecr][j] = reflectXGlyphs[glyphIndices[iIncr][j]];
	}
	return success;
}
bool Knot::generateRot2Sym(ijSignature) {
	const std::vector< std::vector<int> > previousGlyphIndices(glyphIndices);

	const bool isEvenNumberOfColumns = isEvenSegments(jMin, jMax);

	const int jMid = (jMin + jMax) / 2; // the index of the middle column
	const int iMid = (iMin + iMax) / 2; // the index of the middle row

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText("Generating 2-way rotational symmetry... Attempt " + intWX(attempts) + "/" + MAX_ATTEMPTS_STR);
		if (tryGeneratingNoSym(iMin, jMin, iMax, jMid - 1, allGlyphIndices, bRIGHT)) { // Generating the left half

			for (int iIncr = iMin, iDecr = iMax; iIncr <= iMax; iIncr++, iDecr--)							// }	
				for (int jIncr = jMin, jDecr = jMax; jIncr < jMid; jIncr++, jDecr--)						// } Rotate-mirroring the left half
					glyphIndices[iDecr][jDecr] = rotateGlyphs[rotateGlyphs[glyphIndices[iIncr][jIncr]]];	// }

			if (tryGeneratingNoSym(iMin, jMid, iMid - 1, jMid + isEvenNumberOfColumns, allGlyphIndices, bBOTTOM)) {	// Generating the top section of the missing chunk

				for (int iIncr = iMin, iDecr = iMax; iIncr < iMid; iIncr++, iDecr--)
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
	return success;
}

bool Knot::checkVertSym(ijSignature) {
	// Which of the sides of this selection are on the border of the knot
	const bool TOPisBorder		= !(iMin - 1 >= 0);
	const bool BOTTOMisBorder	= !(iMax + 1 < h);
	const bool LEFTisBorder		= !(jMin - 1 >= 0);
	const bool RIGHTisBorder	= !(jMax + 1 < w);

	ConnectionType leftConnection, rightConnection;

	// This `for` loop checks the left and right edges of the selection, from top to bottom.
	for (int i = iMin; i <= iMax; i++) {
		// If the left or right side is on the border, then it requires `ConnectionType EMPTY`, otherwise it requires the external ConnectionType
		leftConnection  =  LEFTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[i][jMin - 1]][RIGHT];
		rightConnection = RIGHTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[i][jMax + 1]][LEFT];
		// If the left connection and the right connection are not equivalent, then this selection does not have vertical symmetry
		if (leftConnection != reflectYTypes[rightConnection])
			return false;
	}

	// This `for` loop checks the top and bottom edges of the selection (in that order, for each column), working outside-in
	for (int jIncr = jMin, jDecr = jMax; jIncr < jDecr; jIncr++, jDecr--) {
		// If the top side is on the border, then they both require `ConnectionType EMPTY`, otherwise they require the external ConnectionType
		leftConnection  = TOPisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iMin - 1][jIncr]][BOTTOM];
		rightConnection = TOPisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iMin - 1][jDecr]][BOTTOM];
		// If the left connection and the right connection are not equivalent, then this selection does not have vertical symmetry
		if (leftConnection != reflectYTypes[rightConnection])
			return false;

		// If the bottom side is on the border, then they both require `ConnectionType EMPTY`, otherwise they require the external ConnectionType
		leftConnection  = BOTTOMisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iMax + 1][jIncr]][TOP];
		rightConnection = BOTTOMisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iMax + 1][jDecr]][TOP];
		// If the left connection and the right connection are not equivalent, then this selection does not have vertical symmetry
		if (leftConnection != reflectYTypes[rightConnection])
			return false;
	}

	return true;
}
bool Knot::checkHoriSym(ijSignature) {
	// Which of the sides of this selection are on the border of the knot
	const bool TOPisBorder = !(iMin - 1 >= 0);
	const bool BOTTOMisBorder = !(iMax + 1 < h);
	const bool LEFTisBorder = !(jMin - 1 >= 0);
	const bool RIGHTisBorder = !(jMax + 1 < w);

	ConnectionType topConnection, bottomConnection;

	// This `for` loop checks the top and bottom edges of the selection, from left to right.
	for (int j = jMin; j <= jMax; j++) {
		// If the top or bottom side is on the border, then it requires `ConnectionType EMPTY`, otherwise it requires the external ConnectionType
		topConnection    =    TOPisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iMin - 1][j]][BOTTOM];
		bottomConnection = BOTTOMisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iMax + 1][j]][TOP];
		// If the top connection and the bottom connection are not equivalent, then this selection does not have horizontal symmetry
		if (topConnection != reflectXTypes[bottomConnection])
			return false;
	}

	// This `for` loop checks the left and right edges of the selection (in that order, for each row), working outside-in
	for (int iIncr = iMin, iDecr = iMax; iIncr < iDecr; iIncr++, iDecr--) {
		// If the left side is on the border, then they both require `ConnectionType EMPTY`, otherwise they require the external ConnectionType
		topConnection    = LEFTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iIncr][jMin - 1]][RIGHT];
		bottomConnection = LEFTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iDecr][jMin - 1]][RIGHT];
		// If the top connection and the bottom connection are not equivalent, then this selection does not have horizontal symmetry
		if (topConnection != reflectXTypes[bottomConnection])
			return false;

		// If the right side is on the border, then they both require `ConnectionType EMPTY`, otherwise they require the external ConnectionType
		topConnection    = RIGHTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iIncr][jMax + 1]][LEFT];
		bottomConnection = RIGHTisBorder ? EMPTY : connectionForGlyphOnSide[glyphIndices[iDecr][jMax + 1]][LEFT];
		// If the top connection and the bottom connection are not equivalent, then this selection does not have horizontal symmetry
		if (topConnection != reflectXTypes[bottomConnection])
			return false;
	}

	return true;
}

bool** Knot::createAssignedPtr(ijSignature, bool diagonal) {
	// This function gives the array of booleans that shows which glyph spots have been assigned already
	bool** assigned = new bool* [h];
	for (int i = 0; i < h; i++) {
		assigned[i] = new bool[w];
		for (int j = 0; j < w; j++) {
			bool withinSelection = (i >= iMin) && (i <= iMax) && (j >= jMin) && (j <= jMax);
			bool belowDiagonal = (i - iMin > j - jMin);
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

bool Knot::tryGeneratingNoSym(ijSignature, const std::vector<int>& startingGlyphList, int ignoreSide) {
	/*	This function generates a knot with no symmetry, lining up with the borders of the selection (unless ignoreSide tells it to be ignored)
	 *	The function changes `glyphIndices`, which may need to be reversed in the outer function that calls it.
	 */
	bool** assigned = createAssignedPtr(iMin, jMin, iMax, jMax);

	bool success = true;
	for (int i = iMin; i <= iMax && success; i++) {
		for (int j = jMin; j <= jMax && success; j++) {
			// Sorted vector of the indices of all the glyphs that can fit into this cell. Gets narrowed down for each side's requirements.
			std::vector<int> glyphList = startingGlyphList;

			/*	Ignore a side iff: (1) bSIDE & ignoreSide == true AND (2) it is on a border
				Otherwise check the side	*/
			if (!((bTOP & ignoreSide)    && (i == iMin)))		checkSide(glyphList, assigned, i, j, TOP);
			if (!((bBOTTOM & ignoreSide) && (i == iMax)))		checkSide(glyphList, assigned, i, j, BOTTOM);
			if (!((bLEFT & ignoreSide)   && (j == jMin)))		checkSide(glyphList, assigned, i, j, LEFT);
			if (!((bRIGHT & ignoreSide)  && (j == jMax)))		checkSide(glyphList, assigned, i, j, RIGHT);

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
inline bool Knot::isEvenSegments(const int min, const int max) {
	return (max - min + 1) % 2 == 0;
}

#pragma warning( pop )

/* WFC method outline */
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
	L-1) Select a random location from `toBeDetermined`, then collapse its `GlyphSuperPos`
	L-2) Remove conflicting options from the other superpositions
		L-2a) Remove from immediate neighbouring superpositions
		L-2b) Recursively check if any of the neighbouring superpositions preclude connections from its own neighbours

ENDING:
	E-1) Write data to the knot

*/

/*
inline bool Knot::inSelection(const int selectNums[4], int i, int j) {
	return (i >= selectNums[0] - 1) && (i <= selectNums[2] - 1) && (j >= selectNums[1] - 1) && (j <= selectNums[3] - 1);
}
inline bool Knot::onBoundary(int i, int j) {
	return (i == 0) || (i == h - 1) || (j == 0) || (j == w - 1);
}//*/

/* WFC algorithm*/
/* bool Knot::waveCollapseNoSym(const int selectNums[4]) {
	const int* n = selectNums;

	/* A general pattern to optimize later:

		ConnectionType requirement = (something);
		for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
			if (superpositions[i][j].glyphList[k].(side) != requirement) {
				superpositions[i][j].remove(k--);
			}
		}
		if (superpositions[i][j].glyphList.empty()) return false;

	* /

	/* STEP S-0: ALLOCATE THE knotGlyphs VECTOR. THIS WILL BE MOVED INTO THE CTOR LATER, WITH THE GLYPHS ZERO-INITIALIZED. * /
	std::vector<std::vector<Glyph>> knotGlyphs;
	for (int i = 0; i < h; i++) {
		knotGlyphs.emplace_back();
		for (int j = 0; j < w; j++) {
			knotGlyphs[i].push_back(allGlyphs[glyphIndices[i][j]]);
		}
	}

	bool success = false;
	for (int attempts = 1; attempts <= MAX_ATTEMPTS && !success; attempts++) {

		/* STEP S-1: STARTING VECTORS * /
		std::vector<std::vector<GlyphSuperPos>> superpositions; // The 2D vector of all superpositions
		std::vector< std::pair<int, int> > toBeDetermined;		// The 2D vector of all locations that need to be determined
		
		/* STEP S-2: EXTERNAL BOUNDARIES * /
		for (int i = 0; i < h; i++) {
			superpositions.emplace_back();
			for (int j = 0; j < w; j++) {

				// Check if this tile is in the selection
				if (inSelection(n, i, j)) {
					toBeDetermined.emplace_back(i, j);
					superpositions[i].push_back(allGlyphSuperPos);
					if (i == 0) {
						ConnectionType requirement = EMPTY;
						for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
							if (superpositions[i][j].glyphList[k].up != requirement) {
								superpositions[i][j].remove(k--);
							}
						}
						if (superpositions[i][j].glyphList.empty()) return success;
					}
					else if (i == h - 1) {
						ConnectionType requirement = EMPTY;
						for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
							if (superpositions[i][j].glyphList[k].down != requirement) {
								superpositions[i][j].remove(k--);
							}
						}
						if (superpositions[i][j].glyphList.empty()) return success;
					}

					if (j == 0) {
						ConnectionType requirement = EMPTY;
						for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
							if (superpositions[i][j].glyphList[k].left != requirement) {
								superpositions[i][j].remove(k--);
							}
						}
						if (superpositions[i][j].glyphList.empty()) return success;
					}
					else if (j == w - 1) {
						ConnectionType requirement = EMPTY;
						for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
							if (superpositions[i][j].glyphList[k].right != requirement) {
								superpositions[i][j].remove(k--);
							}
						}
						if (superpositions[i][j].glyphList.empty()) return success;
					}
				}
				else {
					superpositions[i].emplace_back(knotGlyphs[i][j]);
				}

			}
		}//* /

		/* STEP S-3: INTERNAL BOUNDARIES * /
		if (n[0] - 1 != 0) { // The up selection boundary
			for (int i = n[0] - 1, j = n[1] - 1; j <= n[3] - 1; j++) {
				ConnectionType requirement = static_cast<Glyph>(superpositions[i - 1][j]).down;
				for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
					if (superpositions[i][j].glyphList[k].up != requirement) {
						superpositions[i][j].remove(k--);
					}
				}
				if (superpositions[i][j].glyphList.empty()) return success;
			}
		}
		if (n[2] - 1 != h - 1) { // The down selection boundary
			for (int i = n[2] - 1, j = n[1] - 1; j <= n[3] - 1; j++) {
				ConnectionType requirement = static_cast<Glyph>(superpositions[i + 1][j]).up;
				for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
					if (superpositions[i][j].glyphList[k].down != requirement) {
						superpositions[i][j].remove(k--);
					}
				}
				if (superpositions[i][j].glyphList.empty()) return success;
			}
		}
		if (n[1] - 1 != 0) { // The left selection boundary
			for (int j = n[1] - 1, i = n[0] - 1; i <= n[2] - 1; i++) {
				ConnectionType requirement = static_cast<Glyph>(superpositions[i][j - 1]).right;
				for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
					if (superpositions[i][j].glyphList[k].left != requirement) {
						superpositions[i][j].remove(k--);
					}
				}
				if (superpositions[i][j].glyphList.empty()) return success;
			}
		}
		if (n[3] - 1 != w - 1) { // The right selection boundary
			for (int j = n[3] - 1, i = n[0] - 1; i <= n[2] - 1; i++) {
				ConnectionType requirement = static_cast<Glyph>(superpositions[i][j + 1]).left;
				for (unsigned int k = 0; k < superpositions[i][j].glyphList.size(); k++) {
					if (superpositions[i][j].glyphList[k].right != requirement) {
						superpositions[i][j].remove(k--);
					}
				}
				if (superpositions[i][j].glyphList.empty()) return success;
			}
		}

		/* LOOPING STEPS * /
		while (toBeDetermined.size() != 0) {

			/* STEP L-1: SELECT A RANDOM TILE * /

			const auto& [i, j] = pop_random(toBeDetermined);	// Grab a random location, store as (i,j)
			Glyph glyph = superpositions[i][j].collapse();		// Collapse this location

			/* STEP L-2: REMOVE CONFLICTING OPTIONS * /

			/* The up side * / {
				if (i != n[0] - 1) { // if this location is not on the up selection boundary
					ConnectionType requirement = static_cast<Glyph>(superpositions[i][j]).up;
					for (unsigned int k = 0; k < superpositions[i - 1][j].glyphList.size(); k++) {
						if (superpositions[i - 1][j].glyphList[k].down != requirement) {
							superpositions[i - 1][j].remove(k--);
						}
					}
					if (superpositions[i - 1][j].glyphList.empty()) return success;
				}
			}
			/* The down side * / {
				if (i != n[2] - 1) { // if this location is not on the down selection boundary
					ConnectionType requirement = static_cast<Glyph>(superpositions[i][j]).down;
					for (unsigned int k = 0; k < superpositions[i + 1][j].glyphList.size(); k++) {
						if (superpositions[i + 1][j].glyphList[k].up != requirement) {
							superpositions[i + 1][j].remove(k--);
						}
					}
					if (superpositions[i + 1][j].glyphList.empty()) return success;
				}
			}
			/* The left side * / {
				if (j != n[1] - 1) { // if this location is not on the left selection boundary
					ConnectionType requirement = static_cast<Glyph>(superpositions[i][j]).left;
					for (unsigned int k = 0; k < superpositions[i][j - 1].glyphList.size(); k++) {
						if (superpositions[i][j - 1].glyphList[k].right != requirement) {
							superpositions[i][j - 1].remove(k--);
						}
					}
					if (superpositions[i][j - 1].glyphList.empty()) return success;
				}
			}
			/* The right side * / {
				if (j != n[3] - 1) { // if this location is not on the right selection boundary
					ConnectionType requirement = static_cast<Glyph>(superpositions[i][j]).right;
					for (unsigned int k = 0; k < superpositions[i][j + 1].glyphList.size(); k++) {
						if (superpositions[i][j + 1].glyphList[k].left != requirement) {
							superpositions[i][j + 1].remove(k--);
						}
					}
					if (superpositions[i][j + 1].glyphList.empty()) return success;
				}
			}


			//ConnectionCountArray oldConnectionCount{ superpositions[i][j].connectionCount };


			//ConnectionCountArray newConnectionCount{ superpositions[i][j].connectionCount };



		}//* /
		/*  * /

		/* STEP E-1: SET THE KNOT IN PLACE * /
		success = true;
		if (success) {
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					glyphIndices[i][j] = superpositions[i][j].glyphList[0].index;
				}
			}
		}

//ending:

	}

	return success;
}//*/

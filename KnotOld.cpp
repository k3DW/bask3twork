#if 0

#include "KnotOld.h"

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

KnotOld::KnotOld(int h_, int w_, wxStatusBar* statusBar_) : h(h_), w(w_), statusBar(statusBar_) {

	this->glyphIndices = std::vector<std::vector<int>>(h, std::vector<int>(w, 0));

	//test = 0;

}

	//knotGlyphs = std::vector<std::vector<const Glyph*>>(1, std::vector<const Glyph*>(1, &testGlyph)); // Start by setting all glyphs to space "\x20"

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
//}
wxString KnotOld::get(int i, int j) {
	return glyphs[glyphIndices[i][j]];
}

bool KnotOld::generateNoSym(ijSignature) {
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
bool KnotOld::generateVertSym(ijSignature) {
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
bool KnotOld::generateHoriSym(ijSignature) {
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
bool KnotOld::generateVertHoriSym(ijSignature) {
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
bool KnotOld::generateRot2Sym(ijSignature) {
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

bool KnotOld::checkVertSym(ijSignature) {
	// Which of the sides of this selection are on the border of the knot
	const bool TOPisBorder		= iMin == 0;
	const bool BOTTOMisBorder	= iMax == h - 1;
	const bool LEFTisBorder		= jMin == 0;
	const bool RIGHTisBorder	= jMax == w - 1;

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
bool KnotOld::checkHoriSym(ijSignature) {
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

bool** KnotOld::createAssignedPtr(ijSignature, bool diagonal) {
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
void KnotOld::deleteAssignedPtr(bool** assigned) {
	for (int i = 0; i < h; i++)
		delete[] assigned[i];
	delete[] assigned;
}

bool KnotOld::tryGeneratingNoSym(ijSignature, const std::vector<int>& startingGlyphList, int ignoreSide) {
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
void KnotOld::checkSide(std::vector<int>& glyphList, bool** assigned, int i, int j, SideOLD side) {
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
inline void KnotOld::filterGlyphList(std::vector<int>& glyphList, ConnectionType type, SideOLD side) {
	set_intersection_inplace(glyphList, glyphsWithTypeOnSide[type][side]);
}
inline std::vector<int> KnotOld::set_intersection(const std::vector<int>& first, const std::vector<int>& second) {
	std::vector<int> output;
	std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(output));
	return output;
}
inline void KnotOld::set_intersection_inplace(std::vector<int>& first, const std::vector<int>& second) {
	std::vector<int> output;
	std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(output));
	first = output;
}
inline bool KnotOld::isEvenSegments(const int min, const int max) {
	return (max - min + 1) % 2 == 0;
}

#pragma warning( pop )

#endif
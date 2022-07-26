#include "Knot.h"

Knot::Knot(int h, int w, wxStatusBar* statusBar) : h(h), w(w), statusBar(statusBar), glyphs(h, w) {}
Knot::Knot(Glyphs&& glyphs, wxStatusBar* statusBar) : h(glyphs.rows()), w(glyphs.columns()), statusBar(statusBar), glyphs(glyphs) {}
wxString Knot::get(const int i, const int j) const { return glyphs.at(i, j)->chr; }

bool Knot::generate(Symmetry sym, Selection selection)
/** Generate a knot with the given symmetry in the given selection.
 *
 * Only generates from row \c iMin to row \c iMax, and from column \c jMin to column \c jMax.
 * This function assumes \c iMin \c <= \c iMax and \c jMin \c <= \c jMax, and does not check the values.
 * The boundary conditions around the selection are maintained.
 * This function does not check the boundary symmetries to determine whether the specified symmetry is allowed.
 * The knot will generate with undefined behaviour if the proper symmetry conditions are not met in the first place.
 *
 * \param sym The symmetry of the knot to be generated
 * \param iMin The zero-indexed upper row of the selection visually(lower numerically)
 * \param jMin The zero-indexed leftmost column of the selection
 * \param iMax The zero-indexed lower row of the selection visually(higher numerically)
 * \param jMax The zero-indexed rightmost column of the selection
 * \return A boolean value denoting whether or not the generating was successful
 * 
 * \b Method
 */
{
	/// First, instantiate a \c wxString to show in the status bar,
	/// then add to it in a \c switch statement for the specific symmetry used.
	/// If the value for \c sym is not valid, this \c switch statement will cause the function to return \c false.
	wxString statusBeginning = "Generating ";
	switch (sym) {
		case Symmetry::NoSym:		{ statusBeginning += "no symmetry... ";						break; }
		case Symmetry::HoriSym:		{ statusBeginning += "horizontal symmetry... ";				break; }
		case Symmetry::VertSym:		{ statusBeginning += "vertical symmetry... ";				break; }
		case Symmetry::HoriVertSym: { statusBeginning += "horizontal + vertical symmetry... ";	break; }
		case Symmetry::Rot2Sym:		{ statusBeginning += "2-way rotational symmetry... ";		break; }
		case Symmetry::Rot4Sym:		{ statusBeginning += "4-way rotational symmetry... ";		break; }
		case Symmetry::FwdDiag:		{ statusBeginning += "forward diagonal symmetry... ";		break; }
		case Symmetry::BackDiag:	{ statusBeginning += "backward diagonal symmetry... ";		break; }
		case Symmetry::FullSym:		{ statusBeginning += "full symmetry... ";					break; }
		default:
			return false;
	}

	/// Then, make a copy of \c glyphs, and set all members in the selection to \c nullptr to denote that they are unassigned.
	Glyphs baseGlyphs = glyphs;
	for (int i = selection.min.i; i <= selection.max.i; i++)
		for (int j = selection.min.j; j <= selection.max.j; j++)
			baseGlyphs.at(i, j) = nullptr;

	/// Next, enter a loop, counting the number of attempts made at generating this knot. The steps are as follows.
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		/// \b (1) At certain intervals of numbers of attempts, update the status bar with the number of attempts made.
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText(wxString::Format("%sAttempt %i/%i", statusBeginning, attempts, MAX_ATTEMPTS));

		/// \b (2) Call Knot::tryGenerating() using the copy of \c glyphs created above.
		///		If it fails, \c continue the loop and try again.
		std::optional<Glyphs> newGlyphs = tryGenerating(baseGlyphs, sym, selection);
		if (!newGlyphs) continue;

		/// \b (3) If the knot has been successfully generated, set \c glyphs equal to this generated version and return \c true.
		glyphs = *newGlyphs;
		return true;
	}
	/// \b (4) If this loop has been completed, then the maximum number of attempts have been tried. Therefore return \c false.
	return false;
}

std::optional<Glyphs> Knot::tryGenerating(Glyphs glyphGrid, Symmetry sym, Selection selection) const
/** Called only from Knot::generate(), try generating a knot with the given symmetry for the given selection.
 * 
 * This function pulls the required logic in Knot::generate() in order to generate the Knot selection once, and places it into its own function. 
 *
 * The same conditions apply as in the first paragraph of Knot::generate().
 *
 * The \c glyphGrid variable is supplied from Knot::generate(), already initialized with \c nullptr entries for the selection.
 *
 * \b Method
 */
{
	/// First, start by creating constant \c bool and \c int values to use in the generating.
	/// This includes deconstructing the \c Symmetry into its component bit flags,
	/// storing whether the selection compasses all rows or columns in the Knot,
	/// storing the middle \c i and \c j indices of the selection,
	/// and storing the wrapping conditions.
	const bool bitHori = static_cast<bool>(sym & 0x01);
	const bool bitVert = static_cast<bool>(sym & 0x02);
	const bool bitRot2 = static_cast<bool>(sym & 0x04);
	const bool bitRot4 = static_cast<bool>(sym & 0x08);
	const bool bitFwDi = static_cast<bool>(sym & 0x10);
	const bool bitBkDi = static_cast<bool>(sym & 0x20);

	const bool isEvenRows = (selection.max.i - selection.min.i + 1) % 2 == 0;
	const bool isEvenCols = (selection.max.j - selection.min.j + 1) % 2 == 0;
	const bool isAllRows = selection.min.i == 0 && selection.max.i == h - 1;
	const bool isAllCols = selection.min.j == 0 && selection.max.j == w - 1;
	const int iMid = (selection.min.i + selection.max.i) / 2;
	const int jMid = (selection.min.j + selection.max.j) / 2;
	const GlyphFlag midHoriFlag = isEvenRows ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const GlyphFlag midVertFlag = isEvenCols ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	const GlyphFlag midRot2Flag = isEvenRows ? (isEvenCols ? GlyphFlag::NONE : GlyphFlag::CT_ROT2D) : (isEvenCols ? GlyphFlag::CT_ROT2R : GlyphFlag::SA_ROT2);
	
	const bool isSquare = bitRot4 || bitFwDi || bitBkDi;
	const GlyphFlag midRot4Flag = isSquare ? (isEvenRows ? GlyphFlag::CT_ROT4R : !isEvenRows ? GlyphFlag::SA_ROT4 : GlyphFlag::NONE) : GlyphFlag::NONE;
	const GlyphFlag selfFlag = (GlyphFlag::CT_SELFD * (h == 1)) | (GlyphFlag::CT_SELFR * (w == 1)); // If this selection is only 1 row in length in either direction, flag appropriately
	
	const bool doWrapX = wrapXEnabled && (!isSquare || wrapYEnabled);
	const bool doWrapY = wrapYEnabled && (!isSquare || wrapXEnabled);

	/// Next, enter a loop over each location in the selection. Do the following.
	for (int i = selection.min.i, iOffset = 0; i <= selection.max.i; i++, iOffset++) {
		for (int j = selection.min.j, jOffset = 0; j <= selection.max.j; j++, jOffset++) {
			/// \b (1) If the Glyph in this location is already set, \c continue the loop.
			if (glyphGrid.at(i, j)) continue;

			/// \b (2) If the Glyph has yet to be set, generate a RandomGlyph() for this location, with the \c GlyphFlag::NONE flag.
			///	For each of the 4 \c Connection parameters, there are many possible cases, implemented in a large nested ternary operation, described below.
			glyphGrid.at(i, j) = RandomGlyph(
				/** (a) If this Glyph location is on the outer edge of the selection on this particular side, branch to condition B, otherwise branch to condition E.
				 *
				 *	(b) If wrapping is not enabled in this direction, then the parameter should be \c Connection::EMPTY.
				 *	(c) If the Glyph directly on the other end of the wrap is not assigned, then the parameter should be \c Connection::DO_NOT_CARE.
				 *	(d) In the general wrapping case, where none of the other special wrapping cases apply, the parameter should be
				 *		the connection on the opposite side of the glyph on the other end of the wrap.
				 *
				 *	(e) If the next glyph on this particular side is not yet assigned, the parameter should be \c Connection::DO_NOT_CARE.
				 *	(f) In the general case, where none of the other special cases apply, the parameter should be
				 *		the connection on the opposite side, from the neighbouring glyph on this particular side.
				 */
				{
					i == 0		? (!doWrapY ? Connection::EMPTY : !glyphGrid.at(h - 1, j)	? Connection::DO_NOT_CARE : glyphGrid.at(h - 1, j)->down)	: (!glyphGrid.at(i - 1, j) ? Connection::DO_NOT_CARE : glyphGrid.at(i - 1, j)->down	) ,
					i == h - 1	? (!doWrapY ? Connection::EMPTY : !glyphGrid.at(0, j)		? Connection::DO_NOT_CARE : glyphGrid.at(0, j)->up)		: (!glyphGrid.at(i + 1, j) ? Connection::DO_NOT_CARE : glyphGrid.at(i + 1, j)->up		) ,
					j == 0		? (!doWrapX ? Connection::EMPTY : !glyphGrid.at(i, w - 1)	? Connection::DO_NOT_CARE : glyphGrid.at(i, w - 1)->right) : (!glyphGrid.at(i, j - 1) ? Connection::DO_NOT_CARE : glyphGrid.at(i, j - 1)->right	) ,
					j == w - 1	? (!doWrapX ? Connection::EMPTY : !glyphGrid.at(i, 0)		? Connection::DO_NOT_CARE : glyphGrid.at(i, 0)->left)		: (!glyphGrid.at(i, j + 1) ? Connection::DO_NOT_CARE : glyphGrid.at(i, j + 1)->left	)
				},
				/** The \c boolFlags argument in RandomGlyph() has different components added, under various conditions. 
				 *  (a) If this type of symmetry includes horizontal reflection, then add \c GlyphFlag::CT_MIRU, only if the selection encompasses all rows and if the current location is in the uppermost row of the Knot.
				 *  (b) If this type of symmetry includes horizontal reflection but this time the current operation is in the middle row, then add either \c GlyphFlag::CT_MIRD or \c GlyphFlag::SA_MIRX depending on parity.
				 *  (c) If this type of symmetry includes vertical reflection, then add \c GlyphFlag::CT_MIRL, only if the selection encompasses all columns and if the current location is in the leftmost column of the Knot.
				 *  (d) If this type of symmetry includes vertical reflection but this time the current operation is in the middle column, then add either \c GlyphFlag::CT_MIRR or \c GlyphFlag::SA_MIRY depending on parity.
				 *  (e) If this type of symmetry includes 2-way rotation and the current operation is in the middle row and middle column, then add either \c GlyphFlag::CT_ROT2D, \c GlyphFlag::CT_ROT2R, or \c GlyphFlag::SA_ROT2, depending on the parities.
				 *  (f) If this type of symmetry includes 4-way rotation and the current operation is in the middle row and middle column, then add either \c GlyphFlag::CT_ROT4R or \c GlyphFlag::SA_ROT4 depending on the parity.
				 *  (g) If this type of symmetry includes back diagonal reflection, then add \c GlyphFlag::SA_MIRBD, only if the \c i offset and the \c j offset are equal, meaning that the location is along the diagonal.
				 *  (h) If this selection is only 1 row in height, add GlyphFlag::CT_SELFD.
				 *  (i) If this selection is only 1 column in width, add GlyphFlag::CT_SELFR.
				 */
				(
					(GlyphFlag::CT_MIRU  * (bitHori && isAllRows && i == 0)) | 
					(midHoriFlag         * (bitHori && i == iMid)) | 
					(GlyphFlag::CT_MIRL  * (bitVert && isAllCols && j == 0)) |
					(midVertFlag         * (bitVert && j == jMid)) |
					(midRot2Flag         * (bitRot2 && i == iMid && j == jMid)) |
					(midRot4Flag         * (bitRot4 && i == iMid && j == jMid)) |
					(GlyphFlag::SA_MIRBD * (bitBkDi && isSquare && iOffset == jOffset)) |
					(selfFlag)
				)
			);

			/// \b (3) If this newly generated Glyph turns out to be \c nullptr, then there were no options for this location. Return \c std::nullopt.
			if (!glyphGrid.at(i, j)) return std::nullopt;

			/// \b (4) If the function has made it to this point, then reflect and rotate the newly generated Glyph to the appropriate spots given the symmetry required.
			if (bitHori) glyphGrid.at(selection.max.i - iOffset, j) = glyphGrid.at(i, j)->mirroredX;
			if (bitVert) glyphGrid.at(i, selection.max.j - jOffset) = glyphGrid.at(i, j)->mirroredY;
			if (bitRot2) glyphGrid.at(selection.max.i - iOffset, selection.max.j - jOffset) = glyphGrid.at(i, j)->rotated2;
			if (bitRot4) { glyphGrid.at(selection.min.i + jOffset, selection.max.j - iOffset) = glyphGrid.at(i, j)->rotated4; glyphGrid.at(selection.max.i - jOffset, selection.min.j + iOffset) = glyphGrid.at(i, j)->rotated2->rotated4; }
			if (bitFwDi) glyphGrid.at(selection.max.i - jOffset, selection.max.j - iOffset) = glyphGrid.at(i, j)->mirroredFD;
			if (bitBkDi) glyphGrid.at(selection.min.i + jOffset, selection.min.j + iOffset) = glyphGrid.at(i, j)->mirroredBD;
		}
	}

	/// \b (5) If the loop finishes, then the Knot has been successfully generated. Return \c true.
	return glyphGrid;
}

bool Knot::checkHoriSym(Selection selection) const
{
	if (selection.is_full_selection(h, w))
		return true;

	return check_glyph_pair(&Glyphs::left_side_bounds, { &GlyphIterator::move_down, &GlyphIterator::move_up }, { &Glyph::left, &Glyph::left }, mirror_x, selection)
	    && check_glyph_pair(&Glyphs::right_side_bounds, { &GlyphIterator::move_down, &GlyphIterator::move_up }, { &Glyph::right, &Glyph::right }, mirror_x, selection)
	    && check_glyph_pair(&Glyphs::left_side_bounds, { &GlyphIterator::move_right, &GlyphIterator::move_right }, { &Glyph::up, &Glyph::down }, mirror_x, selection)
	;
}
bool Knot::checkVertSym(Selection selection) const
{
	if (selection.is_full_selection(h, w))
		return true;

	return check_glyph_pair(&Glyphs::upper_side_bounds, { &GlyphIterator::move_right, &GlyphIterator::move_left }, { &Glyph::up, &Glyph::up }, mirror_y, selection)
	    && check_glyph_pair(&Glyphs::lower_side_bounds, { &GlyphIterator::move_right, &GlyphIterator::move_left }, { &Glyph::down, &Glyph::down }, mirror_y, selection)
	    && check_glyph_pair(&Glyphs::upper_side_bounds, { &GlyphIterator::move_down, &GlyphIterator::move_down }, { &Glyph::left, &Glyph::right }, mirror_y, selection)
	;
}
bool Knot::checkRot2Sym(Selection selection) const
{
	if (selection.is_full_selection(h, w))
		return true;

	return check_glyph_pair(&Glyphs::backward_diagonal, { &GlyphIterator::move_right, &GlyphIterator::move_left }, { &Glyph::up, &Glyph::down }, rotate_180, selection)
	    && check_glyph_pair(&Glyphs::backward_diagonal, { &GlyphIterator::move_down, &GlyphIterator::move_up }, { &Glyph::left, &Glyph::right }, rotate_180, selection)
	;
}
bool Knot::checkRot4Sym(Selection selection) const
{
	if (not selection.is_square())
		return false;

	if (selection.is_full_selection(h, w))
		return true;

	return check_glyph_pair(&Glyphs::left_side_bounds, { &GlyphIterator::move_right, &GlyphIterator::move_up }, { &Glyph::up, &Glyph::left }, rotate_90, selection)
	    && check_glyph_pair(&Glyphs::lower_side_bounds, { &GlyphIterator::move_up, &GlyphIterator::move_left }, { &Glyph::left, &Glyph::down }, rotate_90, selection)
	    && check_glyph_pair(&Glyphs::backward_diagonal, { &GlyphIterator::move_down, &GlyphIterator::move_up }, { &Glyph::left, &Glyph::right }, rotate_180, selection)
	;
}
bool Knot::checkFwdDiag(Selection selection) const
{
	if (not selection.is_square())
		return false;

	if (selection.is_full_selection(h, w))
		return true;

	return check_glyph_pair(&Glyphs::backward_diagonal, { &GlyphIterator::move_right, &GlyphIterator::move_up }, { &Glyph::up, &Glyph::right }, mirror_forward_diagonal, selection)
	    && check_glyph_pair(&Glyphs::backward_diagonal, { &GlyphIterator::move_down, &GlyphIterator::move_left }, { &Glyph::left, &Glyph::down }, mirror_forward_diagonal, selection)
	;
}
bool Knot::checkBackDiag(Selection selection) const
{
	if (not selection.is_square())
		return false;

	if (selection.is_full_selection(h, w))
		return true;

	return check_glyph_pair(&Glyphs::forward_diagonal, { &GlyphIterator::move_left, &GlyphIterator::move_up }, { &Glyph::up, &Glyph::left }, mirror_backward_diagonal, selection)
	    && check_glyph_pair(&Glyphs::forward_diagonal, { &GlyphIterator::move_down, &GlyphIterator::move_right }, { &Glyph::right, &Glyph::down }, mirror_backward_diagonal, selection)
	;
}
bool Knot::checkWrapping(Selection selection) const {
	// If the wrap in the Y direction is not enabled
	if (!wrapYEnabled) {
		// Only do the check if the selection either includes the top or bottom row, but not both
		if (selection.min.i == 0 && selection.max.i != h - 1) {
			for (int j = selection.min.j; j <= selection.max.j; j++)
				if (glyphs.at(selection.min.i, j)->up != Connection::EMPTY)
					return false;
		}
		else if (selection.min.i != 0 && selection.max.i == h - 1) {
			for (int j = selection.min.j; j <= selection.max.j; j++)
				if (glyphs.at(selection.max.i, j)->down != Connection::EMPTY)
					return false;
		}
	}

	// If the wrap in the X direction is not enabled
	if (!wrapXEnabled) {
		// Only do the check if the selection either includes the left or right column, but not both
		if (selection.min.j == 0 && selection.max.j != w - 1) {
			for (int i = selection.min.i; i <= selection.max.i; i++)
				if (glyphs.at(i, selection.min.j)->left != Connection::EMPTY)
					return false;
		}
		else if (selection.min.j != 0 && selection.max.j == w - 1) {
			for (int i = selection.min.i; i <= selection.max.i; i++)
				if (glyphs.at(i, selection.max.j)->right != Connection::EMPTY)
					return false;
		}
	}

	return true;
}

bool Knot::check_glyph_pair(iterator_pair_fn starting_pair, move_fn_pair moves, connection_pair connections, transform_fn transform, Selection selection) const
{
	const auto [move_first, move_second] = moves;
	const auto [first_connection, second_connection] = connections;

	for (auto [first, second] = (glyphs.*starting_pair)(selection); first.is_within(selection) && second.is_within(selection); (first.*move_first)(), (second.*move_second)())
	{
		if (first(first_connection) != transform(second(second_connection)))
			return false;
	}
	return true;
}

#include "pch.h"
#include "pure/GridSize.h"
#include "grid/Knot.h"
#include "pure/Glyph.h"
#include "pure/Selection.h"
#include "pure/SelectionZip.h"
#include "pure/Symmetry.h"
#include "grid/Tile.h" // This breaks the dependency direction between folders, todo change this later (maybe)
#include "Constants.h"

Knot::Knot(GridSize size, wxStatusBar* statusBar) : size(size), statusBar(statusBar), glyphs(size.rows, std::vector<const Glyph*>(size.columns, SpaceGlyph)) {}
Knot::Knot(Glyphs&& glyphs, wxStatusBar* statusBar) : size{ .rows = (int)glyphs.size(), .columns = (int)glyphs[0].size() }, statusBar(statusBar), glyphs(std::move(glyphs)) {}
wxUniChar Knot::get(const int i, const int j) const { return wxUniChar(glyphs[i][j]->code_point); }
CodePoint Knot::code_point(const int i, const int j) const { return glyphs[i][j]->code_point; }

void Knot::clear(Selection selection, const Tiles& tiles)
{
	for (int i = selection.min.i; i <= selection.max.i; i++)
	for (int j = selection.min.j; j <= selection.max.j; j++)
	{
		if (!tiles[i][j].locked())
			glyphs[i][j] = SpaceGlyph;
	}
}

bool Knot::generate(Algorithm alg, Symmetry sym, Selection selection, const Tiles& tiles)
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
	Glyphs base_glyphs = make_base_glyphs(sym, selection, tiles);

	switch (alg)
	{
	case fail_fast:
		return generate_fail_fast(std::move(base_glyphs), sym, selection);
	case backtracking:
	{
		auto glyphs_opt = generate_backtracking(std::move(base_glyphs), sym, selection);
		if (!glyphs_opt)
			return false;
		glyphs = std::move(*glyphs_opt);
		return true;
	}
	default:
		throw;
	}
}

bool Knot::generate_fail_fast(Glyphs base_glyphs, Symmetry sym, Selection selection)
{
	const wxString& prefix = status_prefix(sym);

	/// Next, enter a loop, counting the number of attempts made at generating this knot. The steps are as follows.
	for (int attempts = 1; attempts <= MAX_ATTEMPTS; attempts++) {
		/// \b (1) At certain intervals of numbers of attempts, update the status bar with the number of attempts made.
		if (attempts % ATTEMPTS_DISPLAY_INCREMENT == 0)
			statusBar->SetStatusText(wxString::Format("%sAttempt %i/%i", prefix, attempts, MAX_ATTEMPTS));

		/// \b (2) Call Knot::tryGenerating() using the copy of \c glyphs created above.
		///		If it fails, \c continue the loop and try again.
		std::optional<Glyphs> newGlyphs = try_generate_fail_fast(base_glyphs, sym, selection);
		if (!newGlyphs) continue;

		/// \b (3) If the knot has been successfully generated, set \c glyphs equal to this generated version and return \c true.
		glyphs = *newGlyphs;
		return true;
	}
	/// \b (4) If this loop has been completed, then the maximum number of attempts have been tried. Therefore return \c false.
	return false;
}

struct Step
{
	Step(Point primary, std::vector<const Glyph*>&& options)
		: primary(primary), options(std::move(options))
	{}

	Point primary;
	std::vector<const Glyph*> options;

	std::vector<std::pair<Point, const Glyph* Glyph::*>> transforms = {};
	std::size_t index = 0;
};

std::optional<Glyphs> Knot::generate_backtracking(Glyphs glyph_grid, Symmetry sym, Selection selection)
{
	const bool bitHori = sym % Symmetry::HoriSym;
	const bool bitVert = sym % Symmetry::VertSym;
	const bool bitRot2 = sym % Symmetry::Rot2Sym;
	const bool bitRot4 = sym % Symmetry::Rot4Sym;
	const bool bitFwDi = sym % Symmetry::FwdDiag;
	const bool bitBkDi = sym % Symmetry::BackDiag;

	const bool isEvenRows = (selection.max.i - selection.min.i + 1) % 2 == 0;
	const bool isEvenCols = (selection.max.j - selection.min.j + 1) % 2 == 0;
	const bool isAllRows = selection.min.i == 0 && selection.max.i == size.rows - 1;
	const bool isAllCols = selection.min.j == 0 && selection.max.j == size.columns - 1;
	const int iMid = (selection.min.i + selection.max.i) / 2;
	const int jMid = (selection.min.j + selection.max.j) / 2;
	const GlyphFlag midHoriFlag = isEvenRows ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const GlyphFlag midVertFlag = isEvenCols ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	const GlyphFlag midRot2Flag = isEvenRows ? (isEvenCols ? GlyphFlag::NONE : GlyphFlag::CT_ROT2D) : (isEvenCols ? GlyphFlag::CT_ROT2R : GlyphFlag::SA_ROT2);

	const bool isSquare = bitRot4 || bitFwDi || bitBkDi;
	const GlyphFlag midRot4Flag = isSquare ? (isEvenRows ? GlyphFlag::CT_ROT4R : !isEvenRows ? GlyphFlag::SA_ROT4 : GlyphFlag::NONE) : GlyphFlag::NONE;
	const GlyphFlag selfFlag = (GlyphFlag::CT_SELFD * (size.rows == 1)) | (GlyphFlag::CT_SELFR * (size.columns == 1)); // If this selection is only 1 row in length in either direction, flag appropriately

	const bool doWrapX = wrapXEnabled && (!isSquare || wrapYEnabled);
	const bool doWrapY = wrapYEnabled && (!isSquare || wrapXEnabled);



	const wxString& prefix = status_prefix(sym);
	int backtracking_attempts = 0;

	statusBar->SetStatusText(wxString::Format("%sBacktracked %i times", prefix, backtracking_attempts));


	std::list<Step> steps;
	bool backtracking = false;

	const SelectionRange range(selection);
	for (auto it = range.begin(); it != range.end(); )
	{
		Point p = *it;
		auto [i, j] = p;
		int iOffset = i - selection.min.i;
		int jOffset = j - selection.min.j;



		if (backtracking)
		{
			++backtracking_attempts;
			if (backtracking_attempts % 10000 == 0)
				statusBar->SetStatusText(wxString::Format("%sBacktracked %i times", prefix, backtracking_attempts));

			if (steps.empty())
				return std::nullopt;

			Step& step = steps.back();
			// Right here, step.primary == p
			assert(step.primary == p);

			glyph_grid[p.i][p.j] = nullptr;
			for (auto& [point, transform] : step.transforms)
			{
				glyph_grid[point.i][point.j] = nullptr;
			}

			++step.index;
			if (step.index == step.options.size())
			{
				steps.pop_back();
				--it;
				continue; // Keep backtracking
			}

			const Glyph* glyph = step.options[step.index];
			glyph_grid[p.i][p.j] = glyph;

			for (auto& [point, transform] : step.transforms)
			{
				glyph_grid[point.i][point.j] = glyph->*transform;
			}

			backtracking = false;
			++it;
			continue;
		}

		else if (glyph_grid[p.i][p.j])
		{
			++it;
			continue;
		}

		else
		{
			std::vector<const Glyph*> options = Glyph::RandomList(
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
					i == 0                ? (!doWrapY ? Connection::EMPTY : !glyph_grid[size.rows - 1][j]    ? Connection::DO_NOT_CARE : glyph_grid[size.rows - 1][j]->down)     : (!glyph_grid[i - 1][j] ? Connection::DO_NOT_CARE : glyph_grid[i - 1][j]->down)  ,
					i == size.rows - 1    ? (!doWrapY ? Connection::EMPTY : !glyph_grid[0][j]                ? Connection::DO_NOT_CARE : glyph_grid[0][j]->up)                   : (!glyph_grid[i + 1][j] ? Connection::DO_NOT_CARE : glyph_grid[i + 1][j]->up)    ,
					j == 0                ? (!doWrapX ? Connection::EMPTY : !glyph_grid[i][size.columns - 1] ? Connection::DO_NOT_CARE : glyph_grid[i][size.columns - 1]->right) : (!glyph_grid[i][j - 1] ? Connection::DO_NOT_CARE : glyph_grid[i][j - 1]->right) ,
					j == size.columns - 1 ? (!doWrapX ? Connection::EMPTY : !glyph_grid[i][0]                ? Connection::DO_NOT_CARE : glyph_grid[i][0]->left)                 : (!glyph_grid[i][j + 1] ? Connection::DO_NOT_CARE : glyph_grid[i][j + 1]->left)
				},
				/** The \c boolFlags argument in Glyph::Random() has different components added, under various conditions. 
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

			if (options.empty())
			{
				backtracking = true;
				--it;
				continue;
			}

			Step& step = steps.emplace_back(p, std::move(options));

			const Glyph* glyph = step.options[step.index];
			glyph_grid[p.i][p.j] = glyph;

			if (bitHori)
			{
				Point point = { selection.max.i - iOffset, j };
				step.transforms.emplace_back(point, &Glyph::mirror_x);
			}

			if (bitVert)
			{
				Point point = { i, selection.max.j - jOffset };
				step.transforms.emplace_back(point, &Glyph::mirror_y);
			}

			if (bitRot2)
			{
				Point point = { selection.max.i - iOffset, selection.max.j - jOffset };
				step.transforms.emplace_back(point, &Glyph::rotate_180);
			}

			if (bitRot4)
			{
				Point point = { selection.min.i + jOffset - iOffset, selection.max.j - iOffset };
				step.transforms.emplace_back(point, &Glyph::rotate_90);
				Point point2 = { selection.max.i - jOffset, selection.min.j + iOffset };
				step.transforms.emplace_back(point2, &Glyph::rotate_270);
			}

			if (bitFwDi)
			{
				Point point = { selection.max.i - jOffset, selection.max.j - iOffset };
				step.transforms.emplace_back(point, &Glyph::mirror_forward_diagonal);
			}

			if (bitBkDi)
			{
				Point point = { selection.min.i + jOffset, selection.min.j + iOffset };
				step.transforms.emplace_back(point, &Glyph::mirror_backward_diagonal);
			}

			for (auto& [point, transform] : step.transforms)
			{
				glyph_grid[point.i][point.j] = glyph->*transform;
			}

			++it;
			continue;
		}
	}

	return { std::move(glyph_grid) };
}

std::optional<Glyphs> Knot::try_generate_fail_fast(Glyphs glyphGrid, Symmetry sym, Selection selection) const
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
	const bool bitHori = sym % Symmetry::HoriSym;
	const bool bitVert = sym % Symmetry::VertSym;
	const bool bitRot2 = sym % Symmetry::Rot2Sym;
	const bool bitRot4 = sym % Symmetry::Rot4Sym;
	const bool bitFwDi = sym % Symmetry::FwdDiag;
	const bool bitBkDi = sym % Symmetry::BackDiag;

	const bool isEvenRows = (selection.max.i - selection.min.i + 1) % 2 == 0;
	const bool isEvenCols = (selection.max.j - selection.min.j + 1) % 2 == 0;
	const bool isAllRows = selection.min.i == 0 && selection.max.i == size.rows - 1;
	const bool isAllCols = selection.min.j == 0 && selection.max.j == size.columns -1;
	const int iMid = (selection.min.i + selection.max.i) / 2;
	const int jMid = (selection.min.j + selection.max.j) / 2;
	const GlyphFlag midHoriFlag = isEvenRows ? GlyphFlag::CT_MIRD : GlyphFlag::SA_MIRX;
	const GlyphFlag midVertFlag = isEvenCols ? GlyphFlag::CT_MIRR : GlyphFlag::SA_MIRY;
	const GlyphFlag midRot2Flag = isEvenRows ? (isEvenCols ? GlyphFlag::NONE : GlyphFlag::CT_ROT2D) : (isEvenCols ? GlyphFlag::CT_ROT2R : GlyphFlag::SA_ROT2);
	
	const bool isSquare = bitRot4 || bitFwDi || bitBkDi;
	const GlyphFlag midRot4Flag = isSquare ? (isEvenRows ? GlyphFlag::CT_ROT4R : !isEvenRows ? GlyphFlag::SA_ROT4 : GlyphFlag::NONE) : GlyphFlag::NONE;
	const GlyphFlag selfFlag = (GlyphFlag::CT_SELFD * (size.rows == 1)) | (GlyphFlag::CT_SELFR * (size.columns == 1)); // If this selection is only 1 row in length in either direction, flag appropriately
	
	const bool doWrapX = wrapXEnabled && (!isSquare || wrapYEnabled);
	const bool doWrapY = wrapYEnabled && (!isSquare || wrapXEnabled);

	/// Next, enter a loop over each location in the selection. Do the following.
	for (int i = selection.min.i, iOffset = 0; i <= selection.max.i; i++, iOffset++) {
		for (int j = selection.min.j, jOffset = 0; j <= selection.max.j; j++, jOffset++) {
			/// \b (1) If the Glyph in this location is already set, \c continue the loop.
			if (glyphGrid[i][j]) continue;

			/// \b (2) If the Glyph has yet to be set, generate a random \c Glyph for this location, with the \c GlyphFlag::NONE flag.
			///	For each of the 4 \c Connection parameters, there are many possible cases, implemented in a large nested ternary operation, described below.
			glyphGrid[i][j] = Glyph::Random(
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
					i == 0		? (!doWrapY ? Connection::EMPTY : !glyphGrid[size.rows - 1][j]	? Connection::DO_NOT_CARE : glyphGrid[size.rows - 1][j]->down)	: (!glyphGrid[i - 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i - 1][j]->down	) ,
					i == size.rows - 1	? (!doWrapY ? Connection::EMPTY : !glyphGrid[0][j]		? Connection::DO_NOT_CARE : glyphGrid[0][j]->up)		: (!glyphGrid[i + 1][j] ? Connection::DO_NOT_CARE : glyphGrid[i + 1][j]->up		) ,
					j == 0		? (!doWrapX ? Connection::EMPTY : !glyphGrid[i][size.columns - 1]	? Connection::DO_NOT_CARE : glyphGrid[i][size.columns - 1]->right) : (!glyphGrid[i][j - 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j - 1]->right	) ,
					j == size.columns - 1	? (!doWrapX ? Connection::EMPTY : !glyphGrid[i][0]		? Connection::DO_NOT_CARE : glyphGrid[i][0]->left)		: (!glyphGrid[i][j + 1] ? Connection::DO_NOT_CARE : glyphGrid[i][j + 1]->left	)
				},
				/** The \c boolFlags argument in Glyph::Random() has different components added, under various conditions. 
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
			if (!glyphGrid[i][j]) return std::nullopt;

			/// \b (4) If the function has made it to this point, then reflect and rotate the newly generated Glyph to the appropriate spots given the symmetry required.
			if (bitHori) glyphGrid[selection.max.i - iOffset][j] = glyphGrid[i][j]->mirror_x;
			if (bitVert) glyphGrid[i][selection.max.j - jOffset] = glyphGrid[i][j]->mirror_y;
			if (bitRot2) glyphGrid[selection.max.i - iOffset][selection.max.j - jOffset] = glyphGrid[i][j]->rotate_180;
			if (bitRot4) { glyphGrid[selection.min.i + jOffset][selection.max.j - iOffset] = glyphGrid[i][j]->rotate_90; glyphGrid[selection.max.i - jOffset][selection.min.j + iOffset] = glyphGrid[i][j]->rotate_270; }
			if (bitFwDi) glyphGrid[selection.max.i - jOffset][selection.max.j - iOffset] = glyphGrid[i][j]->mirror_forward_diagonal;
			if (bitBkDi) glyphGrid[selection.min.i + jOffset][selection.min.j + iOffset] = glyphGrid[i][j]->mirror_backward_diagonal;
		}
	}

	/// \b (5) If the loop finishes, then the Knot has been successfully generated. Return \c true.
	return { std::move(glyphGrid) };
}

Glyphs Knot::make_base_glyphs(const Symmetry sym, const Selection selection, const Tiles& tiles) const
{
	Glyphs base_glyphs = glyphs;

	/// Make a copy of \c glyphs, and set all members in the selection to \c nullptr to denote that they are unassigned.
	for (int i = selection.min.i; i <= selection.max.i; i++)
	for (int j = selection.min.j; j <= selection.max.j; j++)
	{
		if (tiles[i][j].locked())
			base_glyphs[i][j] = glyphs[i][j];
		else
			base_glyphs[i][j] = nullptr;
	}

	const auto transform = [&](const Symmetry desired_sym, const CornerMovement type1, const CornerMovement type2, const Glyph* GlyphsTransformed::* transformation) -> void
		{
			if (!(sym % desired_sym))
				return;
	
			for (const auto& [p1, p2] : SelectionZipRange{ selection, type1, type2 })
			{
				/// If a tile is locked and its transformed tile is not locked, also lock the transformed tile
				const Tile& t1 = tiles[p1.i][p1.j];
				const Tile& t2 = tiles[p2.i][p2.j];
				if (t1.locked() && !t2.locked())
				{
					base_glyphs[p2.i][p2.j] = base_glyphs[p1.i][p1.j]->*transformation;
				}
				else if (!t1.locked() && t2.locked())
				{
					base_glyphs[p1.i][p1.j] = base_glyphs[p2.i][p2.j]->*(GlyphsTransformed::inverse(transformation));
				}
			}
		};

	using enum Corner;
	using enum Movement;

	transform(Symmetry::HoriSym, (upper_left | right), (lower_left | right), &Glyph::mirror_x);
	transform(Symmetry::VertSym, (upper_left | down), (upper_right | down), &Glyph::mirror_y);

	transform(Symmetry::Rot2Sym, (upper_left | right), (lower_right | left), &Glyph::rotate_180);
	transform(Symmetry::Rot4Sym, (upper_left | right), (lower_left | up), &Glyph::rotate_90);
	transform(Symmetry::Rot4Sym, (upper_left | right), (upper_right | down), &Glyph::rotate_90);

	transform(Symmetry::FwdDiag, (upper_left | right), (lower_right | up), &Glyph::mirror_forward_diagonal);
	transform(Symmetry::BackDiag, (upper_right | left), (lower_left | up), &Glyph::mirror_backward_diagonal);

	return base_glyphs;
}

bool Knot::checkWrapping(Selection selection) const {
	// If the wrap in the Y direction is not enabled
	if (!wrapYEnabled) {
		// Only do the check if the selection either includes the top or bottom row, but not both
		if (selection.min.i == 0 && selection.max.i != size.rows - 1) {
			for (int j = selection.min.j; j <= selection.max.j; j++)
				if (glyphs[selection.min.i][j]->up != Connection::EMPTY)
					return false;
		}
		else if (selection.min.i != 0 && selection.max.i == size.rows - 1) {
			for (int j = selection.min.j; j <= selection.max.j; j++)
				if (glyphs[selection.max.i][j]->down != Connection::EMPTY)
					return false;
		}
	}

	// If the wrap in the X direction is not enabled
	if (!wrapXEnabled) {
		// Only do the check if the selection either includes the left or right column, but not both
		if (selection.min.j == 0 && selection.max.j != size.columns - 1) {
			for (int i = selection.min.i; i <= selection.max.i; i++)
				if (glyphs[i][selection.min.j]->left != Connection::EMPTY)
					return false;
		}
		else if (selection.min.j != 0 && selection.max.j == size.columns - 1) {
			for (int i = selection.min.i; i <= selection.max.i; i++)
				if (glyphs[i][selection.max.j]->right != Connection::EMPTY)
					return false;
		}
	}

	return true;
}

Symmetry Knot::symmetry_of(Selection selection, const Tiles& tiles) const
{
	return check_symmetry(glyphs, tiles, selection, size);
}

wxString Knot::plaintext() const
{
	wxString output;
	for (int i = 0; i < size.rows; i++)
	{
		for (int j = 0; j < size.columns; j++)
		{
			wxUniChar c = get(i, j);
			if (c == SpaceGlyph->code_point)
				output << wxUniChar(0x00A0); // Turn a regular space into a non-breaking space
			else
				output << c;
		}

		if (i != size.rows - 1)
			output << "\r\n";
	}
	return output;
}

const wxString& Knot::status_prefix(Symmetry sym) const
{
	switch (sym) {
	case Symmetry::AnySym:      { static wxString status = "Generating no symmetry... ";                    return status; }
	case Symmetry::HoriSym:     { static wxString status = "Generating horizontal symmetry... ";            return status; }
	case Symmetry::VertSym:     { static wxString status = "Generating vertical symmetry... ";              return status; }
	case Symmetry::HoriVertSym: { static wxString status = "Generating horizontal + vertical symmetry... "; return status; }
	case Symmetry::Rot2Sym:     { static wxString status = "Generating 2-way rotational symmetry... ";      return status; }
	case Symmetry::Rot4Sym:     { static wxString status = "Generating 4-way rotational symmetry... ";      return status; }
	case Symmetry::FwdDiag:     { static wxString status = "Generating forward diagonal symmetry... ";      return status; }
	case Symmetry::BackDiag:    { static wxString status = "Generating backward diagonal symmetry... ";     return status; }
	case Symmetry::FullSym:     { static wxString status = "Generating full symmetry... ";                  return status; }
	default:
		throw;
	}
}

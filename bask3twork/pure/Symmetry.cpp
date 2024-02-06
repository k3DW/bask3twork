#include "pch.h"
#include "pure/CornerMovement.h"
#include "pure/Glyph.h"
#include "pure/GridSize.h"
#include "pure/Selection.h"
#include "pure/SelectionZip.h"
#include "pure/Symmetry.h"
#include "grid/Tile.h" // This breaks the dependency direction between folders, todo change this later (maybe)



class SymmetryChecker
{
public:
	SymmetryChecker(const Glyphs& glyphs, const Tiles& tiles, Selection selection)
		: glyphs(&glyphs), tiles(&tiles), selection(selection)
	{}

	Symmetry connections(GridSize size) const;
	Symmetry locking(Symmetry connection_sym) const; // The parameter is used for short circuiting

private:
	using enum Corner;
	using enum Movement;

	template <ConnectionFn transform, Connection Connections::* lhs, Connection Connections::* rhs = lhs>
	bool check_connections(const SelectionZipRange& range) const;

	bool has_mirror_x_connections() const;
	bool has_mirror_y_connections() const;
	bool has_rotate_180_connections() const;
	bool has_rotate_90_connections() const;
	bool has_forward_diagonal_connections() const;
	bool has_backward_diagonal_connections() const;

	template <const Glyph* GlyphsTransformed::* transform>
	bool check_locking(const SelectionZipRange& range) const;

	bool has_mirror_x_locking() const;
	bool has_mirror_y_locking() const;
	bool has_rotate_180_locking() const;
	bool has_rotate_90_locking() const;
	bool has_forward_diagonal_locking() const;
	bool has_backward_diagonal_locking() const;

	const Glyph* glyph(Point p) const { return (*glyphs)[p.i][p.j]; }
	const Tile* tile(Point p) const { return &((*tiles)[p.i][p.j]); }

private:
	const Glyphs* glyphs;
	const Tiles* tiles;
	Selection selection;
};

Symmetry check_symmetry(const Glyphs& glyphs, const Tiles& tiles, Selection selection, GridSize size)
{
	const SymmetryChecker checker(glyphs, tiles, selection);
	const Symmetry connections = checker.connections(size);
	return checker.locking(connections);
}

Symmetry SymmetryChecker::connections(GridSize size) const
{
	if (selection.is_full_selection(size))
	{
		const Symmetry non_square = Symmetry::HoriSym | Symmetry::VertSym | Symmetry::Rot2Sym;
		const Symmetry square = selection.is_square() * (Symmetry::Rot4Sym | Symmetry::FwdDiag | Symmetry::BackDiag);
		return non_square | square;
	}

	const Symmetry non_square
		= (Symmetry::AnySym)
		| (Symmetry::HoriSym * has_mirror_x_connections())
		| (Symmetry::VertSym * has_mirror_y_connections())
		| (Symmetry::Rot2Sym * has_rotate_180_connections());

	if (!selection.is_square())
		return non_square;

	const Symmetry square
		= ((non_square % Symmetry::Rot2Sym) ? Symmetry::Rot4Sym * has_rotate_90_connections() : Symmetry::Nothing)
		| (Symmetry::FwdDiag * has_forward_diagonal_connections())
		| (Symmetry::BackDiag * has_backward_diagonal_connections());

	return non_square | square;
}

Symmetry SymmetryChecker::locking(Symmetry connection_sym) const
{
	return Symmetry::AnySym
		| (connection_sym % Symmetry::HoriSym  ? Symmetry::HoriSym  * has_mirror_x_locking()          : Symmetry::Nothing)
		| (connection_sym % Symmetry::VertSym  ? Symmetry::VertSym  * has_mirror_y_locking()          : Symmetry::Nothing)
		| (connection_sym % Symmetry::Rot2Sym  ? Symmetry::Rot2Sym  * has_rotate_180_locking()        : Symmetry::Nothing)
		| (connection_sym % Symmetry::Rot4Sym  ? Symmetry::Rot4Sym  * has_rotate_90_locking()         : Symmetry::Nothing)
		| (connection_sym % Symmetry::FwdDiag  ? Symmetry::FwdDiag  * has_forward_diagonal_locking()  : Symmetry::Nothing)
		| (connection_sym % Symmetry::BackDiag ? Symmetry::BackDiag * has_backward_diagonal_locking() : Symmetry::Nothing);
}



template <ConnectionFn transform, Connection Connections::* lhs, Connection Connections::* rhs>
bool SymmetryChecker::check_connections(const SelectionZipRange& range) const
{
	for (const auto& [p1, p2] : range)
	{
		if ((glyph(p1)->*lhs) != transform(glyph(p2)->*rhs))
			return false;
	}
	return true;
}

template <const Glyph* GlyphsTransformed::* transform>
bool SymmetryChecker::check_locking(const SelectionZipRange& range) const
{
	for (const auto& [p1, p2] : range)
	{
		if (tile(p1)->locked() && tile(p2)->locked() && glyph(p1) != (glyph(p2)->*transform))
			return false;
	}
	return true;
}



bool SymmetryChecker::has_mirror_x_connections() const
{
	return check_connections<mirror_x, &Glyph::left>             ({ selection.left_column(), upper_left | down, lower_left | up })
		&& check_connections<mirror_x, &Glyph::right>            ({ selection.right_column(), upper_right | down, lower_right | up })
		&& check_connections<mirror_x, &Glyph::up, &Glyph::down> ({ selection.upper_row(), upper_left | right, selection.lower_row(), lower_left | right });
}

bool SymmetryChecker::has_mirror_y_connections() const
{
	return check_connections<mirror_y, &Glyph::up>                  ({ selection.upper_row(), upper_left | right, upper_right | left })
		&& check_connections<mirror_y, &Glyph::down>                ({ selection.lower_row(), lower_left | right, lower_right | left })
		&& check_connections<mirror_y, &Glyph::left, &Glyph::right> ({ selection.left_column(), upper_left | down, selection.right_column(), upper_right | down });
}

bool SymmetryChecker::has_rotate_180_connections() const
{
	return check_connections<rotate_180, &Glyph::up, &Glyph::down>    ({ selection.upper_row(), upper_left | right, selection.lower_row(), lower_right| left })
		&& check_connections<rotate_180, &Glyph::left, &Glyph::right> ({ selection.left_column(), upper_left | down, selection.right_column(), lower_right | up });
}

bool SymmetryChecker::has_rotate_90_connections() const
{
	return check_connections<rotate_90, &Glyph::up, &Glyph::left>     ({ selection.upper_row(), upper_left | right, selection.left_column(), lower_left | up })
		&& check_connections<rotate_90, &Glyph::left, &Glyph::down>   ({ selection.left_column(), lower_left | up, selection.lower_row(), lower_right | left })
		&& check_connections<rotate_180, &Glyph::left, &Glyph::right> ({ selection.left_column(), upper_left | down, selection.right_column(), lower_right | up });
}

bool SymmetryChecker::has_forward_diagonal_connections() const
{
	return check_connections<mirror_forward_diagonal, &Glyph::up, &Glyph::right>  ({ selection.upper_row(), upper_left | right, selection.right_column(), lower_right | up })
		&& check_connections<mirror_forward_diagonal, &Glyph::left, &Glyph::down> ({ selection.left_column(), upper_left | down, selection.lower_row(), lower_right | left });
}

bool SymmetryChecker::has_backward_diagonal_connections() const
{
	return check_connections<mirror_backward_diagonal, &Glyph::up, &Glyph::left>    ({ selection.upper_row(), upper_right | left, selection.left_column(), lower_left | up })
		&& check_connections<mirror_backward_diagonal, &Glyph::right, &Glyph::down> ({ selection.right_column(), upper_right | down, selection.lower_row(), lower_left | right });
}



bool SymmetryChecker::has_mirror_x_locking() const
{
	return check_locking<&Glyph::mirror_x>({ selection, upper_left | right, lower_left | right });
}

bool SymmetryChecker::has_mirror_y_locking() const
{
	return check_locking<&Glyph::mirror_y>({ selection, upper_left | down, upper_right | down });
}

bool SymmetryChecker::has_rotate_180_locking() const
{
	return check_locking<&Glyph::rotate_180>({ selection, upper_left | right, lower_right | left });
}

bool SymmetryChecker::has_rotate_90_locking() const
{
	return check_locking<&Glyph::rotate_90>({ selection, upper_left | right, lower_left | up })
		&& check_locking<&Glyph::rotate_90>({ selection, upper_right | down, upper_left | right });
}

bool SymmetryChecker::has_forward_diagonal_locking() const
{
	return check_locking<&Glyph::mirror_forward_diagonal>({ selection, upper_left | right, lower_right | up });
}

bool SymmetryChecker::has_backward_diagonal_locking() const
{
	return check_locking<&Glyph::mirror_backward_diagonal>({ selection, upper_right | left, lower_left | up });
}

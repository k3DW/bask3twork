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

	Symmetry symmetry() const;

private:
	using enum Corner;
	using enum Movement;

	template <ConnectionFn transform, Connection Connections::* lhs, Connection Connections::* rhs = lhs>
	bool are_connections_compatible(const SelectionZipRange& range) const;

	Symmetry has_mirror_x_symmetry() const;
	Symmetry has_mirror_y_symmetry() const;
	Symmetry has_rotate_180_symmetry() const;
	Symmetry has_rotate_90_symmetry() const;
	Symmetry has_forward_diagonal_symmetry() const;
	Symmetry has_backward_diagonal_symmetry() const;

	const Glyph* glyph(Point p) const { return (*glyphs)[p.i][p.j]; }
	const Tile* tile(Point p) const { return (*tiles)[p.i][p.j]; }

private:
	const Glyphs* glyphs;
	const Tiles* tiles;
	Selection selection;
};

Symmetry check_symmetry(const Glyphs& glyphs, const Tiles& tiles, Selection selection, GridSize size)
{
	if (selection.is_full_selection(size))
	{
		Symmetry non_square = Symmetry::HoriSym | Symmetry::VertSym | Symmetry::Rot2Sym;
		Symmetry square = selection.is_square() * (Symmetry::Rot4Sym | Symmetry::FwdDiag | Symmetry::BackDiag);

		return non_square | square;
	}

	return SymmetryChecker(glyphs, tiles, selection).symmetry();
}

Symmetry SymmetryChecker::symmetry() const
{
	Symmetry non_square = has_mirror_x_symmetry() | has_mirror_y_symmetry() | has_rotate_180_symmetry();

	Symmetry square = Symmetry::Nothing;
	if (selection.is_square())
		square = has_rotate_90_symmetry() | has_forward_diagonal_symmetry() | has_backward_diagonal_symmetry();

	return Symmetry::AnySym | non_square | square;
}



template <ConnectionFn transform, Connection Connections::* lhs, Connection Connections::* rhs>
bool SymmetryChecker::are_connections_compatible(const SelectionZipRange& range) const
{
	for (const auto& [p1, p2] : range)
	{
		if ((glyph(p1)->*lhs) != transform(glyph(p2)->*rhs))
			return false;
	}
	return true;
}

Symmetry SymmetryChecker::has_mirror_x_symmetry() const
{
	return Symmetry::HoriSym *
	(
		   are_connections_compatible<mirror_x, &Glyph::left>             ({ selection.left_column(), upper_left | down, lower_left | up })
		&& are_connections_compatible<mirror_x, &Glyph::right>            ({ selection.right_column(), upper_right | down, lower_right | up })
		&& are_connections_compatible<mirror_x, &Glyph::up, &Glyph::down> ({ selection.upper_row(), upper_left | right, selection.lower_row(), lower_left | right })
	);
}

Symmetry SymmetryChecker::has_mirror_y_symmetry() const
{
	return Symmetry::VertSym *
	(
		   are_connections_compatible<mirror_y, &Glyph::up>                  ({ selection.upper_row(), upper_left | right, upper_right | left })
		&& are_connections_compatible<mirror_y, &Glyph::down>                ({ selection.lower_row(), lower_left | right, lower_right | left })
		&& are_connections_compatible<mirror_y, &Glyph::left, &Glyph::right> ({ selection.left_column(), upper_left | down, selection.right_column(), upper_right | down })
	);
}

Symmetry SymmetryChecker::has_rotate_180_symmetry() const
{
	return Symmetry::Rot2Sym *
	(
		   are_connections_compatible<rotate_180, &Glyph::up, &Glyph::down>    ({ selection.upper_row(), upper_left | right, selection.lower_row(), lower_right| left })
		&& are_connections_compatible<rotate_180, &Glyph::left, &Glyph::right> ({ selection.left_column(), upper_left | down, selection.right_column(), lower_right | up })
	);
}

Symmetry SymmetryChecker::has_rotate_90_symmetry() const
{
	return Symmetry::Rot4Sym *
	(
		   are_connections_compatible<rotate_90, &Glyph::up, &Glyph::left>     ({ selection.upper_row(), upper_left | right, selection.left_column(), lower_left | up })
		&& are_connections_compatible<rotate_90, &Glyph::left, &Glyph::down>   ({ selection.left_column(), lower_left | up, selection.lower_row(), lower_right | left })
		&& are_connections_compatible<rotate_180, &Glyph::left, &Glyph::right> ({ selection.left_column(), upper_left | down, selection.right_column(), lower_right | up })
	);
}

Symmetry SymmetryChecker::has_forward_diagonal_symmetry() const
{
	return Symmetry::FwdDiag *
	(
		   are_connections_compatible<mirror_forward_diagonal, &Glyph::up, &Glyph::right>  ({ selection.upper_row(), upper_left | right, selection.right_column(), lower_right | up })
		&& are_connections_compatible<mirror_forward_diagonal, &Glyph::left, &Glyph::down> ({ selection.left_column(), upper_left | down, selection.lower_row(), lower_right | left })
	);
}

Symmetry SymmetryChecker::has_backward_diagonal_symmetry() const
{
	return Symmetry::BackDiag *
	(
		   are_connections_compatible<mirror_backward_diagonal, &Glyph::up, &Glyph::left>    ({ selection.upper_row(), upper_right | left, selection.left_column(), lower_left | up })
		&& are_connections_compatible<mirror_backward_diagonal, &Glyph::right, &Glyph::down> ({ selection.right_column(), upper_right | down, selection.lower_row(), lower_left | right })
	);
}

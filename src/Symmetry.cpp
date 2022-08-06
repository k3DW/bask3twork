#include "Symmetry.h"

Symmetry SymmetryChecker::get(int h, int w) const
{
	if (selection.is_full_selection(h, w))
	{
		Symmetry non_square = Symmetry::HoriSym | Symmetry::VertSym | Symmetry::Rot2Sym;
		Symmetry square = selection.is_square() * (Symmetry::Rot4Sym | Symmetry::FwdDiag | Symmetry::BackDiag);

		return non_square | square;
	}

	Symmetry non_square = 
		(has_mirror_x_symmetry() | has_mirror_y_symmetry() | has_rotate_180_symmetry());

	Symmetry square = selection.is_square() *
		(has_rotate_90_symmetry() | has_forward_diagonal_symmetry() | has_backward_diagonal_symmetry());

	return Symmetry::AnySym | non_square | square;
}

template <ConnectionFn transform>
bool SymmetryChecker::glyph_range_compatible(Iterator lhs, Iterator rhs) const
{
	for (; selection.contains(lhs.point()) && selection.contains(rhs.point()); lhs.move(), rhs.move())
	{
		if (lhs.get() != transform(rhs.get()))
			return false;
	}
	return true;
}

Symmetry SymmetryChecker::has_mirror_x_symmetry() const
{
	return Symmetry::HoriSym *
	(
		   glyph_range_compatible<mirror_x> (starting_from_the(upper_left).checking_the(left_connection).moving(down),   starting_from_the(lower_left).checking_the(left_connection).moving(up))
		&& glyph_range_compatible<mirror_x> (starting_from_the(upper_right).checking_the(right_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
		&& glyph_range_compatible<mirror_x> (starting_from_the(upper_left).checking_the(up_connection).moving(right),    starting_from_the(lower_left).checking_the(down_connection).moving(right))
	);
}

Symmetry SymmetryChecker::has_mirror_y_symmetry() const
{
	return Symmetry::VertSym *
	(
		   glyph_range_compatible<mirror_y> (starting_from_the(upper_left).checking_the(up_connection).moving(right),   starting_from_the(upper_right).checking_the(up_connection).moving(left))
		&& glyph_range_compatible<mirror_y> (starting_from_the(lower_left).checking_the(down_connection).moving(right), starting_from_the(lower_right).checking_the(down_connection).moving(left))
		&& glyph_range_compatible<mirror_y> (starting_from_the(upper_left).checking_the(left_connection).moving(down),  starting_from_the(upper_right).checking_the(right_connection).moving(down))
	);
}

Symmetry SymmetryChecker::has_rotate_180_symmetry() const
{
	return Symmetry::Rot2Sym *
	(
		   glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_right).checking_the(down_connection).moving(left))
		&& glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
	);
}

Symmetry SymmetryChecker::has_rotate_90_symmetry() const
{
	return Symmetry::Rot4Sym *
	(
		   glyph_range_compatible<rotate_90>  (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_left).checking_the(left_connection).moving(up))
		&& glyph_range_compatible<rotate_90>  (starting_from_the(lower_left).checking_the(left_connection).moving(up),   starting_from_the(lower_right).checking_the(down_connection).moving(left))
		&& glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
	);
}

Symmetry SymmetryChecker::has_forward_diagonal_symmetry() const
{
	return Symmetry::FwdDiag *
	(
		   glyph_range_compatible<mirror_forward_diagonal> (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_right).checking_the(right_connection).moving(up))
		&& glyph_range_compatible<mirror_forward_diagonal> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(down_connection).moving(left))
	);
}

Symmetry SymmetryChecker::has_backward_diagonal_symmetry() const
{
	return Symmetry::BackDiag *
	(
		   glyph_range_compatible<mirror_backward_diagonal> (starting_from_the(upper_right).checking_the(up_connection).moving(left),    starting_from_the(lower_left).checking_the(left_connection).moving(up))
		&& glyph_range_compatible<mirror_backward_diagonal> (starting_from_the(upper_right).checking_the(right_connection).moving(down), starting_from_the(lower_left).checking_the(down_connection).moving(right))
	);
}

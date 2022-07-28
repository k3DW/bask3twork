#include "SymmetryChecker.h"

Symmetry SymmetryChecker::get() const
{
	Symmetry non_square = has_mirror_x_symmetry()
	                    | has_mirror_y_symmetry()
	                    | has_rotate_180_symmetry();

	Symmetry square = selection.is_square()
	    ? has_rotate_90_symmetry()
	    | has_forward_diagonal_symmetry()
	    | has_backward_diagonal_symmetry()
	: Symmetry::Off;

	return Symmetry::NoSym | non_square | square;
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
	return glyph_range_compatible<mirror_x> (starting_from_the(upper_left).checking_the(left_connection).moving(down),   starting_from_the(lower_left).checking_the(left_connection).moving(up))
	    && glyph_range_compatible<mirror_x> (starting_from_the(upper_right).checking_the(right_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
	    && glyph_range_compatible<mirror_x> (starting_from_the(upper_left).checking_the(up_connection).moving(right),    starting_from_the(lower_left).checking_the(down_connection).moving(right))
	? Symmetry::HoriSym : Symmetry::Off;
}

Symmetry SymmetryChecker::has_mirror_y_symmetry() const
{
	return glyph_range_compatible<mirror_y> (starting_from_the(upper_left).checking_the(up_connection).moving(right),   starting_from_the(upper_right).checking_the(up_connection).moving(left))
	    && glyph_range_compatible<mirror_y> (starting_from_the(lower_left).checking_the(down_connection).moving(right), starting_from_the(lower_right).checking_the(down_connection).moving(left))
		&& glyph_range_compatible<mirror_y> (starting_from_the(upper_left).checking_the(left_connection).moving(down),  starting_from_the(upper_right).checking_the(right_connection).moving(down))
	? Symmetry::VertSym : Symmetry::Off;
}

Symmetry SymmetryChecker::has_rotate_180_symmetry() const
{
	return glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_right).checking_the(down_connection).moving(left))
		&& glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
	? Symmetry::Rot2Sym : Symmetry::Off;
}

Symmetry SymmetryChecker::has_rotate_90_symmetry() const
{
	return glyph_range_compatible<rotate_90>  (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_left).checking_the(left_connection).moving(up))
	    && glyph_range_compatible<rotate_90>  (starting_from_the(lower_left).checking_the(left_connection).moving(up),   starting_from_the(lower_right).checking_the(down_connection).moving(left))
	    && glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
	? Symmetry::Rot4Sym : Symmetry::Off;
}

Symmetry SymmetryChecker::has_forward_diagonal_symmetry() const
{
	return glyph_range_compatible<mirror_forward_diagonal> (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_right).checking_the(right_connection).moving(up))
	    && glyph_range_compatible<mirror_forward_diagonal> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(down_connection).moving(left))
	? Symmetry::FwdDiag : Symmetry::Off;
}

Symmetry SymmetryChecker::has_backward_diagonal_symmetry() const
{
	return glyph_range_compatible<mirror_backward_diagonal> (starting_from_the(upper_right).checking_the(up_connection).moving(left),    starting_from_the(lower_left).checking_the(left_connection).moving(up))
	    && glyph_range_compatible<mirror_backward_diagonal> (starting_from_the(upper_right).checking_the(right_connection).moving(down), starting_from_the(lower_left).checking_the(down_connection).moving(right))
	? Symmetry::BackDiag : Symmetry::Off;
}
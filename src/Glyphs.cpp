#include "Glyphs.h"

template <ConnectionFn transform>
bool SymmetryChecker::glyph_range_compatible(Iterator lhs, Iterator rhs) const
{
	for (; selection.contains(lhs) && selection.contains(rhs); ++lhs, ++rhs)
	{
		if (lhs.get_connection() != transform(rhs.get_connection()))
			return false;
	}
	return true;
}

bool SymmetryChecker::has_mirror_x_symmetry() const
{
	return glyph_range_compatible<mirror_x> (starting_from_the(upper_left).checking_the(left_connection).moving(down),   starting_from_the(lower_left).checking_the(left_connection).moving(up))
	    && glyph_range_compatible<mirror_x> (starting_from_the(upper_right).checking_the(right_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
	    && glyph_range_compatible<mirror_x> (starting_from_the(upper_left).checking_the(up_connection).moving(right),    starting_from_the(lower_left).checking_the(down_connection).moving(right))
	;
}
bool SymmetryChecker::has_mirror_y_symmetry() const
{
	return glyph_range_compatible<mirror_y> (starting_from_the(upper_left).checking_the(up_connection).moving(right),   starting_from_the(upper_right).checking_the(up_connection).moving(left))
	    && glyph_range_compatible<mirror_y> (starting_from_the(lower_left).checking_the(down_connection).moving(right), starting_from_the(lower_right).checking_the(down_connection).moving(left))
		&& glyph_range_compatible<mirror_y> (starting_from_the(upper_left).checking_the(left_connection).moving(down),  starting_from_the(upper_right).checking_the(right_connection).moving(down))
	;
}
bool SymmetryChecker::has_rotate_180_symmetry() const
{
	return glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_right).checking_the(down_connection).moving(left))
		&& glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
	;
}
bool SymmetryChecker::has_rotate_90_symmetry() const
{
	return glyph_range_compatible<rotate_90>  (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_left).checking_the(left_connection).moving(up))
	    && glyph_range_compatible<rotate_90>  (starting_from_the(lower_left).checking_the(left_connection).moving(up),   starting_from_the(lower_right).checking_the(down_connection).moving(left))
	    && glyph_range_compatible<rotate_180> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(right_connection).moving(up))
	;
}
bool SymmetryChecker::has_forward_diagonal_symmetry() const
{
	return glyph_range_compatible<mirror_forward_diagonal> (starting_from_the(upper_left).checking_the(up_connection).moving(right),  starting_from_the(lower_right).checking_the(right_connection).moving(up))
	    && glyph_range_compatible<mirror_forward_diagonal> (starting_from_the(upper_left).checking_the(left_connection).moving(down), starting_from_the(lower_right).checking_the(down_connection).moving(left))
	;
}
bool SymmetryChecker::has_backward_diagonal_symmetry() const
{
	return glyph_range_compatible<mirror_backward_diagonal> (starting_from_the(upper_right).checking_the(up_connection).moving(left),  starting_from_the(lower_left).checking_the(left_connection).moving(up))
	    && glyph_range_compatible<mirror_backward_diagonal> (starting_from_the(upper_right).checking_the(right_connection).moving(down), starting_from_the(lower_left).checking_the(down_connection).moving(right))
	;
}




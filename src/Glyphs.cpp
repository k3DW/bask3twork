#include "Glyphs.h"

Symmetry Glyphs::symmetry_of(Selection selection) const
{
	return (Symmetry::HoriSym * has_mirror_x_symmetry(selection))
	     | (Symmetry::VertSym * has_mirror_y_symmetry(selection))
	     | (Symmetry::Rot4Sym * has_rotate_90_symmetry(selection))
	     | (Symmetry::Rot2Sym * has_rotate_180_symmetry(selection))
	     | (Symmetry::FwdDiag * has_forward_diagonal_symmetry(selection))
	     | (Symmetry::BackDiag * has_backward_diagonal_symmetry(selection))
	;
}

namespace
{
	using iterator_pair_fn = std::pair<GlyphIterator, GlyphIterator>(Glyphs::*)(Selection) const;
	using move_fn_pair = std::pair<GlyphIterator& (GlyphIterator::*)(), GlyphIterator& (GlyphIterator::*)()>;
	using connection_pair = std::pair<Connection Glyph::*, Connection Glyph::*>;
	using transform_fn = Connection(*)(Connection);

	bool check_glyph_pairs(const Glyphs& glyphs, iterator_pair_fn starting_pair, move_fn_pair moves, connection_pair connections, transform_fn transform, Selection selection)
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
}

bool Glyphs::has_mirror_x_symmetry(Selection selection) const
{
	return check_glyph_pairs(*this, &Glyphs::left_side_bounds, { &GlyphIterator::move_down, &GlyphIterator::move_up }, { &Glyph::left, &Glyph::left }, mirror_x, selection)
	    && check_glyph_pairs(*this, &Glyphs::right_side_bounds, { &GlyphIterator::move_down, &GlyphIterator::move_up }, { &Glyph::right, &Glyph::right }, mirror_x, selection)
	    && check_glyph_pairs(*this, &Glyphs::left_side_bounds, { &GlyphIterator::move_right, &GlyphIterator::move_right }, { &Glyph::up, &Glyph::down }, mirror_x, selection)
	;
}
bool Glyphs::has_mirror_y_symmetry(Selection selection) const
{
	return check_glyph_pairs(*this, &Glyphs::upper_side_bounds, { &GlyphIterator::move_right, &GlyphIterator::move_left }, { &Glyph::up, &Glyph::up }, mirror_y, selection)
	    && check_glyph_pairs(*this, &Glyphs::lower_side_bounds, { &GlyphIterator::move_right, &GlyphIterator::move_left }, { &Glyph::down, &Glyph::down }, mirror_y, selection)
	    && check_glyph_pairs(*this, &Glyphs::upper_side_bounds, { &GlyphIterator::move_down, &GlyphIterator::move_down }, { &Glyph::left, &Glyph::right }, mirror_y, selection)
	;
}
bool Glyphs::has_rotate_90_symmetry(Selection selection) const
{
	return check_glyph_pairs(*this, &Glyphs::left_side_bounds, { &GlyphIterator::move_right, &GlyphIterator::move_up }, { &Glyph::up, &Glyph::left }, rotate_90, selection)
	    && check_glyph_pairs(*this, &Glyphs::lower_side_bounds, { &GlyphIterator::move_up, &GlyphIterator::move_left }, { &Glyph::left, &Glyph::down }, rotate_90, selection)
	    && check_glyph_pairs(*this, &Glyphs::backward_diagonal, { &GlyphIterator::move_down, &GlyphIterator::move_up }, { &Glyph::left, &Glyph::right }, rotate_180, selection)
	;
}
bool Glyphs::has_rotate_180_symmetry(Selection selection) const
{
	return check_glyph_pairs(*this, &Glyphs::backward_diagonal, { &GlyphIterator::move_right, &GlyphIterator::move_left }, { &Glyph::up, &Glyph::down }, rotate_180, selection)
	    && check_glyph_pairs(*this, &Glyphs::backward_diagonal, { &GlyphIterator::move_down, &GlyphIterator::move_up }, { &Glyph::left, &Glyph::right }, rotate_180, selection)
	;
}
bool Glyphs::has_forward_diagonal_symmetry(Selection selection) const
{
	return check_glyph_pairs(*this, &Glyphs::backward_diagonal, { &GlyphIterator::move_right, &GlyphIterator::move_up }, { &Glyph::up, &Glyph::right }, mirror_forward_diagonal, selection)
	    && check_glyph_pairs(*this, &Glyphs::backward_diagonal, { &GlyphIterator::move_down, &GlyphIterator::move_left }, { &Glyph::left, &Glyph::down }, mirror_forward_diagonal, selection)
	;
}
bool Glyphs::has_backward_diagonal_symmetry(Selection selection) const
{
	return check_glyph_pairs(*this, &Glyphs::forward_diagonal, { &GlyphIterator::move_left, &GlyphIterator::move_up }, { &Glyph::up, &Glyph::left }, mirror_backward_diagonal, selection)
	    && check_glyph_pairs(*this, &Glyphs::forward_diagonal, { &GlyphIterator::move_down, &GlyphIterator::move_right }, { &Glyph::right, &Glyph::down }, mirror_backward_diagonal, selection)
	;
}

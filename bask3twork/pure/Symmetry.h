#pragma once
#include "pure/CornerMovement.h"
#include "pure/Glyph.h"
#include "pure/GridSize.h"
#include "pure/Selection.h"
#include "pure/UsableEnum.h"

/** All the types of symmetry which can be used in this program.
 * The bit flags tell \c Knot::generate() how to generate.
 * In order from least-significant to most-significant bit, the bits mean the following.
 * (0) The symmetry is being used at all.
 * (1) The symmetry includes horizontal reflection.
 * (2) The symmetry includes vertical reflection.
 * (3) The symmetry includes 2-fold rotation.
 * (4) The symmetry includes 4-fold rotation.
 * (5) The symmetry includes forward diagonal reflection.
 * (6) The symmetry includes backward diagonal reflection.
 */
enum class Symmetry
{
	Nothing     = 0b0000000, ///< This is when a knot cannot be generated
	AnySym      = 0b0000001, ///< Any symmetry, including no symmetry
	HoriSym     = 0b0000011, ///< Mirror symmetry across the horizontal axis
	VertSym     = 0b0000101, ///< Mirror symmetry across the vertical axis
	HoriVertSym = 0b0001111, ///< Mirror symmetry across both the horizontal and vertical axes
	Rot2Sym     = 0b0001001, ///< 2-way rotational symmetry
	Rot4Sym     = 0b0011001, ///< 4-way rotational symmetry (square only)
	FwdDiag     = 0b0100001, ///< Mirror symmetry across the forward diagonal (square only)
	BackDiag    = 0b1000001, ///< Mirror symmetry across the backward diagonal (square only)
	FullSym     = 0b1111111, ///< 4-fold dihedral symmetry (square only)
};

template <> struct opt_into_enum_operations<Symmetry> : std::true_type {};


class SymmetryChecker
{
public:
	SymmetryChecker(const Glyphs& glyphs, Selection selection)
		: glyphs(&glyphs), selection(selection)
	{}

	Symmetry get(GridSize size) const;

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

private:
	const Glyphs* glyphs;
	Selection selection;
};

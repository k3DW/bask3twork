#pragma once
#include "Glyph.h"
#include "Selection.h"

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

constexpr inline Symmetry operator&(Symmetry lhs, Symmetry rhs) { return static_cast<Symmetry>(static_cast<int>(lhs) & static_cast<int>(rhs)); }
constexpr inline Symmetry operator|(Symmetry lhs, Symmetry rhs) { return static_cast<Symmetry>(static_cast<int>(lhs) | static_cast<int>(rhs)); }

constexpr inline Symmetry operator*(Symmetry sym, bool b) { return static_cast<Symmetry>(static_cast<int>(sym) * b); }

class SymmetryChecker
{
public:
	SymmetryChecker(const Glyphs& glyphs, Selection selection)
		: glyphs(&glyphs), selection(selection)
		, corners{ IteratorBuilder(selection.min, &glyphs), IteratorBuilder(Point{ selection.min.i, selection.max.j }, &glyphs), IteratorBuilder(Point{ selection.max.i, selection.min.j }, &glyphs), IteratorBuilder(selection.max, &glyphs), }
	{}

	Symmetry get(int h, int w) const;

private:
	enum class CornerType { upper_left, upper_right, lower_left, lower_right, };
	using enum CornerType;

	enum class ConnectionType { up_connection, down_connection, left_connection, right_connection, };
	using enum ConnectionType;

	enum class MovementType { up, down, left, right, };
	using enum MovementType;

	static constexpr std::array connections{ &Glyph::up, &Glyph::down, &Glyph::left, &Glyph::right, };
	static constexpr std::array movements{ Point{ -1, 0 }, Point{ 1, 0 }, Point{ 0, -1 }, Point{ 0, 1 }, };

	struct IteratorArgs
	{
		Point position;
		Point movement = Point{ 0, 0 };
		Connection Glyph::* connection = nullptr;
		const Glyphs* glyphs;
	};

	class Iterator : private IteratorArgs
	{
	public:
		Iterator(const IteratorArgs& args) : IteratorArgs(args) {}

		Point      point() const { return position; }
		Connection get() const   { return (*glyphs)[position.i][position.j]->*connection; }
		Iterator&  move()        { position.i += movement.i; position.j += movement.j; return *this; }
	};

	class IteratorBuilder : private IteratorArgs
	{
	public:
		IteratorBuilder(Point start, const Glyphs* glyphs) : IteratorArgs{ .position = start, .glyphs = glyphs } {}

		operator Iterator() const { return Iterator(*this); }

		IteratorBuilder& checking_the(ConnectionType con) { connection = connections[static_cast<int>(con)]; return *this; }
		IteratorBuilder& moving(MovementType move)        { movement = movements[static_cast<int>(move)]; return *this; }
	};

	IteratorBuilder starting_from_the(CornerType corner) const
	{
		return corners[static_cast<int>(corner)];
	}

	template <ConnectionFn transform>
	bool glyph_range_compatible(Iterator lhs, Iterator rhs) const;

	Symmetry has_mirror_x_symmetry() const;
	Symmetry has_mirror_y_symmetry() const;
	Symmetry has_rotate_180_symmetry() const;
	Symmetry has_rotate_90_symmetry() const;
	Symmetry has_forward_diagonal_symmetry() const;
	Symmetry has_backward_diagonal_symmetry() const;

private:
	const Glyphs* glyphs;
	Selection selection;
	std::array<IteratorBuilder, 4> corners;
};

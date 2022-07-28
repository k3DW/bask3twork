#pragma once
#include "Glyph.h"
#include "Selection.h"

class SymmetryChecker
{
public:
	SymmetryChecker(const Glyphs& glyphs, Selection selection)
		: glyphs(&glyphs), selection(selection)
		, corners{ IteratorBuilder(selection.min, &glyphs), IteratorBuilder(Point{ selection.min.i, selection.max.j }, &glyphs), IteratorBuilder(Point{ selection.max.i, selection.min.j }, &glyphs), IteratorBuilder(selection.max, &glyphs), }
	{}

	Symmetry get() const;

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

	class IteratorBuilder : public IteratorArgs
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

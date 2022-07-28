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

	class Iterator
	{
	public:
		Iterator(Point position, Point movement, Connection Glyph::* connection, const Glyphs* glyphs)
			: position(position), movement(movement), connection(connection), glyphs(glyphs) {}

		operator Point() const { return position; }

		Iterator& operator++() { position.i += movement.i; position.j += movement.j; return *this; }

		Connection get_connection() { return (*glyphs)[position.i][position.j]->*connection; }

	private:
		Point position;
		Point movement;
		Connection Glyph::* connection;
		const Glyphs* glyphs;
	};

	class IteratorBuilder
	{
	public:
		IteratorBuilder(Point start, const Glyphs* glyphs)
			: start(start), movement{ 0, 0 }, connection(nullptr), glyphs(glyphs) {}

		operator Iterator() const { return Iterator(start, movement, connection, glyphs); }

		IteratorBuilder& checking_the(ConnectionType con) { connection = connections[static_cast<int>(con)]; return *this; }
		IteratorBuilder& moving(MovementType move)        { movement = movements[static_cast<int>(move)]; return *this; }

	private:
		Point start;
		Point movement;
		Connection Glyph::* connection;
		const Glyphs* glyphs;
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

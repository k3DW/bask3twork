#pragma once
#include "Glyph.h"

struct Point
{
	int i;
	int j;
};

struct Selection
{
	Point min;
	Point max;

	bool is_square() const { return max.i - min.i == max.j - min.j; }
	bool is_full_selection(int rows, int columns) const { return min.i == 0 && min.j == 0 && max.i == rows - 1 && max.j == columns - 1; }

	bool contains(Point point) const { return point.i >= min.i && point.i <= max.i && point.j >= min.j && point.j <= max.j; }
	bool contains(Point p1, Point p2) const { return contains(p1) && contains(p2); }
};

class Glyphs
{
public:
	Glyphs(std::size_t rows, std::size_t columns) : data(rows, std::vector<const Glyph*>(columns, DefaultGlyph)) {}
	Glyphs(std::vector<std::vector<const Glyph*>> data) : data(data) {}

	Symmetry symmetry_of(Selection selection) const;

	const auto& operator[](size_t pos) const { return data[pos]; }
	auto&       operator[](size_t pos)       { return data[pos]; }

	const Glyph*  at(int i, int j) const { return data[i][j]; }
	const Glyph*& at(int i, int j)       { return data[i][j]; }

	std::size_t rows()    const { return data.size(); }
	std::size_t columns() const { return data[0].size(); }

private:
	std::vector<std::vector<const Glyph*>> data;
};

class SymmetryChecker
{
public:
	SymmetryChecker(const Glyphs* glyphs, Selection selection)
		: glyphs(glyphs), selection(selection)
		, corners{ IteratorBuilder(selection.min, glyphs), IteratorBuilder(Point{ selection.min.i, selection.max.j }, glyphs), IteratorBuilder(Point{ selection.max.i, selection.min.j }, glyphs), IteratorBuilder(selection.max, glyphs), }
	{}

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

		Connection get_connection() { return glyphs->at(position.i, position.j)->*connection; }

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

	bool has_mirror_x_symmetry() const;
	bool has_mirror_y_symmetry() const;
	bool has_rotate_180_symmetry() const;
	bool has_rotate_90_symmetry() const;
	bool has_forward_diagonal_symmetry() const;
	bool has_backward_diagonal_symmetry() const;

private:
	const Glyphs* glyphs;
	Selection selection;
	std::array<IteratorBuilder, 4> corners;
};

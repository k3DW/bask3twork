#pragma once
#include "pure/CornerMovement.h"
#include "pure/GridSize.h"

struct Point
{
	int i;
	int j;

	friend constexpr bool operator==(Point, Point) = default;
	
	friend constexpr Point operator+(Point lhs, Point rhs)
	{
		return { lhs.i + rhs.i, lhs.j + rhs.j };
	}

	friend constexpr Point operator-(Point lhs, Point rhs)
	{
		return { lhs.i - rhs.i, lhs.j - rhs.j };
	}

	constexpr Point& operator+=(Point that)
	{
		return *this = *this + that;
	}

	constexpr Point& operator-=(Point that)
	{
		return *this = *this - that;
	}

	constexpr Point operator-() const
	{
		return { -i, -j };
	}

	static consteval Point up()    { return { -1, 0 }; }
	static consteval Point down()  { return { 1, 0 }; }
	static consteval Point left()  { return { 0, -1 }; }
	static consteval Point right() { return { 0, 1 }; }

	static constexpr Point movement(Movement type)
	{
		switch (type)
		{
		case Movement::up:    return up();
		case Movement::down:  return down();
		case Movement::left:  return left();
		case Movement::right: return right();
		}
		throw;
	}
};

struct Selection
{
	Point min;
	Point max;

	friend constexpr bool operator==(const Selection&, const Selection&) = default;

	int rows() const { return max.i - min.i + 1; }
	int columns() const { return max.j - min.j + 1; }
	bool is_square() const { return max.i - min.i == max.j - min.j; }
	bool is_full_selection(GridSize size) const { return min.i == 0 && min.j == 0 && max.i == size.rows - 1 && max.j == size.columns - 1; }

	bool contains(Point point) const { return point.i >= min.i && point.i <= max.i && point.j >= min.j && point.j <= max.j; }

	Point upper_left()  const { return min; }
	Point upper_right() const { return { min.i, max.j }; }
	Point lower_left()  const { return { max.i, min.j }; }
	Point lower_right() const { return max; }

	Point corner(Corner type) const
	{
		switch (type)
		{
		case Corner::upper_left:  return upper_left();
		case Corner::upper_right: return upper_right();
		case Corner::lower_left:  return lower_left();
		case Corner::lower_right: return lower_right();
		}
		throw;
	}

	Selection upper_row()    const { return { { min.i, min.j }, { min.i, max.j } }; }
	Selection lower_row()    const { return { { max.i, min.j }, { max.i, max.j } }; }
	Selection left_column()  const { return { { min.i, min.j }, { max.i, min.j } }; }
	Selection right_column() const { return { { min.i, max.j }, { max.i, max.j } }; }
};

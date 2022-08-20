#pragma once

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
};

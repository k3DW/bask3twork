#pragma once

struct GridSize
{
	int rows;
	int columns;

	int area() const { return rows * columns; }

	friend constexpr bool operator==(GridSize, GridSize) = default;
};

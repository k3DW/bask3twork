#pragma once

struct GridSize
{
	int rows;
	int columns;

	int area() const { return rows * columns; }
};

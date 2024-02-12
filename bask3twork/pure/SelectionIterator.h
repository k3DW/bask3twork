#pragma once
#include "pure/CornerMovement.h"
#include "pure/Selection.h"
#include <stdexcept>
#include <vector>

struct SelectionSentinel {};

class SelectionIterator
{
	using Self = SelectionIterator;

public:
	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = std::ptrdiff_t; // Unused
	using value_type = Point;
	using pointer    = const Point*;
	using reference  = const Point&;

	SelectionIterator() = default;

	SelectionIterator(Selection selection, CornerMovement type)
		: selection(selection)
	{
		set_points(type);
		static_assert(std::bidirectional_iterator<Self>);
		static_assert(!std::random_access_iterator<Self>);
	}

	friend bool operator==(const Self& lhs, const Self& rhs)
	{
		if (lhs.constants() != rhs.constants())
			throw std::logic_error("Trying to compare iterators into 2 different grid ranges.");
		return lhs.current == rhs.current;
	}

	bool operator==(SelectionSentinel) const
	{
		return !selection.contains(current);
	}

	reference operator*() const
	{
		return current;
	}

	Self& operator++()
	{
		current += movement;
		if (!selection.contains(current))
		{
			current += movement_2;
		}
		return *this;
	}

	Self operator++(int)
	{
		auto temp = *this;
		++(*this);
		return temp;
	}

	Self& operator--()
	{
		current -= movement;
		if (!selection.contains(current))
		{
			current -= movement_2;
		}
		return *this;
	}

	Self operator--(int)
	{
		auto temp = *this;
		--(*this);
		return temp;
	}

private:
	void set_points(const CornerMovement type)
	{
		using enum Corner;
		using enum Movement;

		current = selection.corner(type.corner);
		movement = Point::movement(type.movement);

		switch (type)
		{
		break; case upper_left | down:
			movement_2 = { -selection.rows(), 1 };
		break; case upper_left | right:
			movement_2 = { 1, -selection.columns() };
		break; case upper_right | down:
			movement_2 = { -selection.rows(), -1 };
		break; case upper_right | left:
			movement_2 = { 1, selection.columns() };
		break; case lower_left | up:
			movement_2 = { selection.rows(), 1 };
		break; case lower_left | right:
			movement_2 = { -1, -selection.columns() };
		break; case lower_right | up:
			movement_2 = { selection.rows(), -1 };
		break; case lower_right | left:
			movement_2 = { -1, selection.columns() };
		break; default:
			throw std::logic_error("Incorrect combination of Corner and Movement.");
		}
	}

	std::tuple<const Point&, const Point&, const Selection&> constants() const
	{
		return std::tie(movement, movement_2, selection);
	}

	Point current = { -1, -1 };
	Point movement = { 1 << 16, 1 << 16 };
	Point movement_2 = { 1 << 16, 1 << 16 };

	Selection selection = { { -1, -1 }, { -1, -1 } };
};

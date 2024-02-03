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
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t; // Unused
	using value_type = Point;
	using pointer    = const Point*;
	using reference  = const Point&;

	SelectionIterator() = default;

	SelectionIterator(Selection selection, CornerMovement type)
		: selection(selection)
	{
		set_points(type);
		static_assert(std::forward_iterator<Self>);
		static_assert(!std::bidirectional_iterator<Self>);
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
			baseline_current += baseline_movement;
			current = baseline_current;
		}
		return *this;
	}

	Self operator++(int)
	{
		auto temp = *this;
		++(*this);
		return temp;
	}

private:
	void set_points(const CornerMovement type)
	{
		using enum Corner;
		using enum Movement;

		current = baseline_current = selection.corner(type.corner);
		movement = Point::movement(type.movement);

		switch (type)
		{
		break; case upper_left | down:
			baseline_movement = Point::right();
		break; case upper_left | right:
			baseline_movement = Point::down();
		break; case upper_right | down:
			baseline_movement = Point::left();
		break; case upper_right | left:
			baseline_movement = Point::down();
		break; case lower_left | up:
			baseline_movement = Point::right();
		break; case lower_left | right:
			baseline_movement = Point::up();
		break; case lower_right | up:
			baseline_movement = Point::left();
		break; case lower_right | left:
			baseline_movement = Point::up();
		break; default:
			throw std::logic_error("Incorrect combination of Corner and Movement.");
		}
	}

	std::tuple<const Point&, const Point&, const Selection&> constants() const
	{
		return std::tie(movement, baseline_movement, selection);
	}

	Point current = { -1, -1 };
	Point movement = { 1 << 16, 1 << 16 };

	Point baseline_current = { -1, -1 };
	Point baseline_movement = { 1 << 16, 1 << 16 };

	Selection selection = { { -1, -1 }, { -1, -1 } };
};
